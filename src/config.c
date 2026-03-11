#include <config.h>


int WIDTH = 1280;
int HEIGHT = 720;
Font font = {0};
Console console = {0};
bool console_shown = false;
const char *cmd_names[CMDS_CAPACITY] = {0};
size_t cmd_names_count = 0;
Console_cmds cmds = {0};

void init_globals(void) {
	font = GetFontDefault();

	console = make_console(0, font, 24);
}
