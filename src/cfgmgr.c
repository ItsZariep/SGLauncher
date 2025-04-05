#include "cfgmgr.h"

GtkWidget *cfgdialog;
GtkWidget *weblabel;
GtkWidget *webcombo;
GtkWidget *webctm;
GtkWidget *wmlayout;
GtkWidget *wshowcmd;
GtkWidget *wshowweb;
GtkWidget *wshowcalc;
GtkWidget *wshowscientific;
GtkWidget *wexitwhenunfocused;
GtkWidget *defbtn;
GtkWidget *applybtn;
GtkWidget *wshowda;
GtkWidget *wusecsd;
GtkWidget *whidetitle;
GtkWidget *wentryonbottom;
GtkWidget *wiconsize;
GtkWidget *wuseiconview;
GtkWidget *wcloseterm;
GtkWidget *wignorenodisplay;
GtkWidget *wignoreterminal;
GtkWidget *whidewindeco;
GtkWidget *wignoreonlyshowin;
GtkWidget *wshowofd;
GtkWidget *wsingleinstance;
GtkWidget *wshowqa;
GtkWidget *wqasize;
GtkWidget *wshowappicons;
GtkWidget *wmsizex;
GtkWidget *wmsizey;
GtkWidget *cancelbtn;
GtkWidget *okbtn;

guint callconfig = 0;

void on_webcombo_changed(GtkComboBox *widget, gpointer user_data)
{
	webctm = GTK_WIDGET(user_data);

	if (gtk_combo_box_get_active(GTK_COMBO_BOX(webcombo)) == 3)
	{
		gtk_widget_show(webctm);
		gtk_widget_show(weblabel);
	}
	else
	{
		gtk_widget_hide(webctm);
		gtk_widget_hide(weblabel);
	}

	g_free((gpointer) active_text);
}

void on_cancelbutton_clicked(GtkWidget *widget, gpointer data)
{
	if (sgcfg == 1)
	{
		restarting = 0;
		exit_window(window);
	}
	disableunfocus = 0;
	callconfig = 0;
	ismoving = 0;
	gtk_widget_destroy(cfgdialog);
}

void on_dialog_destroy(GtkWidget *widget, gpointer data)
{
	if (sgcfg == 1)
	{
		restarting = 0;
		exit_window(window);
	}
	disableunfocus = 0;
	callconfig = 0;
	ismoving = 0;
}

void togglewidget(GtkWidget *input, gpointer data)
{
	GtkWidget *target= GTK_WIDGET(data);
	if (gtk_widget_get_visible(target)) 
	{
		gtk_widget_hide(target);
	}
	else
	{
		gtk_widget_show(target);
	}
}

void showcfg(void)
{
	callconfig = 1;
	ismoving = 1;
	cfgdialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(cfgdialog), "Settings - SGLauncher");
	gtk_container_set_border_width(GTK_CONTAINER(cfgdialog), 10);
	gtk_widget_set_size_request(cfgdialog, 400, 300);
	window_set_icon(GTK_WINDOW(cfgdialog), program_icon);

	GtkWidget *confbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *notebook = gtk_notebook_new();

		webcombo = gtk_combo_box_text_new();
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "DuckDuckGo");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Google");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Bing");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), _("Custom"));
		weblabel = gtk_label_new(_("Custom Search Engine"));
			webctm = gtk_entry_new();
		wshowcmd = gtk_check_button_new_with_label(_("Show \"Run in Terminal\" entry"));
			gtk_widget_set_direction(wshowcmd, GTK_TEXT_DIR_RTL);
		wshowweb = gtk_check_button_new_with_label (_("Show \"Search on Web\" entry"));
			gtk_widget_set_direction(wshowweb, GTK_TEXT_DIR_RTL);
		wshowofd = gtk_check_button_new_with_label (_("Show \"Open File/Directory\" entry"));
			gtk_widget_set_direction(wshowofd, GTK_TEXT_DIR_RTL);
		wshowcalc = gtk_check_button_new_with_label (_("Show Math answer when numbers are typed"));
			gtk_widget_set_direction(wshowcalc, GTK_TEXT_DIR_RTL);

		wmlayout = gtk_combo_box_text_new();
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wmlayout), _("Horizontal - Apps at top"));
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wmlayout), _("Horizontal - Apps at bottom"));
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wmlayout), _("Vertical - Apps at left"));
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(wmlayout), _("Vertical - Apps at right"));
		wshowda = gtk_check_button_new_with_label(_("Show Desktop actions"));
			gtk_widget_set_direction(wshowda, GTK_TEXT_DIR_RTL);
		wentryonbottom = gtk_check_button_new_with_label(_("Search entry on bottom"));
			gtk_widget_set_direction(wentryonbottom, GTK_TEXT_DIR_RTL);
		wshowqa = gtk_check_button_new_with_label(_("Show quick actions"));
			gtk_widget_set_direction(wshowqa, GTK_TEXT_DIR_RTL);
		wuseiconview = gtk_check_button_new_with_label(_("Use icon view"));
			gtk_widget_set_direction(wuseiconview, GTK_TEXT_DIR_RTL);
		wshowappicons = gtk_check_button_new_with_label(_("Show app icons"));
			gtk_widget_set_direction(wshowappicons, GTK_TEXT_DIR_RTL);
		GtkAdjustment *Icon_adjustment = gtk_adjustment_new(1, 0, 1024, 1, 1, 0);
			wiconsize = gtk_spin_button_new(Icon_adjustment, 1, 0);
			GtkWidget *wiconsize_label = gtk_label_new(_("Icon Size"));
		GtkAdjustment *qa_adjustment = gtk_adjustment_new(1, 0, 1024, 1, 1, 0);
			wqasize = gtk_spin_button_new(qa_adjustment, 1, 0);
			GtkWidget *wqasize_label = gtk_label_new(_("Quick Actions Size"));

		wcloseterm = gtk_check_button_new_with_label(_("Close terminal when process finishes"));
			gtk_widget_set_direction(wcloseterm, GTK_TEXT_DIR_RTL);
		wshowscientific = gtk_check_button_new_with_label(_("Use scientific notation on math answers"));
			gtk_widget_set_direction(wshowscientific, GTK_TEXT_DIR_RTL);
		wignorenodisplay = gtk_check_button_new_with_label(_("Ignore \"NoDisplay\" apps"));
			gtk_widget_set_direction(wignorenodisplay, GTK_TEXT_DIR_RTL);
		wignoreterminal = gtk_check_button_new_with_label(_("Ignore Terminal apps"));
			gtk_widget_set_direction(wignoreterminal, GTK_TEXT_DIR_RTL);
		wignoreonlyshowin  = gtk_check_button_new_with_label(_("Ignore \"OnlyShowIn\" Value"));
			gtk_widget_set_direction(wignoreonlyshowin, GTK_TEXT_DIR_RTL);

		GtkAdjustment *msizex_adjustment = gtk_adjustment_new(1, 0, 16384, 1, 1, 0);
			wmsizex = gtk_spin_button_new(msizex_adjustment, 1, 0);
		GtkAdjustment *msizey_adjustment = gtk_adjustment_new(1, 0, 16384, 1, 1, 0);
			wmsizey= gtk_spin_button_new(msizey_adjustment, 1, 0);
		wusecsd = gtk_check_button_new_with_label(_("Use CSD Decoration"));
			gtk_widget_set_direction(wusecsd, GTK_TEXT_DIR_RTL);
		whidetitle = gtk_check_button_new_with_label(_("Hide CSD Title"));
			gtk_widget_set_direction(whidetitle, GTK_TEXT_DIR_RTL);
		whidewindeco = gtk_check_button_new_with_label(_("Disable WM Decoration"));
			gtk_widget_set_direction(whidewindeco, GTK_TEXT_DIR_RTL);
		wexitwhenunfocused = gtk_check_button_new_with_label(_("Exit when unfocused"));
			gtk_widget_set_direction(wexitwhenunfocused, GTK_TEXT_DIR_RTL);
		wsingleinstance = gtk_check_button_new_with_label(_("Allow only a single instance"));
			gtk_widget_set_direction(wsingleinstance, GTK_TEXT_DIR_RTL);

		defbtn = gtk_button_new_with_label(_("Default"));
		okbtn = gtk_button_new_with_label(_("OK"));
		cancelbtn = gtk_button_new_with_label(_("Cancel"));
		applybtn = gtk_button_new_with_label(_("Apply"));

	GtkWidget *tab_elements = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab_elements), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_elements, gtk_label_new(_("Elements")));
		gtk_grid_attach(GTK_GRID(tab_elements), gtk_label_new(_("Search Engine")), 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_elements), webcombo, 1, 0, 1, 1);

		gtk_grid_attach(GTK_GRID(tab_elements), weblabel, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_elements), webctm, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_elements), wshowcmd, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_elements), wshowweb, 0, 4, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_elements), wshowofd, 0, 5, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_elements), wshowcalc, 0, 6, 2, 1);

	GtkWidget *tab_view = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab_view), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_view, gtk_label_new(_("View")));
		gtk_grid_attach(GTK_GRID(tab_view), gtk_label_new(_("Menu layout")), 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wmlayout, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wiconsize_label, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_view), wiconsize, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wqasize_label, 0, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_view), wqasize, 1, 2, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wshowappicons, 0, 3, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wshowqa, 0, 4, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wuseiconview, 0, 5, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wshowda, 0, 6, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_view), wentryonbottom, 0, 7, 2, 1);

	GtkWidget *tab_behavior = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab_behavior), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_behavior, gtk_label_new(_("Behavior")));
		gtk_grid_attach(GTK_GRID(tab_behavior), wcloseterm, 0, 0, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_behavior), wshowscientific, 0, 1, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_behavior), wignorenodisplay, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_behavior), wignoreterminal, 0, 3, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_behavior), wignoreonlyshowin, 0, 4, 2, 1);

	GtkWidget *tab_window = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab_window), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_window, gtk_label_new(_("Window")));
		gtk_grid_attach(GTK_GRID(tab_window), gtk_label_new(_("Window width")), 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_window), wmsizex, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_window), gtk_label_new(_("Window height")), 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(tab_window), wmsizey, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(tab_window), wusecsd, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_window), whidetitle, 0, 3, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_window), whidewindeco, 0, 4, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_window), wexitwhenunfocused, 0, 5, 2, 1);
		gtk_grid_attach(GTK_GRID(tab_window), wsingleinstance, 0, 6, 2, 1);

	gtk_box_pack_start(GTK_BOX(confbox), notebook, TRUE, TRUE, 0);

	GtkWidget *applybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		if (!sgcfg)
		{
			gtk_box_pack_end(GTK_BOX(applybox), applybtn, FALSE, FALSE, 2);
		}
		gtk_box_pack_end(GTK_BOX(applybox), cancelbtn, FALSE, FALSE, 2);
		gtk_box_pack_end(GTK_BOX(applybox), okbtn, FALSE, FALSE, 2);
		gtk_box_pack_start(GTK_BOX(applybox), defbtn, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(confbox), applybox, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(cfgdialog), confbox);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowda), showda);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowappicons), showappicons);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowqa), showqa);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wuseiconview), useiconview);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wentryonbottom), entryonbottom);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcmd), showcmd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wcloseterm), closeterm);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowweb), showweb);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowofd), showofd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcalc), showcalc);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowscientific), showscientific);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wignorenodisplay), ignorenodisplay);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wignoreterminal), ignoreterminal);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wignoreonlyshowin), ignoreonlyshowin);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wusecsd), usecsd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(whidetitle), hidetitle);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(whidewindeco), hidewindeco);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused), exitwhenunfocused);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wsingleinstance), singleinstance);

	gint length = strlen(cengine);
	if (length > 0 && cengine[length - 1] == '\n')
	{
		cengine[length - 1] = '\0';
	}

	gtk_entry_set_text(GTK_ENTRY(webctm), cengine);

	gtk_combo_box_set_active(GTK_COMBO_BOX(webcombo), wengine);
	gtk_combo_box_set_active(GTK_COMBO_BOX(wmlayout), mlayout);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(wiconsize), iconsize);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(wqasize), qasize);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(wmsizex), msizex);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(wmsizey), msizey);

	g_signal_connect(G_OBJECT(webcombo), "changed", G_CALLBACK(on_webcombo_changed), webctm);

	g_signal_connect(wusecsd, "clicked", G_CALLBACK(togglewidget), whidetitle);
	g_signal_connect(wusecsd, "clicked", G_CALLBACK(togglewidget), whidewindeco);
	g_signal_connect(wusecsd, "clicked", G_CALLBACK(togglewidget), wentryonbottom);
	g_signal_connect(wshowcalc, "clicked", G_CALLBACK(togglewidget), wshowscientific);

	g_signal_connect(wshowappicons, "clicked", G_CALLBACK(togglewidget), wiconsize_label);
	g_signal_connect(wshowappicons, "clicked", G_CALLBACK(togglewidget), wiconsize);
	g_signal_connect(wshowqa, "clicked", G_CALLBACK(togglewidget), wqasize);
	g_signal_connect(wshowqa, "clicked", G_CALLBACK(togglewidget), wqasize_label);

	g_signal_connect(defbtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(1));
	g_signal_connect(cancelbtn, "clicked", G_CALLBACK(on_cancelbutton_clicked), NULL);
	g_signal_connect(applybtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(2));
	g_signal_connect(okbtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(0));

	gtk_window_set_position(GTK_WINDOW(cfgdialog), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(cfgdialog);

	if (wengine != 3)
	{
		gtk_widget_hide(webctm);
		gtk_widget_hide(weblabel);
	}

	if (!showcalc)
	{
		gtk_widget_hide(wshowscientific);
	}

	if (!showappicons)
	{
		gtk_widget_hide(wiconsize);
		gtk_widget_hide(wiconsize_label);
	}

	if (!showqa)
	{
		gtk_widget_hide(wqasize);
		gtk_widget_hide(wqasize_label);
	}

	if (!usecsd)
	{
		gtk_widget_hide(whidetitle);
	}
	else if (usecsd)
	{
		gtk_widget_hide(wentryonbottom);
		gtk_widget_hide(whidewindeco);
	}

	g_signal_connect(cfgdialog, "destroy", G_CALLBACK(on_dialog_destroy), NULL);
	gtk_main();
}
