void exit_window(void);
void run_command(GtkWidget *widget, gpointer data)
{
	gchar *input = (gchar *)data;
	g_info("Running input: %s", input);
	if (input)
	{
		gchar *percentPos = strchr(input, '%');
		if (percentPos != NULL)
		{
			memmove(percentPos, percentPos + 2, strlen(percentPos + 2) + 1);
		}
		GError *error = NULL;
		GPid pid;
		gboolean success = g_spawn_async_with_pipes(NULL,
		(gchar *[]){"/bin/sh", "-c", input, NULL}, NULL, G_SPAWN_SEARCH_PATH,
		NULL, NULL, &pid, NULL, NULL, NULL, &error);

		if (!success)
		{
			g_warning("Failed to start program: %s", error->message);
			g_error_free(error);
		}
		else
		{
			g_spawn_close_pid(pid);
			exit_window();
		}
	}
	else
	{
		g_warning("No command to execute.\n");
	}
}

void on_item_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *null, gpointer user_data)
{
	GtkTreeModel *model;
	if (useiconview)
		model = gtk_icon_view_get_model(GTK_ICON_VIEW(iconview));
	else
		model = gtk_tree_view_get_model(treeview);

	GtkTreeIter iter;

	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gchar *app_name = NULL;
		gchar *toexec = NULL;
		gchar *icon_name = NULL;

		gtk_tree_model_get(model, &iter, 0, &app_name, 1, &toexec, 2, &icon_name, -1);

		if (toexec)
		{
			run_command(applist, (gpointer)toexec);
		}
		g_free(app_name);
		g_free(toexec);
		g_free(icon_name);
	}
	else
	{
		g_warning("Failed to get iterator for path.\n");
	}
}

void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *input)
{
	const gchar *text = gtk_entry_get_text(GTK_ENTRY(input));
	gchar *cmd, *binary_name, *program_path, *arguments;

	GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox2));

	if ((void*)selected_row == (void*)cmd_row)
	{
		if (terminal == NULL) 
		{
			g_warning("TERMINAL environment variable not set\n");

			dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
				"TERMINAL environment variable not set. \n You can declare it in /etc/environment or in your ~/.profile \n E.G.: TERMINAL=urxvt");

			gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			return;
		}

		binary_name = g_strdup(text);
		arguments = strchr(binary_name, ' ');

		if (arguments != NULL)
		{
			*arguments = '\0';
			arguments++;
		}

		program_path = g_find_program_in_path(binary_name);
		if (program_path == NULL) 
		{
			return;
		}

		if (arguments != NULL)
			cmd = g_strdup_printf("%s -e %s %s", terminal, binary_name, arguments);
		else
			cmd = g_strdup_printf("%s -e %s", terminal, binary_name);

		run_command(applist, (gpointer)cmd);

		g_free(cmd);
		g_free(binary_name);
		g_free(program_path);
	} 
	else if ((void*)selected_row == (void*)web_row)
	{
		cmd = g_strdup_printf("xdg-open %s=%s", webengine, text);
		run_command(applist, (gpointer)cmd);
		g_free(cmd);
	}
	else if ((void*)selected_row == (void*)xdg_row)
	{
		cmd = g_strdup_printf("xdg-open %s", text);
		run_command(applist, (gpointer)cmd);
		g_free(cmd);
	}
}

