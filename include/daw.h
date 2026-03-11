#ifndef DAW_H_
#define DAW_H_

#include <stddef.h>
#include <miniaudio.h>

typedef enum Note Note;

enum Note {
	C = 0,
	CSHARP,
	D,
	DSHARP,
	E,
	F,
	FSHARP,
	G,
	GSHARP,
	A,
	ASHARP,
	B,
	NOTE_COUNT,
};

const char *note_as_str(const Note note);

float hertz_from_note(const Note note, size_t octave);


// Miniaudio Helpers
const char *ma_waveform_type_as_str(const ma_waveform_type t);

#endif // DAW_H_

/////////////////////////////////////////////////////////////////////////////////////
#ifdef DAW_IMPLEMENTATION
#include <math.h>

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

const char *note_as_str(const Note note) {
	switch (note) {
		case C: return "C";
		case CSHARP: return "C#";
		case D: return "D";
		case DSHARP: return "D#";
		case E: return "E";
		case F: return "F";
		case FSHARP: return "F#";
		case G: return "G";
		case GSHARP: return "G#";
		case A: return "A";
		case ASHARP: return "A#";
		case B: return "B";
		case NOTE_COUNT:
		default: ASSERT(false, "UNREACHABLE!");
	}
}

float hertz_from_note(const Note note, size_t octave) {
    if (octave < 0 || note < 0 || note >= NOTE_COUNT) {
		log_error("Invalid Note!");
        return -1;
    }
    return 16.35f * powf(2.f, (note + 12 * octave) / 12.f);
}


// Miniaudio Helpers
const char *ma_waveform_type_as_str(const ma_waveform_type t) {
	switch (t) {
		case ma_waveform_type_sine: return "Sine";
		case ma_waveform_type_square: return "Square";
		case ma_waveform_type_triangle: return "Triangle";
		case ma_waveform_type_sawtooth: return "Sawtooth";
	}
}
#endif
