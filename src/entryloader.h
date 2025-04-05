#ifndef ENTRYLOADER_H
#define ENTRYLOADER_H

#include <gtk/gtk.h>

typedef struct
{
	gchar *name;
	gchar *icon;
	gchar *path;
	gchar *toexec;
}AppDetails;

typedef struct
{
	GtkEntry *entry;
	GtkTreeModelFilter *filter;
	gchar *filter_text;
	GtkTreeView *treeview;
	GtkIconView *iconview;
} FilterData;

extern FilterData filter_data;

//included here to avoid problems with FilterData definition
#include "settings.h"
#include "quickaccess.h"

extern GtkTreeStore *store;
extern GtkTreeViewColumn *column;
extern GtkCellRenderer *renderer;
extern GtkTreeModelSort *sorted_model;
extern GtkTreeModelFilter *filter_model;

gint gtk_tree_iter_compare_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data);
void load_apps(GtkTreeView *treeview, GtkIconView *iconview);

#endif
