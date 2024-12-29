void load_quickaccess(void);

static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer menu)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);
		return TRUE;
	}
	return FALSE;
}

gint compare_filenames(gconstpointer a, gconstpointer b)
{
	const gchar *name1 = (const gchar *)a;
	const gchar *name2 = (const gchar *)b;

	gint num1 = atoi(name1);
	gint num2 = atoi(name2);

	return (num1 - num2);
}

void rearrageqa(const gchar *folder_path)
{
	GDir *dir;
	const gchar *filename;
	GError *error = NULL;

	dir = g_dir_open(folder_path, 0, &error);
	if (!dir) {
		g_printerr("Error opening directory: %s\n", error->message);
		g_clear_error(&error);
		return;
	}

	GList *file_list = NULL;

	while ((filename = g_dir_read_name(dir)) != NULL)
	{
		gchar *full_path = g_build_filename(folder_path, filename, NULL);
		if (g_file_test(full_path, G_FILE_TEST_IS_REGULAR))
		{
			file_list = g_list_append(file_list, g_strdup(filename));
		}
		g_free(full_path);
	}

	g_dir_close(dir);

	file_list = g_list_sort(file_list, (GCompareFunc)g_strcmp0);

	guint counter = 0;
	for (GList *l = file_list; l != NULL; l = l->next)
	{
		gchar *old_name = l->data;
		gchar *old_path = g_build_filename(folder_path, old_name, NULL);
		gchar *new_name = g_strdup_printf("%u.desktop", counter++);
		gchar *new_path = g_build_filename(folder_path, new_name, NULL);

		GFile *file1 = g_file_new_for_path(old_path);
		GFile *file2 = g_file_new_for_path(new_path);

		if (!g_file_move(file1, file2, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &error))
		{
			g_printerr("Failed to rename %s to %s: %s\n", old_path, new_path, error->message);
			g_clear_error(&error);
		}

		g_object_unref(file1);
		g_object_unref(file2);

		g_free(old_name);
		g_free(old_path);
		g_free(new_name);
		g_free(new_path);
	}
	g_list_free(file_list);
}

void reloadqa(void)
{
	GtkWidget *child;
	GList *children, *iter;

	children = gtk_container_get_children(GTK_CONTAINER(qa));

	for (iter = children; iter != NULL; iter = iter->next)
	{
		child = GTK_WIDGET(iter->data);
		gtk_widget_destroy(child);
	}
}

void swapfiles(GtkWidget *input, gpointer arg)
{
	gint optype = GPOINTER_TO_INT(arg); 
	const gchar *inputname = gtk_widget_get_name(input);
	gint inputvalue = atoi(inputname);

	if (g_str_has_suffix(inputname, ".desktop"))
	{
		size_t len = strlen(inputname) - strlen(".desktop");
		gchar *new_name = g_strndup(inputname, len);
		g_free(new_name);
	}
	else
	{
		g_warning("input: %s is not a .desktop file, please report this issue", inputname);
		return;
	}

	const gchar *qadir = g_build_filename(g_get_user_data_dir(), "sglauncher", "quickaccess", NULL);
	gchar *file1_path = g_build_filename(qadir, inputname, NULL);

	gint target = (optype == 0) ? inputvalue - 1 : inputvalue + 1;
	gchar *targetname = g_strconcat(g_strdup_printf("%d", target), ".desktop", NULL);
	gchar *file2_path = g_build_filename(qadir, targetname, NULL);

	GFile *file1 = g_file_new_for_path(file1_path);
	GFile *file2 = g_file_new_for_path(file2_path);

	if (g_file_test(file2_path, G_FILE_TEST_EXISTS))
	{
		gchar *tempname = g_strconcat(targetname, ".temp", NULL);
		gchar *tempname_path = g_build_filename(qadir, tempname, NULL);
		GFile *tempfile = g_file_new_for_path(tempname_path);

		if (g_file_move(file2, tempfile, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL))
		{
			if (g_file_move(file1, file2, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL))
			{
				g_file_move(tempfile, file1, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);
			}
		}

		g_free(tempname);
		g_free(tempname_path);
		g_object_unref(tempfile);
	}
	else
	{
		g_file_move(file1, file2, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);
	}

	g_free(file1_path);
	g_free(file2_path);
	g_free(targetname);
	g_object_unref(file1);
	g_object_unref(file2);
	reloadqa();
	load_quickaccess();
}


void removeqa(GtkWidget *input, gpointer data)
{
	gchar *inputfile = (gchar *)data;

	if (inputfile == NULL || *inputfile == '\0')
	{
		g_print("Error: inputfile is NULL or empty\n");
		return;
	}

	GFile *target = g_file_new_for_path(inputfile);

	GError *error = NULL;

	if (g_file_delete(target, NULL, &error))
	{
		g_print("File removed: %s\n", inputfile);
	}
	else
	{
		g_print("Error removing file: %s\n", error->message);
		g_clear_error(&error);
	}

	g_object_unref(target);


	const gchar *qadir = g_build_filename(g_get_user_data_dir(), "sglauncher", "quickaccess", NULL);
	rearrageqa(qadir);
	reloadqa();
	load_quickaccess();
}

void load_quickaccess(void)
{
	const gchar *qadir = g_build_filename(g_get_user_data_dir(), "sglauncher", "quickaccess", NULL);

	DIR *dir = opendir(qadir);
	if (dir == NULL)
	{
		g_warning("Could not open directory: %s", qadir);
		g_free((gchar *)qadir);
		return;
	}

	struct dirent *ent;
	GList *file_list = NULL;

	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] == '.' || !g_str_has_suffix(ent->d_name, ".desktop"))
		{
			continue;
		}

		file_list = g_list_prepend(file_list, g_strdup(ent->d_name));
	}

	closedir(dir);

	file_list = g_list_sort(file_list, compare_filenames);

	for (GList *l = file_list; l != NULL; l = l->next)
	{
		gchar *file_name = (gchar *)l->data;
		gchar *path = g_build_filename(qadir, file_name, NULL);

		GKeyFile *key_file = g_key_file_new();
		GError *error = NULL;

		if (!g_key_file_load_from_file(key_file, path, G_KEY_FILE_NONE, &error))
		{
			g_warning("Error loading .desktop file: %s", error->message);
			g_error_free(error);
			g_free(path);
			g_key_file_free(key_file);
			continue;
		}

		gchar *app_name = g_key_file_get_locale_string(key_file, "Desktop Entry", "Name", NULL, NULL);
		gchar *app_comment = g_key_file_get_locale_string(key_file, "Desktop Entry", "Comment", NULL, NULL);
		gchar *icon_name = g_key_file_get_string(key_file, "Desktop Entry", "Icon", NULL);
		gchar *toexec = g_key_file_get_string(key_file, "Desktop Entry", "Exec", NULL);

		GdkPixbuf *icon_pixbuf = NULL;

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

		if (!icon_pixbuf)
		{
			g_warning("Failed to load icon: %s, error: %s", icon_name, error ? error->message : "unknown error");
			g_clear_error(&error);
			g_free(path);
			g_free(app_name);
			g_free(app_comment);
			g_free(icon_name);
			g_free(toexec);
			g_key_file_free(key_file);
			continue;
		}

		GtkWidget *qaimage = gtk_image_new_from_pixbuf(icon_pixbuf);
		g_object_unref(icon_pixbuf);

		GtkWidget *qabutton = gtk_button_new();
		gtk_button_set_image(GTK_BUTTON(qabutton), qaimage);
		gtk_widget_set_tooltip_text(qabutton, app_name);
		gtk_widget_set_name(qabutton, file_name);

		GtkWidget *menu,*menu_item_comment,*menu_item_run,*menu_item_runterm,
			*menu_item_moveleft,*menu_item_moveright, *menu_item_remove;

		menu = gtk_menu_new();
		gtk_menu_set_reserve_toggle_size(GTK_MENU(menu), FALSE);

		gchar *cmdexec = NULL;
		cmdexec = g_strdup_printf("%s -e /bin/sh -c \"%s %s\"", terminal, (gchar *)g_strdup(toexec), ccloseterm);

		menu_item_comment = gtk_menu_item_new_with_label(g_strdup(app_comment));
		menu_item_run = gtk_menu_item_new_with_label("Run");
		menu_item_runterm = gtk_menu_item_new_with_label("Run in a terminal");

		menu_item_moveleft = gtk_menu_item_new_with_label("Move to left");
		menu_item_moveright = gtk_menu_item_new_with_label("Move to right");
			gtk_widget_set_name(menu_item_moveleft, file_name);
			gtk_widget_set_name(menu_item_moveright, file_name);

	if (l == g_list_first(file_list))
	{
		gtk_widget_set_sensitive(menu_item_moveleft, FALSE);
	}
	else
	{
		g_signal_connect(menu_item_moveleft, "activate", G_CALLBACK(swapfiles), GINT_TO_POINTER(0));
	}

	if (l == g_list_last(file_list))
	{
		gtk_widget_set_sensitive(menu_item_moveright, FALSE);
	}
	else
	{
		g_signal_connect(menu_item_moveright, "activate", G_CALLBACK(swapfiles), GINT_TO_POINTER(1));
	}

		menu_item_remove = gtk_menu_item_new_with_label("Remove from Quick access");

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_comment);
			gtk_widget_set_sensitive(menu_item_comment, FALSE);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_run);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_runterm);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

			gchar **groups = g_key_file_get_groups(key_file, NULL);
			for (int j = 0; groups[j] != NULL; j++)
			{
				if (g_str_has_prefix(groups[j], "Desktop Action"))
				{
					gchar *action_name = g_key_file_get_locale_string(key_file, groups[j], "Name", NULL, NULL);
					gchar *exec_value = g_key_file_get_string(key_file, groups[j], "Exec", NULL);

					if (action_name && exec_value)
					{
						GtkWidget *daitem = gtk_menu_item_new_with_label(action_name);
						gtk_menu_shell_append(GTK_MENU_SHELL(menu), daitem);
						g_signal_connect(daitem, "activate", G_CALLBACK(run_command), (gpointer)g_strdup(exec_value));
					}
				}
			}
			g_strfreev(groups);

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_moveleft);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_moveright);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_remove);
		gtk_widget_show_all(menu);

			g_signal_connect(menu_item_run, "activate", G_CALLBACK(run_command), (gpointer)g_strdup(toexec));
			g_signal_connect(menu_item_runterm, "activate", G_CALLBACK(run_command), (gpointer)cmdexec);
			g_signal_connect(menu_item_remove, "activate", G_CALLBACK(removeqa), (gpointer)g_strdup(path));

		g_signal_connect(qabutton, "button-press-event", G_CALLBACK(on_button_press_event), menu);
		g_signal_connect(qabutton, "clicked", G_CALLBACK(run_command), (gpointer)g_strdup(toexec));

		gtk_box_pack_start(GTK_BOX(qa), qabutton, FALSE, FALSE, 2);

		gtk_widget_show_all(qa);
		g_free(app_name);
		g_free(app_comment);
		g_free(icon_name);
		g_free(toexec);
		g_free(path);
		g_key_file_free(key_file);
	}

	g_list_free_full(file_list, g_free);
	g_free((gchar *)qadir);
}

void create_quickaccess(GtkWidget *pass, gpointer data)
{
	const gchar *input = (gchar *)data;
	const gchar *qapath = g_build_filename(g_get_user_data_dir(), "sglauncher", "quickaccess", NULL);

	GError *error = NULL;
	if (g_mkdir_with_parents(qapath, 0755) != 0)
	{
		g_warning("Failed to create directory: %s", g_strerror(errno));
	}
	else {}

	gint index = 0;
	gchar *coutput;
	GFile *output;

	do
	{
		coutput = g_build_filename(qapath, g_strdup_printf("%d.desktop", index), NULL);
		output = g_file_new_for_path(coutput);

		if (!g_file_query_exists(output, NULL))
		{
			break;
		}

		g_object_unref(output);
		g_free(coutput);
		index++;
	} while (1);

	if (g_file_make_symbolic_link(output, input, NULL, &error))
	{
		g_info("Symbolic link created: %s -> %s\n", input, coutput);
	}
	else
	{
		g_warning("error creating file: %s\n", error->message);
		g_clear_error(&error);
	}

	reloadqa();
	load_quickaccess();
	g_object_unref(output);
	g_free(coutput);
	g_free((gchar *)qapath);
}
