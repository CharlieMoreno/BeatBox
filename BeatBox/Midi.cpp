#include "Midi.h"

#define MIDI_CHANNEL        B00001001		//primeros 4 bits (lsb)
#define MIDI_NOTE_OFF       B10000000		// utimos 4 bits	(msb)
#define MIDI_NOTE_ON        B10010000
#define MIDI_PROG_CHANGE    B11000000
#define MIDI_PITCH_WHEEL    B11100000
#define MIDI_SYSEX          B11110000
#define MIDI_END_SYSEX      B11110111

boolean midiMode = true;

void midiSetup() {
	midiMode = true;
}

void sendMidiNoteOff(int note) {
	if (!midiMode) return;
	note = constrain(note, 0, 127);
	int vel = 0;
	Serial.write(MIDI_NOTE_OFF | MIDI_CHANNEL);		
	Serial.write(note);
	Serial.write(vel);
}

void sendMidiNoteOn(int note, int vel) {
	if (!midiMode) return;
	vel = constrain(vel, 0, 127);
	note = constrain(note, 0, 127);
	Serial.write(MIDI_NOTE_ON | MIDI_CHANNEL);
	Serial.write(note);
	Serial.write(vel);
}

void sendMidiProgramChange(int prog) {
	if (!midiMode) return;
	prog = constrain(prog, 0, 127);
	Serial.write(MIDI_PROG_CHANGE | MIDI_CHANNEL);
	Serial.write(prog);
}

void sendMidiPitchWheel(int val) {
	if (!midiMode) return;
	val = constrain(val, -8192, 8191);
	val = val + 8192;
	int vall = val & 0x7F;
	int valh = (val >> 7) & 0x7F;
	Serial.write(MIDI_PITCH_WHEEL | MIDI_CHANNEL);
	Serial.write(vall);
	Serial.write(valh);
}

void startMidiSysEx() {
    Serial.write((char)MIDI_SYSEX);					// TENGO CURIOSIDAD POR SABER CÓMO VA ESTO.
}

void endMidiSysEx() {
    Serial.write((char)MIDI_END_SYSEX);
}	
