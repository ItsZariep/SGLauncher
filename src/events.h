#ifndef EVENTS_H
#define EVENTS_H

#include <sys/stat.h>

#include "sglauncher.h"
#include "mainwindow.h"
#include "settings.h"

void exit_window(GtkWidget *window);
void create_window(void);
gchar* probe_icons_from_theme(GPtrArray *icon_names);
void window_set_icon(GtkWindow *target, gchar *iconname);
gboolean applist_show_menu(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean match_func(GtkEntryCompletion *completion, const gchar *key_string, GtkTreeIter *iter, gpointer user_data);
gboolean is_symlink(const gchar *path);
gboolean on_completion_match_selected(GtkEntryCompletion *completion, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data);
GList *get_executables_from_path(void);

#endif
