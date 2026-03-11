#include <console_cmd.h>
#include <config.h>

void help_f(String_array args, const char *rest) {
	(void)args;
	(void)rest;

	for (int i = 0; i < cmds.count; ++i) {
		log_info_console(console, "%s - %s", cmds.items[i].name, cmds.items[i].desc);
	}
}

void exit_f(String_array args, const char *rest) {
	(void)args;
	(void)rest;
	exit(0);
}
