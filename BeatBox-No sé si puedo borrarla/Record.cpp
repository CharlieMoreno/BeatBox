#include "BeatBox.h"
#include "Trigger.h"
#include "Taps.h"
#include "Record.h"
#include "Play.h"
#include "Lights.h"
#include "Midi.h"
#include "Debug.h"

boolean firstBeatIdentification;

void recordSetup() {
	
	deleteCompass();
	deleteAllTaps();

	for(int i = 0; i < 50; i++) {
		lightsTestSequence();
	}
	
	recordingTaps   = true;
	loopMillis    = 0;
	lastMillis    = millis();
	maxLoopMillis = 0;
	firstBeatIdentification = true;
}


void recordTaps() {

	// Configuración de beatLength Y Compás:
	if (identifying && recordingTaps) {													// Si se está en modo de identificación de tempo y compás y la grabación está habilitada:
		if (getCurrentR(&accZ) > 0) {													// si hay un golpe en el eje Z,
			if (!firstBeatIdentification) lastBeatIndex++;									// si no es el primer beat identificado, es que estamos tratando con el siguiente beat, por lo que se incrementa lastBeatIndex.
			beatMillis[lastBeatIndex] = millis();										// se captura ese instante de tiempo,
			if (compass > 0) {															// y si ya se ha registrado más de un beat, 
				beatLength = (int)(beatMillis[lastBeatIndex] - beatMillis[0])/compass;	// el duración de beat será el tiempo entre este golpe y el primero dividido entre los huecos (= nº de golpes - 1).
			}																			// Si no, el beatLength (duración de una negra) seguirá valiendo 0.
			compass++;																	// El compás pasa a tener otro beat.
			compassLenght = beatLength*compass;											// El compás dura lo que dura una negra por el número de negras hasta el momento
			nextCompass = beatMillis[lastBeatIndex] + beatLength + compassLenght;		// Como no se sabe si es el último beat, y esto se comprueba cuando ha pasado el 1 del siguiente compás y medio beatLength, ya se espera al siguiente compás. 
			//lastBeatIndex++;		Lo he cambiado de sitio, más arriba.
			sendMidiNoteOn(noteMapping[7], 64);
			lightsFlash(250);
			firstBeatIdentification = false;											// Si se ha ejecutado este if una vez, el siguiente beat ya no será el primero.

		}
		if (lastBeatIndex > 1) {															// Si ya se han registrado más de dos beats,
			if ( abs(millis() - beatMillis[lastBeatIndex]) > (int)(1.5*beatLength) ) {		// y pasa un tiempo mayor a una duración de beat y media, se sale del modo de identificación.
				identifying = false;
				nextBeat   = nextBeat + beatLength;											// Análogo a nextCompass.
                                setQuantization(getQuantization());
			}
		}
	}
	
	//Grabación de Taps:
	else {
		if (getCurrentR(&accX) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 0;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];		// Por ahora cada instrumento es una nota, pero igual más adelante cada instrumento puede tocar varias notas.
			taps[tapCounter].velocity = getCurrentR(&accX);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
		if (getCurrentL(&accX) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 1;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].velocity = getCurrentL(&accX);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
		if (getCurrentR(&accY) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 2;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].velocity = getCurrentR(&accY);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
		if (getCurrentL(&accY) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 3;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].velocity = getCurrentL(&accY);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
		if (getCurrentR(&accZ) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 4;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].velocity = getCurrentR(&accZ);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
		if (getCurrentL(&accZ) > 0) {
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].instrument = 5;
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].velocity = getCurrentL(&accZ);
			if (printingTaps) printTapInfo(tapCounter);
			sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);
			if (recordingTaps) tapCounter++;
		}
	}
}
