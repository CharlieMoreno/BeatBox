#include "Midi.h"
#include "BeatBox.h"

#define MIDI_CHANNEL        B00001001		//primeros 4 bits (lsb)
#define MIDI_NOTE_OFF       B10000000		// últimos 4 bits (msb)
#define MIDI_NOTE_ON        B10010000		// últimos 4 bits (msb)
#define MIDI_PROG_CHANGE    B11000000		// últimos 4 bits (msb)
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
	#if PUERTOSERIE
	Serial.write(MIDI_NOTE_OFF | MIDI_CHANNEL);		
	Serial.write(note);
	Serial.write(vel);
	#endif
	#if BLUETOOTH
	BT.write(MIDI_NOTE_OFF | MIDI_CHANNEL);
	BT.write(note);
	BT.write(vel);
	#endif
	
}

void sendMidiNoteOn(int note, int vel) {
	if (!midiMode) return;
	vel = constrain(vel, 0, 127);
	note = constrain(note, 0, 127);
	#if PUERTOSERIE
	Serial.write(MIDI_NOTE_ON | MIDI_CHANNEL);
	Serial.write(note);
	Serial.write(vel);
	#endif
	#if BLUETOOTH
	BT.write(MIDI_NOTE_ON | MIDI_CHANNEL);
	BT.write(note);
	BT.write(vel);
	#endif
	//DEBUG:
	//Serial.print("\t");
	//Serial.print(MIDI_NOTE_ON | MIDI_CHANNEL, BIN);
	//Serial.print("\t");
	//Serial.print(note, BIN); 
	//Serial.print("\t");
	//Serial.println(vel, BIN);
}

void sendMidiProgramChange(int prog) {
	if (!midiMode) return;
	prog = constrain(prog, 0, 127);
	#if PUERTOSERIE
	Serial.write(MIDI_PROG_CHANGE | MIDI_CHANNEL);
	Serial.write(prog);
	#endif
	#if BLUETOOTH
	BT.write(MIDI_PROG_CHANGE | MIDI_CHANNEL);
	BT.write(prog);
	#endif
}

//void sendMidiPitchWheel(int val) {
	//if (!midiMode) return;
	//val = constrain(val, -8192, 8191);
	//val = val + 8192;
	//int vall = val & 0x7F;
	//int valh = (val >> 7) & 0x7F;
	//#if PUERTOSERIE
	//Serial.write(MIDI_PITCH_WHEEL | MIDI_CHANNEL);
	//Serial.write(vall);
	//Serial.write(valh);
	//#endif
	//#if BLUETOOTH
	//BT.write(MIDI_PITCH_WHEEL | MIDI_CHANNEL);
	//BT.write(vall);
	//BT.write(valh);
	//#endif
//}

void startMidiSysEx() {
	#if PUERTOSERIE
    Serial.write((char)MIDI_SYSEX);					// TENGO CURIOSIDAD POR SABER CÓMO VA ESTO.
	#endif
	#if BLUETOOTH
	BT.write((char)MIDI_SYSEX);
	#endif
}

void endMidiSysEx() {
	#if PUERTOSERIE
    Serial.write((char)MIDI_END_SYSEX);
	#endif
	#if BLUETOOTH
	BT.write((char)MIDI_END_SYSEX);
	#endif
}	
