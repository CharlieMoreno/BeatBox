#ifndef MIDI_H_
#define MIDI_H_

#include "BeatBox.h"
//#include <SoftwareSerial.h>

#define MIDI_CHANNEL        B00001001		//primeros 4 bits (lsb)
#define MIDI_NOTE_OFF       B10000000		// últimos 4 bits (msb)
#define MIDI_NOTE_ON        B10010000		// últimos 4 bits (msb)
#define MIDI_PROG_CHANGE    B11000000		// últimos 4 bits (msb)
#define MIDI_PITCH_WHEEL    B11100000
#define MIDI_SYSEX          B11110000
#define MIDI_END_SYSEX      B11110111

extern boolean midiMode;

void midiSetup();
void sendMidiNoteOff(int note);
void sendMidiNoteOn(int note, int vel);
void sendMidiProgramChange(int prog);
void sendMidiPitchWheel(int val);
void startMidiSysEx();
void endMidiSysEx();

#endif /* MIDI_H_ */

