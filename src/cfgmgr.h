#ifndef CFGMGR_H
#define CFGMGR_H

#include "sglauncher.h"
#include "settings.h"

#define XA 0.0f
#define XM 4

extern GtkWidget *cfgdialog;
extern GtkWidget *weblabel;
extern GtkWidget *webcombo;
extern GtkWidget *webctm;
extern GtkWidget *wmlayout;
extern GtkWidget *wshowcmd;
extern GtkWidget *wshowweb;
extern GtkWidget *wshowcalc;
extern GtkWidget *wshowscientific;
extern GtkWidget *wexitwhenunfocused;
extern GtkWidget *defbtn;
extern GtkWidget *applybtn;
extern GtkWidget *wshowda;
extern GtkWidget *wusecsd;
extern GtkWidget *whidetitle;
extern GtkWidget *wentryonbottom;
extern GtkWidget *wiconsize;
extern GtkWidget *wuseiconview;
extern GtkWidget *wcloseterm;
extern GtkWidget *wignorenodisplay;
extern GtkWidget *wignoreterminal;
extern GtkWidget *whidewindeco;
extern GtkWidget *wignoreonlyshowin;
extern GtkWidget *wshowofd;
extern GtkWidget *wsingleinstance;
extern GtkWidget *wshowqa;
extern GtkWidget *wqasize;
extern GtkWidget *wshowappicons;
extern GtkWidget *wmsizex;
extern GtkWidget *wmsizey;
extern GtkWidget *wusecustomcss;

extern guint callconfig;

void on_webcombo_changed(GtkComboBox *widget, gpointer user_data);
void on_dialog_destroy(GtkWidget *widget, gpointer data);
void togglewidget(GtkWidget *input, gpointer data);
void showcfg(void);

#endif
