#include "sglauncher.h"
int main(int argc, char *argv[]) 
{
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, localedir);
	textdomain(GETTEXT_PACKAGE);

	gchar* env_sgcsd = getenv("SGCSD");
	terminal = getenv("TERMINAL");
	deskenv = getenv("XDG_CURRENT_DESKTOP");
	nocsd = (env_sgcsd != NULL) ? atoi(env_sgcsd) == 0 : 0;
	fcsd = (env_sgcsd != NULL) ? 1 : 0;

	GOptionContext *context = g_option_context_new("");
	g_option_context_add_main_entries(context, entries, NULL);
	g_option_context_parse(context, &argc, &argv, NULL);
	nocsd = fcsd;

	readconf();
	gtk_init(&argc, &argv);

	create_window();
}
