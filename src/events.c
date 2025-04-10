#include "events.h"

guint ismoving = 0;
void exit_window(GtkWidget *window)
{

	if (restarting)
	{
		gtk_widget_destroy(window);
		restarting = 0;
		readconf();
		create_window();
	}
	else
	{
		const gchar *lock_file_path = "/tmp/sglauncher.lock";
		GFile *lock_file = g_file_new_for_path(lock_file_path);
		GError *error = NULL;

		if (g_file_query_exists(lock_file, NULL))
		{
			g_file_delete(lock_file, NULL, &error);
			if (error)
			{
				g_printerr("Error removing lock file: %s\n", error->message);
				g_error_free(error);
			}
			else
			{
				g_info("Lock file removed. Program finished.\n");
			}
		}
		else
		{
			g_info("Lock file does not exist. No action taken.\n");
		}
		exit(0);
	}
}

gchar* probe_icons_from_theme(GPtrArray *icon_names)
{
	theme = gtk_icon_theme_get_default();
	gint i;

	for (i = 0; i < icon_names->len; i++)
	{
		gchar *icon_name = g_ptr_array_index(icon_names, i);
		info = gtk_icon_theme_lookup_icon(theme, icon_name, 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			if (icon != NULL)
			{
				g_object_unref(icon);
				g_object_unref(info);
				return g_strdup(icon_name);
			}
			g_object_unref(info);
		}
	}
	return g_strdup("\0");
}

void window_set_icon(GtkWindow *target, gchar *iconname)
{
	info = gtk_icon_theme_lookup_icon(theme, iconname, 16, 16);
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(target, icon);
		g_object_unref(icon);
		g_object_unref(info);
}

gboolean applist_show_menu(GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		ismoving = 1;

		GtkTreeView *tree_view = GTK_TREE_VIEW(widget);
		GtkIconView *icon_view = GTK_ICON_VIEW(iconview);
		GtkTreePath *path = NULL;
		GtkTreeModel *model = NULL;
		GtkTreeIter iter;

		gchar *app_comment = NULL;
		gchar *toexec = NULL;
		gchar *cmdexec = NULL;
		gchar *fmexec = NULL;
		gchar *app_path = NULL;
		gchar *entrypath = NULL;

		if (useiconview == 1)
		{
			path = gtk_icon_view_get_path_at_pos(icon_view, (gint)event->x, (gint)event->y);
			if (path != NULL)
			{
				gtk_icon_view_select_path(icon_view, path);
				gtk_icon_view_set_cursor(icon_view, path, NULL, FALSE);
				model = gtk_icon_view_get_model(icon_view);
			}
		}
		else if (useiconview == 0)
		{
			if (gtk_tree_view_get_path_at_pos(tree_view, (gint)event->x, (gint)event->y, &path, NULL, NULL, NULL))
			{
				GtkTreeSelection *selection = gtk_tree_view_get_selection(tree_view);
				gtk_tree_selection_select_path(selection, path);
				gtk_tree_view_set_cursor(tree_view, path, NULL, FALSE);
				model = gtk_tree_view_get_model(tree_view);
			}
		}

		if (path != NULL && model != NULL)
		{
			if (gtk_tree_model_get_iter(model, &iter, path))
			{
				gtk_tree_model_get(model, &iter, 4, &app_comment, 1, &toexec, 5, &app_path, 6, &entrypath, -1);
				cmdexec = g_strdup_printf("%s -e /bin/sh -c \"%s %s\"", terminal, toexec, ccloseterm);
				fmexec = g_strdup_printf("xdg-open %s", app_path);

				GtkWidget *menu = gtk_menu_new(),
				*menu_item_run = gtk_menu_item_new_with_label(_("Run")),
				*menu_item_runt = gtk_menu_item_new_with_label(_("Run in terminal")),
				*menu_item_showfm = gtk_menu_item_new_with_label(_("Show entry in file manager")),
				*menu_item_addqa = gtk_menu_item_new_with_label(_("Add to Quick access")),
				*menu_item_comment = gtk_menu_item_new_with_label(app_comment);
					gtk_menu_set_reserve_toggle_size(GTK_MENU(menu), FALSE);

				gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_comment);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_run);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_runt);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_showfm);
				if (showqa)
				{
					gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_addqa);
				}

				gtk_widget_set_sensitive(menu_item_comment, FALSE);

				if (gtk_tree_model_iter_has_child(model, &iter))
				{
					gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
					gint n_children = gtk_tree_model_iter_n_children(model, &iter);
					for (gint i = 0; i < n_children; i++)
					{
						GtkTreeIter child_iter;
						if (gtk_tree_model_iter_nth_child(model, &child_iter, &iter, i))
						{
							gchar *child_label = NULL;
							gchar *child_cmd = NULL;
							gtk_tree_model_get(model, &child_iter, 0, &child_label, 1, &child_cmd, -1);

							GtkWidget *child_menu_item = gtk_menu_item_new_with_label(child_label);
							gtk_menu_shell_append(GTK_MENU_SHELL(menu), child_menu_item);

							g_signal_connect(child_menu_item, "activate", G_CALLBACK(run_command), (gpointer)child_cmd);

							g_free(child_label);
						}
					}
				}

				gtk_widget_show_all(menu);

				g_signal_connect(menu_item_run, "activate", G_CALLBACK(run_command), (gpointer)toexec);
				g_signal_connect(menu_item_runt, "activate", G_CALLBACK(run_command), (gpointer)cmdexec);
				g_signal_connect(menu_item_showfm, "activate", G_CALLBACK(run_command), (gpointer)fmexec);
				g_signal_connect(menu_item_addqa, "activate", G_CALLBACK(create_quickaccess), (gpointer)entrypath);

				gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent*)event);

				return TRUE;
			}
		}
	}
	return FALSE;
}

gboolean match_func(GtkEntryCompletion *completion, const gchar *key_string, GtkTreeIter *iter, gpointer user_data)
{
	GtkTreeModel *model = gtk_entry_completion_get_model(completion);
	gchar *command;
	gtk_tree_model_get(model, iter, 0, &command, -1);

	if (key_string[0] != *commandsprefix)
	{
		g_free(command);
		return FALSE;
	}

	const gchar *typed = key_string + 1;

	gchar *command_lower = g_ascii_strdown(command, -1);
	gchar *typed_lower = g_ascii_strdown(typed, -1);

	gboolean result = g_str_has_prefix(command_lower, typed_lower);

	g_free(command);
	g_free(command_lower);
	g_free(typed_lower);

	return result;
}

gboolean is_symlink(const gchar *path)
{
	struct stat path_stat;
	if (lstat(path, &path_stat) == 0)
	{
		return S_ISLNK(path_stat.st_mode);
	}
	return FALSE;
}

GList *get_executables_from_path(void)
{
	GList *executables = NULL;
	GHashTable *seen = g_hash_table_new(g_str_hash, g_str_equal);

	gchar *path = g_strdup(g_getenv("PATH"));
	if (!path)
	{
		return NULL;
	}

	gchar **path_dirs = g_strsplit(path, ":", -1);

	for (gint i = 0; path_dirs[i] != NULL; i++)
	{
		if (is_symlink(path_dirs[i]))
		{
			continue;
		}

		DIR *dir = opendir(path_dirs[i]);
		if (!dir)
		{
			continue;
		}

		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			gchar *full_path = g_build_filename(path_dirs[i], entry->d_name, NULL);

			if (g_file_test(full_path, G_FILE_TEST_IS_EXECUTABLE) &&
				!g_file_test(full_path, G_FILE_TEST_IS_DIR))
			{
				if (!g_hash_table_contains(seen, entry->d_name))
				{
					executables = g_list_prepend(executables, g_strdup(entry->d_name));
					g_hash_table_add(seen, g_strdup(entry->d_name));
				}
			}

			g_free(full_path);
		}

		closedir(dir);
	}

	g_hash_table_destroy(seen);
	g_strfreev(path_dirs);
	g_free(path);

	return executables;
}

gboolean on_completion_match_selected(GtkEntryCompletion *completion, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	GtkEntry *entry = GTK_ENTRY(user_data);

	gchar *cmd;
	gtk_tree_model_get(model, iter, 0, &cmd, -1);

	gtk_entry_set_text(entry, cmd);

	gtk_widget_activate(cmd_row);

	g_free(cmd);

	return TRUE;
}
