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

	for(int i = 0; i < 2; i++) {
		lightsTestSequence();
	}
	
	recordingTaps = true;
	firstBeatIdentification = true;
	loopMillis    = 0;
	lastMillis    = millis();
	maxLoopMillis = 0;
	
}


void recordTaps() {

	// Configuración de beatLength Y Compás
	if (identifying && recordingTaps) {													// Si se está en modo de identificación de tempo y compás y la grabación está habilitada:
		//if (getCurrentR(&accZ) > 0) {													// si hay un golpe en el eje Z,
		if (maxTriggerAxis != -1) {
			if (!firstBeatIdentification) lastBeatIndex++;								// si no es el primer beat identificado, es que estamos tratando con el siguiente beat, por lo que se incrementa lastBeatIndex (lastBeatIndex se pone a 0 en el setup).
			beatMillis[lastBeatIndex] = millis();										// se captura ese instante de tiempo,
			if (compass > 0) {															// y si ya se ha registrado más de un beat, 
				beatLength = (int)(beatMillis[lastBeatIndex] - beatMillis[0]) / compass;	// la duración de beat será el tiempo entre este golpe y el primero dividido entre los huecos (= nº de golpes - 1).
			}																			// Si no, el beatLength (duración de una negra) seguirá valiendo 0.
			compass++;																	// El compás pasa a tener otro beat.
			compassLength = beatLength*compass;											// El compás dura lo que dura una negra por el número de negras hasta el momento
			nextCompass = beatMillis[lastBeatIndex] + beatLength + compassLength;		// Como no se sabe si es el último beat, y esto se comprueba cuando ha pasado el 1 del siguiente compás y medio beatLength, ya se espera al siguiente compás. 
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
				// DEBUG:
				Serial.println("Compas definido!");
				//Serial.println("\n beatLength = " + String(beatLength));
				//Serial.println("compass = " + String(compass));
				//Serial.println("compass length = " + String(compassLength));
				//for (int i = 0; i<lastBeatIndex+1; i++) Serial.println("beatMillis[" + String(i) + "] = " + String(beatMillis[i]));
				// END DEBUG
			}
		}
	}
	//Grabación de Taps: 
	else {
		if (maxTriggerAxis != -1){
			lightsPulse(maxTriggerAxis, 50);
			taps[tapCounter].instrument = maxTriggerAxis;																																																																																																																																																					
			taps[tapCounter].note = noteMapping[taps[tapCounter].instrument];
			taps[tapCounter].timeInCompass = compassTime;
			taps[tapCounter].velocity = map(maxTriggerValue, 200, 500, 0, 127);
			if (printingTaps) printTapInfo(tapCounter);
			//sendMidiNoteOn(taps[tapCounter].note, taps[tapCounter].velocity);		// Lo he puesto en getRealTrigger()
			if (recordingTaps) tapCounter++;
			
			maxTriggerAxis = -1;	// Para que no vuelva a grabarse el mismo tap como uno nuevo.
		}		
	}
}

void detectShakes(){			//detección de shakes para borrado y reinicio:
	
		
		if ((getShake(&accX) || getShake(&accY)) && getRestR(&accZ)) {
			lightsPulse(4, 500);
			if (getTapsNumForInstrument(4) == 0) recordSetup();				// Si se hace un shake de una cara en la que no hay ningún tap o ya se han borrado todos, se reinicializa el loop. Podría ocurrir con todas las caras, no?
			deleteTapsWithInstrument(4);
		}
		if ((getShake(&accX) || getShake(&accY)) && getRestL(&accZ)) {
			lightsPulse(5, 500);
			deleteTapsWithInstrument(5);
		}
		if ((getShake(&accX) || getShake(&accZ)) && getRestR(&accY)) {
			lightsPulse(2, 500);
			deleteTapsWithInstrument(2);
		}
		if ((getShake(&accX) || getShake(&accZ)) && getRestL(&accY)) {
			lightsPulse(3, 500);
			deleteTapsWithInstrument(3);
		}

		if ((getShake(&accY) || getShake(&accZ)) && getRestR(&accX)) {
			lightsPulse(0, 500);
			deleteTapsWithInstrument(0);
		}
		if ((getShake(&accY) || getShake(&accZ)) && getRestL(&accX)) {
			lightsPulse(1, 500);
			deleteTapsWithInstrument(1);
		}
	
}