#include "BeatBox.h"
#include "Taps.h"
#include "Record.h"
#include "Play.h"
#include "Debug.h"
#include "Midi.h"
#include "Log.h"

boolean printingTaps;
boolean printMeanAcc;
boolean printLoopMillis;

void debugSetup() {
	printingTaps    = true;
	printMeanAcc    = false;
	printLoopMillis = false;
}

void printMillis() {
	Serial.print(F("Max Loop (ms): "));
	Serial.println(maxLoopMillis);
	lastMillis = millis();
	maxLoopMillis = 0;
}

void printState() {
	startMidiSysEx();
	Serial.print(F("\nRecording Beats   : "));
	Serial.println((recordingTaps ? F("on") : F("off")));
	Serial.print(F("Send Midi      : "));
	Serial.println((midiMode ? F("on") : F("off")));
	Serial.print(F("Print Beats    : "));
	Serial.println((printingTaps ? F("on") : F("off")));
	Serial.print(F("Print Mean Accs: "));
	Serial.println((printMeanAcc ? F("on") : F("off")));
	Serial.print(F("Print Loop Millis: "));
	Serial.println((printLoopMillis ? F("on") : F("off")));
	endMidiSysEx();
	printMillis();
}

void printTiming() {
	startMidiSysEx();
	Serial.print(F("\nbeatLength: "));
	Serial.println(getBeatLength());
	Serial.print(F("Compass: "));
	Serial.print(getCompass());
	Serial.print(F(" : "));
	Serial.println(getBeatLength()*getCompass());
	Serial.print(F("Quantization: "));
	Serial.print(getQuantization());
	Serial.print(F(" : "));
	Serial.println(getQTime());
	endMidiSysEx();
	printMillis();
}

void printTaps() {
	startMidiSysEx();
	Serial.print(F("\ntapCounter: "));
	Serial.println(getTapCounter());
	Serial.println(F("List:"));
	for(int i = 0; i < getTapCounter(); i++) {
		printTapInfo(i);
	}
	endMidiSysEx();
	printMillis();
}

void debugUpdate() {
	
	if (Serial.available() != 0) {
		char cod = Serial.read();
		switch(cod) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				setQuantization(cod - '0');
				break;
			case 't' :
				printTiming();
				break;
			case 's' :
				printState();
				break;
			case 'l' :
				printTaps();
				break;
			case 'a' :
				printLoggedAccs();
				break;
			case 'e' :
				printMillis();
				break;
			case 'd' :
				deleteAllTaps();
				break;
			case 'c' :
				recordSetup();
				break;	
			case 'P' :
				printingTaps = !printingTaps;
				break;
			case 'R' :
				recordingTaps = !recordingTaps;
				break;
			case 'A' :
				printMeanAcc = !printMeanAcc;
				break;
			case 'L' :
				printLoopMillis = !printLoopMillis;
				break;
			case 'M' :
				midiMode = !midiMode;
				break;
									
			default :
				startMidiSysEx();
				Serial.println(F("\nt - Print Timing\ns - Print Stats\nl - List Taps\na - Print Logged Accs"));
				Serial.println(F("e - Print and Erase Max loop\nd - Delete Taps\nc - Clear All\n"));
				Serial.println(F("0..9 - Sets Quantization (max half quarters)"));
				Serial.println(F("P - Print beats (on/off)\nR - Record beats (on/off)"));
				Serial.println(F("A - Print Mean Accs (on/off) \nL - Print Loop Millis (on/off)\nM - Send Midi (on/off)"));
				endMidiSysEx();
			break;
		}
	}
	
	if (printMeanAcc) printMeanAccels();
	if (printLoopMillis) printMillis();
}
