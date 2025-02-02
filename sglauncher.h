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

const gchar *cweb, *cwebng, *corder, *active_text, *home_dir, *config_file_path, *terminal,
*deskenv, *quick_dirs[] = {NULL};
gchar *pm, *webengine, *cwengine, cengine[ML], *program_icon, *ccloseterm;

guint wengine, order=0, showweb=1, showcmd=1, showcalc=1, showda=1, showofd=1, useiconview=0,
	showscientific=1, exitwhenunfocused=0, sgcfg, nocsd=0, hidetitle=0, entryonbottom=0,
	callconfig=0, nohome=0, restarting=0, ismoving=0, usecsd=1, fcsd=0, iconsize=16, qasize=16,
	closeterm=1,ignorenodisplay=1, ignoreterminal=0, ignoreonlyshowin=0, hidewindeco=0,
	forceinstance=0,singleinstance=0, showqa=1, showappicons=0;

GtkWidget *window, *grid, *button, *cmd_row, *dialog, *web_row, *entry, *manswer, *mathtext,
*listbox2, *qa, *row, *headerbar, *button, *image, *wtitle, *submenu, *submenu_item_settings,
*submenu_item_quickhelp, *submenu_item_about, *submenu_item4, *submenu_item5, *weblabel, *webcombo,
*webctm, *worder, *wshowcmd, *wshowweb, *wshowcalc, *wshowscientific, *wexitwhenunfocused, *defbtn,
*applybtn, *applist, *web_box, *wshowda, *wusecsd, *whidetitle ,*scrolled_window, *math, *iconview,
*submenu_item_onlinehelp, *submenu_menu_help, *submenu_item_help, *wentryonbottom, *wiconsize,
*wuseiconview, *wcloseterm, *wignorenodisplay, *wignoreterminal, *whidewindeco, *wignoreonlyshowin,
*xdg_box, *xdg_row, *wshowofd, *wsingleinstance, *wshowqa, *wqasize, *wshowappicons;

GPtrArray *program_icon_names;
GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

GtkTreeStore *store;
GtkTreeViewColumn *column;
GtkCellRenderer *renderer;
GtkTreeModelSort *sorted_model;
GtkTreeModelFilter *filter_model;

GOptionEntry entries[] =
{
	{ "cfg", 0, 0, G_OPTION_ARG_NONE, &sgcfg, "Open SGLauncher settings", NULL },
	{ "nocsd", 0, 0, G_OPTION_ARG_NONE, &fcsd, "Disable CSD Decoration", NULL },
	{ "new-instance", 0, 0, G_OPTION_ARG_NONE, &forceinstance, "Force open program ignoring lock file", NULL },
	{ NULL }
};

#include "settings.h"
#include "calc.h"
#include "exec.h"
#include "quickaccess.h"
#include "entryloader.h"
#include "events.h"
#include "keyhandler.h"
#include "cfgmgr.h"
#include "mainwindow.h"
