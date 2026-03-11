#include <config.h>


int WIDTH = 1280;
int HEIGHT = 720;
Font font = {0};

void init_globals(void) {
	font = GetFontDefault();
}
