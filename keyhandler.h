static gboolean on_filter_visible(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	FilterData *filterdata = (FilterData *)data;
	gchar *name;
	gboolean visible = FALSE;

	gtk_tree_model_get(model, iter, 3, &name, -1);

	gchar *name_lower = g_utf8_strdown(name, -1);
	gchar *filter_text_lower = g_utf8_strdown(filterdata->filter_text, -1);

	visible = (name_lower && filter_text_lower && g_strrstr(name_lower, filter_text_lower) != NULL);

	if (!visible && gtk_tree_model_iter_has_child(model, iter))
	{
		GtkTreeIter child_iter;
		gboolean valid_child = gtk_tree_model_iter_children(model, &child_iter, iter);
		while (valid_child && !visible)
		{
			visible = on_filter_visible(model, &child_iter, data);
			valid_child = gtk_tree_model_iter_next(model, &child_iter);
		}

		if (visible)
		{
			GtkTreePath *path = gtk_tree_model_get_path(model, iter);
			gtk_tree_view_expand_to_path(filterdata->treeview, path);
			gtk_tree_path_free(path);
		}
	}

	g_free(name_lower);
	g_free(filter_text_lower);
	g_free(name);

	return visible;
}

static void on_entry_changed(GtkEntry *target, FilterData *filterdata)
{
	if (filterdata->filter == NULL)
	{
		return;
	}

	g_free(filterdata->filter_text);
	filterdata->filter_text = g_strdup(gtk_entry_get_text(target));

	gtk_tree_model_filter_refilter(filterdata->filter);

	GtkTreeIter iter;
	GtkTreeModel *model = GTK_TREE_MODEL(filterdata->filter);
	gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

	while (valid)
	{
		if (on_filter_visible(model, &iter, filterdata))
		{
			GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
			gtk_tree_path_free(path);
		}
		valid = gtk_tree_model_iter_next(model, &iter);
	}

	if (filterdata->filter_text != NULL)
	{
		if (strlen(filterdata->filter_text) > 0) 
		{
			double minscientific = 999999;
			double result = evaluate((char*)filterdata->filter_text);
			char buffer[256];
			if (result < minscientific)
			{
				snprintf(buffer, 256, "%g", result);
			}
			else if (result > minscientific && showscientific == 0)
			{
				snprintf(buffer, 256, "%f", result);
			}
			else if (result > minscientific && showscientific == 1)
			{
				snprintf(buffer, 256, "%g", result);
			}

			const gchar *message = isnan(result) ? "Invalid operation" : buffer;
			gtk_label_set_text(GTK_LABEL(manswer), message);

			gtk_widget_show(mathtext);
			gtk_widget_show(math);
			gtk_widget_show(manswer);
		}
		else if (strlen(filterdata->filter_text) > 0 && !isdigit(filterdata->filter_text[0]))
		{
			gtk_widget_show(mathtext);
			gtk_widget_hide(math);
			gtk_widget_hide(manswer);
			gtk_widget_show(listbox2);
		}
		else
		{
			gtk_widget_hide(listbox2);
		}
	}
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data) 
{
	//const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
	if (event->keyval == GDK_KEY_Escape || (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK))) 
	{
		exit_window();
		return TRUE;
	}
	else if (event->keyval == GDK_KEY_Escape || ((event->state & GDK_CONTROL_MASK) && 
		(event->keyval == GDK_KEY_m || event->keyval == GDK_KEY_t)))
	{
		gtk_widget_activate(cmd_row);
	}

	else if (event->keyval == GDK_KEY_Return && gtk_widget_has_focus(entry))
	{
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(applist));
		GtkTreeIter iter;
		GtkTreePath *path;
		gchar *app_name = NULL;
		const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));

		if (gtk_tree_model_get_iter_first(model, &iter))
		{
			gtk_tree_model_get(model, &iter, 0, &app_name, -1);
			gchar *entry_text_lower = g_ascii_strdown(entry_text, -1);
			gchar *app_name_lower = g_ascii_strdown(app_name, -1);

			if (gtk_tree_model_iter_has_child(model, &iter))
			{
				if (g_strstr_len(app_name_lower, -1, entry_text_lower) == NULL)
				{
					GtkTreeIter child_iter;
					if (gtk_tree_model_iter_children(model, &child_iter, &iter))
					{
						path = gtk_tree_model_get_path(model, &child_iter);
						gtk_tree_view_set_cursor(GTK_TREE_VIEW(applist), path, NULL, FALSE);
						gtk_tree_view_row_activated(GTK_TREE_VIEW(applist), path, NULL);
						gtk_tree_path_free(path);
						g_free(app_name);
						return 0;
					}
				}
				else
				{
					path = gtk_tree_model_get_path(model, &iter);
					gtk_tree_view_set_cursor(GTK_TREE_VIEW(applist), path, NULL, FALSE);
					gtk_tree_view_row_activated(GTK_TREE_VIEW(applist), path, NULL);
					gtk_tree_path_free(path);
				}
			}
			else
			{
				path = gtk_tree_model_get_path(model, &iter);
				gtk_tree_view_set_cursor(GTK_TREE_VIEW(applist), path, NULL, FALSE);
				gtk_tree_view_row_activated(GTK_TREE_VIEW(applist), path, NULL);
				gtk_tree_path_free(path);
			}
			g_free(app_name);
		}
	}

	else if(event->keyval == GDK_KEY_Down && gtk_widget_has_focus(entry))
	{
		if (useiconview)
			gtk_widget_grab_focus(iconview);
		else
			gtk_widget_grab_focus(applist);
	}
	else if((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_b))
	{
		gtk_widget_activate(web_row);
	}
	return FALSE;
}

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		submenu = GTK_WIDGET(data);
		gtk_menu_popup_at_pointer(GTK_MENU(submenu), NULL);
		return TRUE;
	}
	else if (event->type == GDK_BUTTON_PRESS && event->button == 1)
	{
		ismoving = 1;
	}
	return FALSE;
}

gboolean close_window_if_unfocused(gpointer widget)
{
	if (gtk_widget_get_visible(submenu) || gtk_widget_get_visible(dialog) || ismoving)
	{
		ismoving = 0;
		return FALSE;
	}

	GdkModifierType modifier_state = gdk_keymap_get_modifier_state(gdk_keymap_get_for_display(gdk_display_get_default()));
	GdkSeat *seat = gdk_display_get_default_seat(gdk_display_get_default());
	GdkDevice *pointer = gdk_seat_get_pointer(seat);
	guint button_state = 0;

	gdk_device_get_state(pointer, gtk_widget_get_window(GTK_WIDGET(widget)), NULL, &button_state);

	if (!gtk_window_has_toplevel_focus(GTK_WINDOW(widget)) && button_state == 0 &&
		!(modifier_state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SUPER_MASK)))
	{
		gtk_widget_destroy(GTK_WIDGET(widget));
	}
	return FALSE;
}

gboolean on_focus_out(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
	g_timeout_add(100, close_window_if_unfocused, widget);
	return FALSE;
}
