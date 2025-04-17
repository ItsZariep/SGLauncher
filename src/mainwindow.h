#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <gtk/gtk.h>

#include "sglauncher.h"
#include "exec.h"
#include "entryloader.h"
#include "keyhandler.h"
#include "events.h"
#include "cfgmgr.h"

extern GtkWidget *window;
extern GtkWidget *iconview;
extern GtkWidget *listbox2;
extern GtkWidget *qa;
extern GtkWidget *qas;
extern GtkWidget *manswer;
extern GtkWidget *mathtext;
extern GtkWidget *math;
extern GtkWidget *submenu;
extern GtkWidget *dialog;
extern GtkWidget *applist;
extern GtkWidget *cmd_row;
extern GtkWidget *xdg_row;
extern GtkWidget *web_row;
extern GtkWidget *entry;
extern GtkWidget *submenu_item_settings;

void on_submenu_item_about_selected(GtkMenuItem *menuitem, gpointer userdata);
void on_submenu_item_onlinehelp_selected(GtkMenuItem *menuitem, gpointer userdata);
void create_window(void);
#endif
