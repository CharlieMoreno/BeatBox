/*
* Taps.cpp
*
* (fka Events.cpp)
* Created: 19/06/2012 15:04:34
* Author: Yago
*/

#include "Taps.h"

Tap     taps[60];				// Las notas del patrón rítmico, cada una con su instante en el compás, su intrumento (cara del cubo), su nota (necesario?), su velocidad y un bandera de "enviado".
int     tapCounter;			// Número de Taps registrados hasta el momento. ***SUGERENCIA DE LLAMARLO tapCounter*** howManyTaps
int     whichTapsNow[32];		// Los índices de los taps que hay en el instante en el que se llama a la función que rellena este vector.				***NO SE UTILIZA***
int     compass;				// Número de negras por compás.
int     compassLength;			// Duración del compás en ms.
int     compassTime;			// Instante en el que se encuentra el programa referido al inicio del compás.
int     beatLength;				// Duración de una negra en ms (el inverso del tempo). ANTES LLAMADO "tempo".
long    beatMillis[32];			// Instantes de tiempo absolutos de los beats del metrónomo. Se pueden registrar hasta 32 (sobrao).
int     lastBeatIndex;			// Índice de los beats del metrónomo (fka "events").
unsigned long    nextCompass;	// Instante absoluto del siguiente compás (referido al inicio del programa).
unsigned long    nextBeat;		// Instante absoluto de la siguiente negra (referido al inicio del programa). ANTES LLAMADO "nextBlack".
boolean recordingTaps;			// Modo de overdubbing. Se puede salir de este modo.
boolean identifying;			// Modo de identificación del tempo y del compás.

unsigned char noteMapping[INSTRUMENTS] = { 60, 62, 64, 65, 67, 69, 71, 72 };
	
void deleteCompass() {		
	for(int i = 0; i < 32; i++) {
		beatMillis[i] = 0;
		whichTapsNow[i] = 0;
	}

	identifying    = true;		
	compass        = 0;
	beatLength     = 0;
	lastBeatIndex  = 0;
}

void deleteAllTaps() {
	tapCounter = 0;
	for(int i = 0; i < 60; i++) {
		taps[i].timeInCompass = 0;
		taps[i].instrument    = 0;
		taps[i].note          = 0;
		taps[i].velocity      = 0;
		taps[i].sent          = 0;
	}
}

void deleteTapsWithInstrument(int instrument) {		// Borra los taps que usen el instrumento nº i, es decir, los taps de la cara 'i' del cubo.
	for(int j = tapCounter - 1; j >= 0; j--) {		// Para ello, se recorren todos los taps registrados hasta el momento.
		if (taps[j].instrument == instrument) {		// Si un tap j utiliza el instrumento i, 
			if (j != tapCounter - 1) {				// y si no es el último tap registrado,
				taps[j] = taps[tapCounter - 1];	// se sobreescribe con el último tap (porque da igual el orden en el array taps[])
			}
			tapCounter--;							// y se decrementa el número de taps. Si el tap j fuera el último, no se sobreescribe ni se borra, solo se decrementa el nº de taps.
		}
	}
}

/*
int getCurrentBeatsNumber(int time) {			// NO SE UTILIZA (CREO)
	int res = 0;
	for(int i = 0; i < tapCounter; i++) {
		if (taps[i].timeInCompass == time) {
			whichTapsNow[res] = i;
			res++;
		}
	}
	return res;
}

Tap * getCurrentBeat(int i) {					// NO SE UTILIZA (CREO)
	return &(taps[whichTapsNow[i]]);
}

char getInstrumentAtCurrentBeat(int i) {		// NO SE UTILIZA (CREO)
	return taps[whichTapsNow[i]].instrument;
}

char getVelocityAtCurrentBeat(int i) {			// NO SE UTILIZA (CREO)
	return taps[whichTapsNow[i]].velocity;
}
*/

int getBeatLength() {
	return beatLength;
}
	
int getCompass() {
	return compass;
}

int getTapCounter() {
	return tapCounter;
}

int getTapTime(int i) {
	return taps[i].timeInCompass;
}

int getTapIntrument(int i) {
	return taps[i].instrument;
}

int getTapNote(int i) {
	return taps[i].note;
}

int getTapVelocity(int i) {
	return taps[i].velocity;
}

int getTapsNumForInstrument(int instrument) {			// Número de taps que usan ese intrumento. Cada cara del cubo es un intrumento.
	int res = 0;		// "resultado"
	for(int j = 0; j < tapCounter; j++) {
		if (taps[j].instrument == instrument) {
			res++;
		}
	}
	return res;
}

void printTapInfo(int tapIndex) {
	//Serial.print("TapIndex: ");
	Serial.print(tapIndex);
	Serial.print(' ');
	//Serial.print(" | Time: ");
	Serial.print(getTapTime(tapIndex));
	Serial.print(' ');
	//Serial.print(" | Instrument: ");
	Serial.print(getTapIntrument(tapIndex));
	Serial.print(' ');
	//Serial.print(" | Note: ");
	Serial.print(getTapNote(tapIndex));
	Serial.print(' ');
	//Serial.print(" | Velocity: ");
	Serial.println(getTapVelocity(tapIndex));
}
