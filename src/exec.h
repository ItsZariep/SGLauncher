#ifndef EXEC_H
#define EXEC_H

#include <gtk/gtk.h>
#include "events.h"
#include "mainwindow.h"

void run_command(GtkWidget *widget, gpointer data);
void on_item_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *input);

#endif
