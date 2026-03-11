#include <config.h>
#define DAW_IMPLEMENTATION
#include <daw.h>

#include <miniaudio.h>

#define ENGINE_IMPLEMENTATION
#include <engine.h>

#define COMMONLIB_REMOVE_PREFIX
#define COMMONLIB_IMPLEMENTATION
#include <commonlib.h>

#define FRAMES_CAPACITY 1024
typedef struct {
	Note note;
	bool play;
	float frames[FRAMES_CAPACITY];
	size_t frame_count;
} Userdata;


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	(void)pInput;
	Userdata *ud = ((Userdata*)pDevice->pUserData);

	ma_waveform_config config = ma_waveform_config_init(
		ma_format_f32,
		1,
		41000,
		ma_waveform_type_sawtooth,
		0.5f,
		hertz_from_note(ud->note, 4)
	);
	ma_waveform waveform;
	if (ma_waveform_init(&config, &waveform) != MA_SUCCESS) {
		log_error("Failed to init waveform!");
	} else {
		if (ud->play)
		ma_waveform_read_pcm_frames(&waveform, pOutput, frameCount, NULL);
	}

	ma_waveform_uninit(&waveform);

	memcpy(ud->frames, pOutput, sizeof(float)*frameCount);
	ud->frame_count = frameCount;
}

int main(void) {
	init_window(1280, 720, 1.f, "DAW", &WIDTH, &HEIGHT);

	Userdata ud = {
		.note = C,
		.play = false,
	};

	ma_device_config config  = ma_device_config_init(ma_device_type_playback);
	config.playback.format   = ma_format_f32;
	config.playback.channels = 1;
	config.sampleRate        = 41000;
	config.dataCallback      = data_callback;
	config.pUserData         = &ud;

	ma_device device;
	if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
		log_error("Failed to init ma_device!");
		return 1;
	}

	ma_device_start(&device);

	while (!WindowShouldClose()) {
		begin_frame();
		ud.play = false;


		if (IsKeyDown(KEY_Z)) {
			ud.play = true;
			ud.note = C;
		}
		if (IsKeyDown(KEY_S)) {
			ud.play = true;
			ud.note = CSHARP;
		}
		if (IsKeyDown(KEY_X)) {
			ud.play = true;
			ud.note = D;
		}
		if (IsKeyDown(KEY_D)) {
			ud.play = true;
			ud.note = DSHARP;
		}
		if (IsKeyDown(KEY_C)) {
			ud.play = true;
			ud.note = E;
		}
		if (IsKeyDown(KEY_V)) {
			ud.play = true;
			ud.note = F;
		}
		if (IsKeyDown(KEY_G)) {
			ud.play = true;
			ud.note = FSHARP;
		}
		if (IsKeyDown(KEY_B)) {
			ud.play = true;
			ud.note = G;
		}
		if (IsKeyDown(KEY_H)) {
			ud.play = true;
			ud.note = GSHARP;
		}
		if (IsKeyDown(KEY_N)) {
			ud.play = true;
			ud.note = A;
		}
		if (IsKeyDown(KEY_J)) {
			ud.play = true;
			ud.note = ASHARP;
		}
		if (IsKeyDown(KEY_M)) {
			ud.play = true;
			ud.note = B;
		}


		ClearBackground(BLACK);
		Rectangle rect = {
			.x = 100.f,
			.y = 100.f,
		};
		rect.width = WIDTH-(2.f*rect.x);
		rect.height = HEIGHT-(2.f*rect.y);

		DrawRectangleLinesEx(rect, 1.f, WHITE);


		DrawLineV(v2(rect.x, rect.y + (rect.height * 0.5)), v2(rect.x + rect.width, rect.y + (rect.height * 0.5)), WHITE);

		if (ud.play) {
			const float P = 2.f;
			draw_text(GetFontDefault(), note_as_str(ud.note), v2(rect.x + P, rect.y + P), 18, WHITE);

			draw_text(GetFontDefault(), TextFormat("Frames: %d", ud.frame_count), v2(rect.x + P, rect.y + P + 18), 18, WHITE);

			for (int i = 0; i < ud.frame_count; i+=1) {
				float t = (float)i / (float)ud.frame_count;
				float frame = ud.frames[i];
				float x = rect.x + (t * rect.width);
				float y = rect.y + (rect.height * 0.5f);

				DrawLineV(v2(x, y), v2(x, y + frame * 100.f), WHITE);
			}
		}

		end_frame();
	}

	ma_device_uninit(&device);
	close_window();
}
