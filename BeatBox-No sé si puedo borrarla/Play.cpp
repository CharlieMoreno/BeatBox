/*
* Play.cpp
*
* Created: 18/06/2012 12:01:23
*  Author: Yago
*/

#include "Taps.h"
#include "Record.h"
#include "Lights.h"
#include "Play.h"
#include "Midi.h"

unsigned char quantization;		// Quantization divider (1 full compass, 2 half compass, 4 quarters, etc....)
int  qTime;						// Quantization time in ms
unsigned char qSent[6];			// Boolean value. True if instrument i has been already triggered for current quantization period
unsigned char beatTriggered;    // Boolean value. True if current quantization period has already been triggered

void setupPlayBack() {
	setQuantization(0);
}

void setQuantization(unsigned char q) {
	quantization  = q;
	qTime         = 0;
	beatTriggered = false;
	if (quantization) qTime = beatLength / quantization;
	for(int i = 0; i < 6; i++) qSent[i] = false;
}

unsigned char getQuantization() {
	return quantization;
}

int getQTime() {
	return qTime;
}

void playbackTaps() {

	// DEFINICIÓN DE compassTime
	if (identifying) return;
	
	unsigned long m = millis();
	
	if (m >= nextCompass) {
		nextCompass = nextCompass + compassLenght;
		nextBeat   = nextBeat + beatLength;
		sendMidiNoteOn(noteMapping[6], 100);
		lightsPulse(4, beatLength >> 2);
		for(int i = 0; i < tapCounter; i++) taps[i].sent = false;
	}
	else {
		if (m >= nextBeat) {
			nextBeat = nextBeat + beatLength;
			sendMidiNoteOn(noteMapping[7], 64);
			lightsPulse(4, beatLength >> 5);
		}
		if (quantization) {
			if (m > nextCompass - (qTime >> 1)) {
				for(int i = 0; i < 6; i++) qSent[i] = 0;
			}
		}
	}
	compassTime = m - nextCompass + compassLenght;		// = compassLenth - (nextCompass - m)

	// DISPARO Y LOOP:
	if (quantization) {
		int  currentRest = ((compassTime + (qTime >> 1)) % qTime) - (qTime >> 1);			// Operación matemática que devuelve un valor negativo si se está entre [qTime medios antes del instante de disparo, instante de disparo) y un valor positivo si se está entre [instante de disparo, qTime medios después del instante de disparo)
		if (currentRest < 0) {																// Si no se ha llegado al instante de disparo, 
			beatTriggered = false;															// se indica que no se ha disparado nada de ese instante
			for(int i = 0; i < 6; i++) qSent[i] = 0;										// y que ningun intrumento ha sido disparado aún.
		} else {																																// Si ya se ha alcanzado o sobrepasado el instante de disparo
			if (!beatTriggered) {																												// y todavía no se ha efectuado el disparo de los intrumentos, 
				for (int i = 0; i < tapCounter; i++) {																							// Para todos los taps:
					if (!qSent[taps[i].instrument]) {																							// Una vez se ha disparado un tap con un intrumento i, ya no se disparan más taps con ese mismo intrumento en este periodo de cuantización. Digamos que la cuantización unifica los diversos golpes de un mismo instrumento en un mismo periodo de cuantización.
						if (taps[i].timeInCompass >= compassTime - (qTime >> 1) && taps[i].timeInCompass < compassTime + (qTime >> 1) ) {		// y si el tap se encuentra dentro del intervalo +-qTime/2 alrededor del cursor temporal (compassTime), 
							sendMidiNoteOn(taps[i].note, taps[i].velocity);																		// se reproduce,
							lightsPulse(taps[i].instrument, taps[i].velocity >> 5);																// se ilumina la cara correspodiente
							qSent[taps[i].instrument] = true;																					// y se indica como ya disparado.
							taps[i].sent = true;																								// Esta indicación es redundante, pero bueno, no hace daño.
						}
					} else {
						taps[i].sent = true; // Redundante pero no hace daño.
					}
				}
				beatTriggered = true;
			}
		}
	} else {																			// Sin cuantización:
		for (int i = 0; i < tapCounter; i++) {											// Para todos los taps:
			if (abs(taps[i].timeInCompass - compassTime) < 10) {						// si el tap se encuentra en un intervalo de +- 10ms alrededor del cursor temporal		
				if (!taps[i].sent) {													// y aún no ha sido reproducido,
					sendMidiNoteOn(taps[i].note, taps[i].velocity);						// se reproduce,
					lightsPulse(taps[i].instrument, taps[i].velocity >> 5);				// se ilumina la cara correspondiente
					taps[i].sent = true;												// y se indica como ya reproducido.
				}
			} else {																	// Si el tap no se encuentra dentro de ese intervalo
				taps[i].sent = false;													// se indica como no reproducido.
			}
		}
	}
}




