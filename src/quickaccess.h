#ifndef QUICKACCESS_H
#define QUICKACCESS_H

#include <gtk/gtk.h>
#include "mainwindow.h"
#include "exec.h"

void load_quickaccess(void);

//static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer menu);
gint compare_filenames(gconstpointer a, gconstpointer b);
void rearrageqa(const gchar *folder_path);
void reloadqa(void);
void swapfiles(GtkWidget *input, gpointer arg);
void removeqa(GtkWidget *input, gpointer data);
void create_quickaccess(GtkWidget *pass, gpointer data);

#endif
