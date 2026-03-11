#include <config.h>

#define ENGINE_IMPLEMENTATION
#include <engine.h>

#define COMMONLIB_REMOVE_PREFIX
#define COMMONLIB_IMPLEMENTATION
#include <commonlib.h>

int main(void) {
	init_window(1280, 720, 1.f, "DAW", &WIDTH, &HEIGHT);


	while (!WindowShouldClose()) {
		begin_frame();

		end_frame();
	}

	close_window();
}
