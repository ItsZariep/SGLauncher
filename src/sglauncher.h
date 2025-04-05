#ifndef SGLAUNCHER_H
#define SGLAUNCHER_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)
#define GETTEXT_PACKAGE "sglauncher"
#define localedir "/usr/share/locale"

#define ML 256

extern GPtrArray *program_icon_names;
extern GtkIconTheme *theme;
extern GtkIconInfo *info;
extern GdkPixbuf *icon;

extern guint forceinstance;
extern guint sgcfg;
extern guint fcsd;
extern const gchar *terminal;
extern const gchar *deskenv;
extern guint restarting;
extern guint ismoving;
extern guint disableunfocus;

#include "settings.h"
#include "mainwindow.h"
#endif
