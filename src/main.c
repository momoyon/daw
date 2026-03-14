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
	size_t octave;
	float amplitude;
	bool play;
	float frames[FRAMES_CAPACITY];
	size_t frame_count;
	ma_waveform waveform;
} Userdata;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	(void)pInput;
	Userdata *ud = ((Userdata*)pDevice->pUserData);

	if (ud->play)
		ma_waveform_read_pcm_frames(&ud->waveform, pOutput, frameCount, NULL);
	memcpy(ud->frames, pOutput, sizeof(float)*frameCount);
	ud->frame_count = frameCount;
}

int main(void) {
	init_globals();

	init_window(1280, 720, 1.f, "DAW", &WIDTH, &HEIGHT);
	SetTargetFPS(60);

	const float console_h = HEIGHT * 0.5f;
	float console_y = -console_h;

	Note prev_note = C;
	Userdata ud = {
		.note = C,
		.play = false,
		.octave = 4,
		.amplitude = 2.f,
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

	ma_waveform_config wave_config = ma_waveform_config_init(
		config.playback.format,
		config.playback.channels,
		config.sampleRate,
		ma_waveform_type_sine,
		ud.amplitude,
		hertz_from_note(ud.note, 4)
	);

	if (ma_waveform_init(&wave_config, &ud.waveform) != MA_SUCCESS) {
		log_error("Failed to init waveform!");
	}

	while (!WindowShouldClose()) {
		begin_frame();
		ud.play = false;


		// NOTE: UI


		// NOTE: Input
		if (IsKeyPressed(KEY_F2)) {
			console_shown = !console_shown;
		}
		if (console_shown) {
			if (input_to_console(&console, "", 0)) {
				String_array args = get_current_console_args(&console);
				char *line = get_current_console_line_buff(&console);
				add_line_to_console_simple(&console, line, WHITE, true);


				clear_current_console_line(&console);
                if (args.count > 0) {
                    const char *cmd = args.items[0];
                    const char *restofargs = get_current_console_line_without_first_word(&console);
                    Ids matched_ids = match_command(cmd, cmd_names, cmd_names_count);

                    if (matched_ids.count == 0) {
                        log_error_console(console, "`%s` is not a valid command!", cmd);
                        clear_current_console_line(&console);
                    } else if (matched_ids.count == 1) {
                        Console_cmd console_cmd = cmds.items[matched_ids.items[0]];
                        size_t cmd_len = strlen(console_cmd.name);
                        if (cmd_len != strlen(cmd)) {
                            // Command is not fully typed
                            ASSERT(console.lines.count > 0, "BRUH");
                            Console_line *line = get_console_line(&console, 0);
                            memcpy(line->buff, console_cmd.name, cmd_len);
                            line->count = cmd_len;
                        } else {
                            console_cmd.func(args, restofargs);
                            clear_current_console_line(&console);
                        }
                    } else {
                        for (int i = 0; i < matched_ids.count; ++i) {
                            Console_cmd console_cmd = cmds.items[matched_ids.items[i]];
                            size_t cmd_len = strlen(console_cmd.name);
                            if (strlen(cmd) == cmd_len) {
                                // The input matches completely with a defined command; so execute that
                                console_cmd.func(args, restofargs);
                                clear_current_console_line(&console);
                            }
                            log_info_console(console, "%s - %s", console_cmd.name, console_cmd.desc);
                        }
                    }

                    darr_free(matched_ids);

				}
			}
		} else {
			ma_waveform_set_frequency(&ud.waveform, hertz_from_note(ud.note, ud.octave));

			if (IsKeyPressed(KEY_DOWN)) {
				if (ud.octave > 0) ud.octave--;
			}

			if (IsKeyPressed(KEY_UP)) {
				if (ud.octave < 10) ud.octave++;
			}

			if (IsKeyDown(KEY_EQUAL)) {
				ud.amplitude += GetFrameTime();
				ma_waveform_set_amplitude(&ud.waveform, ud.amplitude);
			}

			if (IsKeyDown(KEY_MINUS)) {
				ud.amplitude -= GetFrameTime();
				if (ud.amplitude <= 0.f) ud.amplitude = 0.1f;
				ma_waveform_set_amplitude(&ud.waveform, ud.amplitude);
			}

			if (IsKeyPressed(KEY_RIGHT)) {
				ma_waveform_type t = ud.waveform.config.type;
				if (t == ma_waveform_type_sawtooth)
					t = ma_waveform_type_sine;
				else
					t++;

				ma_waveform_set_type(&ud.waveform, t);
			}

			if (IsKeyPressed(KEY_LEFT)) {
				ma_waveform_type t = ud.waveform.config.type;
				if (t == ma_waveform_type_sine) 
					t = ma_waveform_type_sawtooth;
				else
					t--;

				ma_waveform_set_type(&ud.waveform, t);
			}

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
		}

		// NOTE: Update
		{
			const float target_y = console_shown ? 0.f : -console_h;
			console_y += (target_y - console_y) * GetFrameTime() * 10.f;
		}

		// NOTE: Draw

		ClearBackground(BLACK);
		Rectangle rect = {
			.x = 100.f,
			.y = 100.f,
		};
		rect.width = WIDTH-(2.f*rect.x);
		rect.height = HEIGHT-(2.f*rect.y);

		DrawRectangleLinesEx(rect, 1.f, WHITE);

		DrawLineV(v2(rect.x, rect.y + (rect.height * 0.5)), v2(rect.x + rect.width, rect.y + (rect.height * 0.5)), WHITE);

		draw_text_aligned(font, ma_waveform_type_as_str(ud.waveform.config.type), v2(rect.x + (rect.width * 0.5f), rect.y - 18-2), 18, TEXT_ALIGN_V_BOTTOM, TEXT_ALIGN_H_CENTER, WHITE);

		if (ud.play) {
			const float P = 2.f;
			draw_text(font, TextFormat("%s%d", note_as_str(ud.note), ud.octave), v2(rect.x + P, rect.y + P), 18, WHITE);

			draw_text(font, TextFormat("Frames: %d", ud.frame_count), v2(rect.x + P, rect.y + P + 18), 18, WHITE);

			for (int i = 0; i < ud.frame_count; i+=1) {
				float t = (float)i / (float)ud.frame_count;
				float frame = ud.frames[i];
				float x = rect.x + (t * rect.width);
				float y = rect.y + (rect.height * 0.5f);

				DrawLineV(v2(x, y), v2(x, y + frame * 100.f), WHITE);
			}
		}

        if (console_y > -console_h) {
            Rectangle console_rect = {
                .x = 0,
                .y = console_y,
                .width = WIDTH,
                .height = console_h,
            };
			Color col = (Color) {0x12, 0x17, 0x3d, 0xFF};
            draw_console(&console, console_rect, v2xx(2), col, WHITE, 1.f);
        }

		end_frame();
	}

	ma_waveform_uninit(&ud.waveform);
	ma_device_uninit(&device);
	close_window();
}
