#ifndef SETTINGS_H
#define SETTINGS_H

#include "sglauncher.h"
#include "cfgmgr.h"

#define ML 256
extern const gchar *cweb;
extern const gchar *cwebng;
extern const gchar *corder;
extern const gchar *active_text;
extern const gchar *home_dir;
extern const gchar *config_file_path;
extern const gchar *quick_dirs[];
extern gchar *pm;
extern gchar *webengine;
extern gchar *cwengine;
extern gchar cengine[ML];
extern gchar *program_icon;
extern gchar *ccloseterm;
extern guint wengine;
extern guint mlayout;
extern guint showweb;
extern guint showcmd;
extern guint showcalc;
extern guint showda;
extern guint showofd;
extern guint useiconview;
extern guint showscientific;
extern guint exitwhenunfocused;
extern guint nocsd;
extern guint hidetitle;
extern guint entryonbottom;
extern guint callconfig;
extern guint nohome;
extern guint usecsd;
extern guint iconsize;
extern guint qasize;
extern guint closeterm;
extern guint ignorenodisplay;
extern guint ignoreterminal;
extern guint ignoreonlyshowin;
extern guint hidewindeco;
extern guint singleinstance;
extern guint showqa;
extern guint showappicons;
extern guint msizex;
extern guint msizey;
extern guint usecustomcss;

void updateconf(GtkButton *widget, gpointer user_data);
void readconf(void);

#endif
