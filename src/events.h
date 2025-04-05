#ifndef EVENTS_H
#define EVENTS_H

#include "sglauncher.h"

void exit_window(GtkWidget *window);
void create_window(void);
gchar* probe_icons_from_theme(GPtrArray *icon_names);
void window_set_icon(GtkWindow *target, gchar *iconname);
gboolean applist_show_menu(GtkWidget *widget, GdkEventButton *event, gpointer user_data);

#endif
