#include "entryloader.h"

FilterData filter_data = { NULL, NULL, NULL, NULL, NULL };
GtkTreeStore *store = NULL;
GtkTreeViewColumn *column = NULL;
GtkCellRenderer *renderer = NULL;
GtkTreeModelSort *sorted_model = NULL;
GtkTreeModelFilter *filter_model = NULL;


gint gtk_tree_iter_compare_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data)
{
	GtkTreeIter parent_a, parent_b;

	gboolean a_is_toplevel = !gtk_tree_model_iter_parent(model, &parent_a, a);
	gboolean b_is_toplevel = !gtk_tree_model_iter_parent(model, &parent_b, b);

	if (a_is_toplevel && b_is_toplevel)
	{
		gchar *name_a, *name_b;
		gtk_tree_model_get(model, a, 0, &name_a, -1);
		gtk_tree_model_get(model, b, 0, &name_b, -1);

		gint result = g_strcmp0(name_a, name_b);

		g_free(name_a);
		g_free(name_b);

		return result;
	}
	return 0;
}

void search_directory_recursively(const gchar *dir_path, GtkTreeStore *store, GtkTreeIter *parent_iter)
{
	DIR *dir = opendir(dir_path);
	if (dir == NULL)
	{
		return;
	}

	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] == '.') 
		{
			continue;
		}

		gchar *full_path = g_strdup_printf("%s/%s", dir_path, ent->d_name);

		if (g_file_test(full_path, G_FILE_TEST_IS_DIR))
		{
			if (searchrecursive)
			{
				search_directory_recursively(full_path, store, parent_iter);
			}
		}
		else if (g_str_has_suffix(ent->d_name, ".desktop"))
		{
			GKeyFile *key_file = g_key_file_new();
			GError *error = NULL;

			if (!g_key_file_load_from_file(key_file, full_path, G_KEY_FILE_NONE, &error))
			{
				g_warning("Error loading .desktop file: %s", error->message);
				g_error_free(error);
				g_free(full_path);
				g_key_file_free(key_file);
				continue;
			}

			if (g_key_file_get_boolean(key_file, "Desktop Entry", "NoDisplay", NULL) && ignorenodisplay)
			{
				g_free(full_path);
				g_key_file_free(key_file);
				continue;
			}

			if (deskenv)
			{
				gchar **show_in_list = NULL;
				gchar *only_show_in = g_key_file_get_string(key_file, "Desktop Entry", "OnlyShowIn", NULL);
				if (only_show_in)
				{
					show_in_list = g_strsplit(only_show_in, ";", -1);

					gboolean matches = FALSE;
					for (gchar **env = show_in_list; *env; env++)
					{
						if (g_strcmp0(*env, deskenv) == 0)
						{
							matches = TRUE;
							break;
						}
					}

					g_free(only_show_in);
					g_strfreev(show_in_list);
					if (!matches && !ignoreonlyshowin)
					{
						g_free(full_path);
						g_key_file_free(key_file);
						continue;
					}
				}
			}

			gchar *app_name = g_key_file_get_locale_string(key_file, "Desktop Entry", "Name", NULL, NULL);
			gchar *app_comment = g_key_file_get_locale_string(key_file, "Desktop Entry", "Comment", NULL, NULL);
			gchar *icon_name = g_key_file_get_string(key_file, "Desktop Entry", "Icon", NULL);
			gchar *toexec = g_key_file_get_string(key_file, "Desktop Entry", "Exec", NULL);
			GdkPixbuf *icon_pixbuf = NULL;

			if (g_key_file_get_boolean(key_file, "Desktop Entry", "Terminal", NULL))
			{
				if (ignoreterminal)
				{
					g_free(full_path);
					g_key_file_free(key_file);
					continue;
				}
				else
				{
					toexec = g_strdup_printf("%s -e %s", terminal, toexec);
				}
			}

			if (showappicons)
			{
				if (g_path_is_absolute(icon_name) && g_file_test(icon_name, G_FILE_TEST_EXISTS))
				{
					icon_pixbuf = gdk_pixbuf_new_from_file(icon_name, &error);
				}
				else
				{
					GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
					GtkIconInfo *icon_info = gtk_icon_theme_lookup_icon(icon_theme, icon_name, iconsize, GTK_ICON_LOOKUP_USE_BUILTIN);
					if (icon_info)
					{
						icon_pixbuf = gtk_icon_info_load_icon(icon_info, &error);
						g_object_unref(icon_info);
					}
				}

				if (icon_pixbuf)
				{
					GdkPixbuf *resized_icon = gdk_pixbuf_scale_simple(icon_pixbuf, iconsize, iconsize, GDK_INTERP_BILINEAR);
					g_object_unref(icon_pixbuf);
					icon_pixbuf = resized_icon;
				}
				else
				{
					icon_pixbuf = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(), "application-x-executable", iconsize, 0, NULL);
				}
			}

			gchar *dir_name = g_strdup(dir_path);
			GtkTreeIter app_iter;
			gtk_tree_store_append(store, &app_iter, parent_iter);
			gchar *merged_data = g_strdup_printf("%s%s%s", app_name, toexec, icon_name);
			gtk_tree_store_set(store, &app_iter, 0, app_name, 1, toexec, 2, showappicons ? icon_pixbuf : NULL, 3, merged_data, 4, app_comment, 5, dir_name, 6, full_path, -1);
			gtk_tree_view_set_tooltip_column(GTK_TREE_VIEW(applist), 4);
			gtk_icon_view_set_tooltip_column(GTK_ICON_VIEW(iconview), 4);

			g_free(app_name);
			g_free(icon_name);
			g_clear_object(&icon_pixbuf);
			g_free(toexec);
			g_free(merged_data);
			g_key_file_free(key_file);
		}
		g_free(full_path);
	}

	closedir(dir);
}

void load_apps(GtkTreeView *treeview, GtkIconView *iconview)
{
	const gchar *const *data_dirs = g_get_system_data_dirs();
	const gchar *user_dir = g_get_user_data_dir();

	size_t num_data_dirs = 0;
	for (const gchar *const *dir = data_dirs; *dir != NULL; dir++)
	{
		num_data_dirs++;
	}

	const gchar **app_dirs = g_malloc((num_data_dirs + 1 + 1) * sizeof(gchar *));
	size_t z = 0;
	for (const gchar *const *dir = data_dirs; *dir != NULL; dir++)
	{
		app_dirs[z] = g_strconcat(*dir, "/applications", NULL);
		z++;
	}

	app_dirs[z] = g_strconcat(user_dir, "/applications", NULL);
	z++;

	app_dirs[z] = NULL;

	store = gtk_tree_store_new(7, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer, "pixbuf", 2, NULL);
	gtk_tree_view_append_column(treeview, column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer, "text", 0, NULL);
	gtk_tree_view_append_column(treeview, column);

	sorted_model = GTK_TREE_MODEL_SORT(gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(store)));
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(sorted_model), 0, (GtkTreeIterCompareFunc)gtk_tree_iter_compare_func, NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(sorted_model), 0, GTK_SORT_ASCENDING);

	gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(sorted_model));
	gtk_icon_view_set_model(GTK_ICON_VIEW(iconview), GTK_TREE_MODEL(sorted_model));
	gtk_icon_view_set_text_column(GTK_ICON_VIEW(iconview), 0);
	gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(iconview), 2);

	for (int i = 0; app_dirs[i] != NULL; i++)
	{
		search_directory_recursively(app_dirs[i], store, NULL);
	}

	if (showqa)
	{
		load_quickaccess();
	}

	g_free(app_dirs);
}