#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <gtk/gtk.h>
#include "entryloader.h"
#include "mainwindow.h"
#include "calc.h"

void on_entry_changed(GtkEntry *target, FilterData *filterdata);
gboolean on_filter_visible(GtkTreeModel *model, GtkTreeIter *iter, gpointer data);
gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean close_window_if_unfocused(gpointer widget);
gboolean on_focus_out(GtkWidget *widget, GdkEventFocus *event, gpointer user_data);

#endif
