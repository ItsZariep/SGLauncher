#include "sglauncher.h"

guint forceinstance = 0;
guint sgcfg = 0;
guint fcsd = 0;
const gchar *terminal;
const gchar *deskenv;

GOptionEntry entries[] =
{
	{ "cfg", 0, 0, G_OPTION_ARG_NONE, &sgcfg, "Open SGLauncher settings", NULL },
	{ "nocsd", 0, 0, G_OPTION_ARG_NONE, &fcsd, "Disable CSD Decoration", NULL },
	{ "new-instance", 0, 0, G_OPTION_ARG_NONE, &forceinstance, "Force open program ignoring lock file", NULL },
	{ NULL }
};

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

	if (singleinstance)
	{
		if (forceinstance)
		{}
		else
		{
			const gchar *lock_file_path = "/tmp/sglauncher.lock";
			GFile *lock_file = g_file_new_for_path(lock_file_path);
			GError *error = NULL;

			if (g_file_query_exists(lock_file, NULL))
			{
				g_error("Program already running, if not, use --new-instance to avoid self lock\n");
				g_object_unref(lock_file);
				return 0;
			}

			if (!g_file_create(lock_file, G_FILE_CREATE_NONE, NULL, &error))
			{
				g_warning("Error creating lock file: %s\n", error->message);
				g_error_free(error);
				g_object_unref(lock_file);
			}
		}
	}


	gtk_init(&argc, &argv);

	create_window();
}
