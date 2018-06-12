#ifndef MIDI_H_
#define MIDI_H_

#include "BeatBox.h"

extern boolean midiMode;

void midiSetup();
void sendMidiNoteOff(int note);
void sendMidiNoteOn(int note, int vel);
void sendMidiProgramChange(int prog);
void sendMidiPitchWheel(int val);
void startMidiSysEx();
void endMidiSysEx();

#endif /* MIDI_H_ */

