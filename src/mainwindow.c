#include "mainwindow.h"

GtkWidget *math;
GtkWidget *xdg_box;
GtkWidget *xdg_row;
GtkWidget *dialog;
GtkWidget *window;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *cmd_row;
GtkWidget *web_row;
GtkWidget *web_box;
GtkWidget *entry;
GtkWidget *manswer;
GtkWidget *mathtext;
GtkWidget *listbox2;
GtkWidget *qa;
GtkWidget *row;
GtkWidget *headerbar;
GtkWidget *image;
GtkWidget *wtitle;
GtkWidget *submenu;
GtkWidget *submenu_item_onlinehelp;
GtkWidget *submenu_menu_help;
GtkWidget *submenu_item_help;
GtkWidget *submenu_item_settings;
GtkWidget *submenu_item_quickhelp;
GtkWidget *submenu_item_about;
GtkWidget *submenu_item4;
GtkWidget *submenu_item5;
GtkWidget *scrolled_window;
GtkWidget *iconview;
GtkWidget *applist;

GPtrArray *program_icon_names;
GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

void on_submenu_item_about_selected(GtkMenuItem *menuitem, gpointer userdata)
{
	dialog = gtk_about_dialog_new();
		window_set_icon(GTK_WINDOW(dialog), program_icon);
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGLaucher");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2025 ItsZariep");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Simple GTK Launcher");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itszariep.codeberg.page/projects/sglauncher/");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), _("Project Website"));
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),program_icon);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


void on_submenu_item_onlinehelp_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	const gchar *link = "https://codeberg.org/ItsZariep/SGLauncher/wiki/?action=_pages";

	dialog = gtk_dialog_new_with_buttons(_("Documentation available on Codeberg"), NULL, GTK_DIALOG_MODAL, NULL, NULL);
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *info_image = gtk_image_new_from_icon_name("dialog-information", GTK_ICON_SIZE_DIALOG);
	GtkWidget *label = gtk_label_new(_("Documentation available at:"));

	gchar *markup = g_strdup_printf("<a href=\"%s\">%s</a>", link, link);
	GtkWidget *link_label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(link_label), markup);
	g_free(markup);

	gtk_label_set_use_markup(GTK_LABEL(link_label), TRUE);
	gtk_label_set_use_underline(GTK_LABEL(link_label), TRUE);

	gtk_box_pack_start(GTK_BOX(box), info_image, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), link_label, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), box);
	gtk_widget_show_all(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void create_window(void)
{
	program_icon_names = g_ptr_array_new();
	g_ptr_array_add(program_icon_names, "sglauncher");
	g_ptr_array_add(program_icon_names, "menulibre");
	g_ptr_array_add(program_icon_names, "application-menu");
	g_ptr_array_add(program_icon_names, "application-menu-symbolic");
	g_ptr_array_add(program_icon_names, "gnome-menu");
	g_ptr_array_add(program_icon_names, "start-here");
	program_icon = probe_icons_from_theme(program_icon_names);

	listbox2 = gtk_list_box_new();
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, msizex, msizey);
	gtk_window_set_title(GTK_WINDOW(window), "SGLauncher");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(window, "destroy", G_CALLBACK(exit_window), window);

	window_set_icon(GTK_WINDOW(window), program_icon);

	headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);

	entry = gtk_entry_new();

	button = gtk_menu_button_new();
	GtkIconTheme *icon_theme = gtk_icon_theme_get_default();
	GdkPixbuf *pixbuf = gtk_icon_theme_load_icon(icon_theme, program_icon, 64, GTK_ICON_LOOKUP_USE_BUILTIN, NULL);

	GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 16, 16, GDK_INTERP_BILINEAR);

	image = gtk_image_new_from_pixbuf(scaled_pixbuf);

	gtk_container_add(GTK_CONTAINER(button), image);

	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);

	submenu = gtk_menu_new();
	gtk_menu_set_reserve_toggle_size(GTK_MENU(submenu), FALSE);

	if (!nohome)
	{
		submenu_item_settings = gtk_menu_item_new_with_label(_("Settings"));
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item_settings);
	}

	submenu_item_help = gtk_menu_item_new_with_label(_("Help"));
	submenu_menu_help = gtk_menu_new();
		gtk_menu_set_reserve_toggle_size(GTK_MENU(submenu_menu_help), FALSE);
		submenu_item_onlinehelp = gtk_menu_item_new_with_label(_("Online help"));
		submenu_item_about = gtk_menu_item_new_with_label(_("About"));

	gtk_menu_shell_append(GTK_MENU_SHELL(submenu_menu_help), submenu_item_quickhelp);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu_menu_help), submenu_item_onlinehelp);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu_menu_help), submenu_item_about);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenu_item_help), submenu_menu_help);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item_help);

	gtk_widget_show_all(submenu);

	gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);

	if (!nocsd)
	{
		gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
		if (!hidetitle)
		{
			wtitle = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(wtitle), "<b>SGLauncher</b>");
			gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
			gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), entry);
		}
		else
		{
			GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
			gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);
			gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), box);
			gtk_widget_set_hexpand(entry, TRUE);
			gtk_widget_set_hexpand(box, TRUE);
		}
	}
	else
	{
		if (hidewindeco)
		{
			gtk_window_set_decorated(GTK_WINDOW(window), !hidewindeco);
			gtk_container_set_border_width(GTK_CONTAINER(window), 5);
		}
	}

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_hide(gtk_scrolled_window_get_hscrollbar(GTK_SCROLLED_WINDOW(scrolled_window)));
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
		GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);

	iconview = gtk_icon_view_new();
		gtk_icon_view_set_activate_on_single_click(GTK_ICON_VIEW(iconview),TRUE);
		gtk_icon_view_set_item_padding(GTK_ICON_VIEW(iconview),0);
	applist = gtk_tree_view_new();
		gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(applist), TRUE);
		gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(applist), FALSE);

	if (useiconview)
		gtk_container_add(GTK_CONTAINER(scrolled_window), iconview);
	else
		gtk_container_add(GTK_CONTAINER(scrolled_window), applist);

	gtk_widget_grab_focus(entry);

	cmd_row = gtk_list_box_row_new();
	GtkWidget *cmd_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_add(GTK_CONTAINER(cmd_row), cmd_box);

	GtkWidget *cmdicon = gtk_image_new_from_icon_name("terminal-tango", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(cmd_box), cmdicon, FALSE, FALSE, 0);

	GtkWidget *cmdrun = gtk_label_new(_("Run in Terminal"));
	gtk_box_pack_start(GTK_BOX(cmd_box), cmdrun, FALSE, FALSE, 0);

	web_row = gtk_list_box_row_new();
	web_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_container_add(GTK_CONTAINER(web_row), web_box);

	GtkWidget *webicon = gtk_image_new_from_icon_name("edit-find", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(web_box), webicon, FALSE, FALSE, 0);

	GtkWidget *webrun = gtk_label_new(_("Search on Web"));
	gtk_box_pack_start(GTK_BOX(web_box), webrun, FALSE, FALSE, 0);

	xdg_row = gtk_list_box_row_new();
	xdg_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_add(GTK_CONTAINER(xdg_row), xdg_box);


	GtkWidget *xdgicon = gtk_image_new_from_icon_name("folder", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(xdg_box), xdgicon, FALSE, FALSE, 0);

	GtkWidget *xdgrun = gtk_label_new(_("Open file/directory"));
	gtk_box_pack_start(GTK_BOX(xdg_box), xdgrun, FALSE, FALSE, 0);

	if (showcmd == 1)
	{
		gtk_list_box_insert(GTK_LIST_BOX(listbox2), cmd_row, -1);
	}
	if (showweb == 1)
	{
		gtk_list_box_insert(GTK_LIST_BOX(listbox2), web_row, -1);
	}
	if (showofd == 1)
	{
		gtk_list_box_insert(GTK_LIST_BOX(listbox2), xdg_row, -1);
	}

	gtk_widget_set_size_request(web_row, -1, 32);
	gtk_widget_set_size_request(cmd_row, -1, 32);

	g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), NULL);
	g_signal_connect(submenu_item_settings, "activate", G_CALLBACK(showcfg), NULL);
	g_signal_connect(submenu_item_about, "activate", G_CALLBACK(on_submenu_item_about_selected), NULL);
	g_signal_connect(submenu_item_onlinehelp, "activate", G_CALLBACK(on_submenu_item_onlinehelp_selected), NULL);
	g_signal_connect(listbox2, "row-activated", G_CALLBACK(on_run_command), entry);
	g_signal_connect(applist, "row-activated", G_CALLBACK(on_item_activated), NULL);
	g_signal_connect(iconview, "item-activated", G_CALLBACK(on_item_activated), NULL);

	g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press), submenu);

	if (exitwhenunfocused)
	{
		g_signal_connect(window, "focus-out-event", G_CALLBACK(on_focus_out), window);
	}

	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	mathtext = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	qa = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	math = gtk_label_new(_("Math Answer: "));
	manswer = gtk_label_new("");
	gtk_label_set_selectable(GTK_LABEL(manswer), TRUE);

	if (showcalc == 1)
	{
		gtk_box_pack_start(GTK_BOX(mathtext), math, TRUE, TRUE, 0);
		gtk_box_pack_start(GTK_BOX(mathtext), manswer, TRUE, TRUE, 0);
	}

	gtk_widget_set_vexpand(scrolled_window, TRUE);
	gtk_widget_set_hexpand(scrolled_window, TRUE);

	if (mlayout == 1)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
	}
	else if (mlayout == 0)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 4, 1, 1);
	}
	else if (mlayout == 2)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 1, 3, 1, 1);
	}
	else if (mlayout == 3)
	{
		gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
	}

	gtk_widget_set_size_request(listbox2, -1, -1);
	gtk_widget_set_size_request(scrolled_window, -1, 256);

	if (nocsd)
	{
		if (entryonbottom)
		{
			gtk_grid_attach(GTK_GRID(grid), mathtext, 0, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry, 0, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), qa, 0, 7, 1, 1);
		}
		else
		{
			gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), qa, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), mathtext, 0, 2, 1, 1);
		}
	}
	else
	{
		gtk_grid_attach(GTK_GRID(grid), qa, 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), mathtext, 0, 2, 1, 1);
	}

	if (sgcfg)
	{
		gtk_widget_activate(submenu_item_settings);
	}
	else
	{
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		load_apps(GTK_TREE_VIEW(applist), GTK_ICON_VIEW(iconview));

		filter_model = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(sorted_model), NULL));

		filter_data.filter_text = g_strdup("");
		filter_data.filter = filter_model;
		filter_data.treeview = GTK_TREE_VIEW(applist);
		filter_data.iconview = GTK_ICON_VIEW(iconview);

		gtk_tree_model_filter_set_visible_func(filter_model,
			(GtkTreeModelFilterVisibleFunc) on_filter_visible, &filter_data, NULL);
			if (useiconview)
				gtk_icon_view_set_model(GTK_ICON_VIEW(iconview), GTK_TREE_MODEL(filter_model));
			else
				gtk_tree_view_set_model(GTK_TREE_VIEW(applist), GTK_TREE_MODEL(filter_model));
			filter_data.entry = GTK_ENTRY(entry);

		g_signal_connect(entry, "changed", G_CALLBACK(on_entry_changed), &filter_data);
		g_signal_connect(applist, "button-press-event", G_CALLBACK(applist_show_menu), NULL);
		g_signal_connect(iconview, "button-press-event", G_CALLBACK(applist_show_menu), NULL);
		gtk_widget_show_all(window);
		gtk_widget_hide(mathtext);
		gtk_widget_hide(listbox2);
		gtk_window_present(GTK_WINDOW(cfgdialog));

		gtk_main();
	}
}
