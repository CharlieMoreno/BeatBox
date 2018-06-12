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

unsigned char quantization = 0;		// Quantization divider (1 full compass, 2 half compass, 4 quarters, 8 eighths,etc....)
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
	//YAGO: if (quantization) qTime = beatLength / quantization;	// No me cuadra que se divida el beatLenth entre el quantization divider, si cuando este vale 1 se refiere al compás entero. Voy a probar con compassLengh:
	if (quantization) qTime = compassLength / quantization;
	for(int i = 0; i < 6; i++) qSent[i] = false;
}

unsigned char getQuantization() {
	return quantization;
}

int getQTime() {
	return qTime;
}

void playbackTaps() {

	// DEFINICIÓN DE compassTime y METRÓNOMO
	if (identifying) return;
	
	unsigned long m = millis();
	
	if (m >= nextCompass) {											// Si la barra temporal ha llegado a un final/comienzo de compás o lo ha sobrepasado,
		nextCompass = nextCompass + compassLength;					// se recalcula el instante del siguiente final/comienzo de compás,
		nextBeat   = nextBeat + beatLength;							// se recalcula el instante del siguiente beat,
		sendMidiNoteOn(noteMapping[6], 100);						// suena la nota aguda del metrónomo,
		lightsPulse(4, beatLength >> 2);							// se ilumina la cara 4
		for(int i = 0; i < tapCounter; i++) taps[i].sent = false;	// y se establece que ningún beat ha sido reproducido en ese compás.
	}
	else {													// Si la barra temporal no ha llegado al final del compás,
		if (m >= nextBeat) {								// pero ha sobrepasado el instante del siguiente beat,
			nextBeat = nextBeat + beatLength;				// se recalcula el instante del siguiente beat,
			sendMidiNoteOn(noteMapping[7], 64);				// suena la nota grave del metrónomo,
			lightsPulse(4, beatLength >> 5);				// y se ilumina la cara 4.
		}
		if (quantization) {									// Si la cuantización está activada,
			if (m > nextCompass - (qTime >> 1)) {			// y si la barra temporal ha sobrepasado el instante 'final_de_compás_menos_qTime_medios',
				for(int i = 0; i < 6; i++) qSent[i] = 0;	// las banderas de reproducciones cuantizadas se ponen a cero.
			}
		}
	}
	compassTime = m - nextCompass + compassLength;		// = compassLenth - (nextCompass - m), es el instante en el que se encuentra la barra temporal referido al compás.

	// DISPARO Y LOOP:
	if (quantization) {																		// Si la cuantización está activada:
		int  currentRest = ((compassTime + (qTime >> 1)) % qTime) - (qTime >> 1);			// Operación matemática que devuelve un valor negativo si se está entre [qTime medios antes del instante de disparo, instante de disparo) y un valor positivo si se está entre [instante de disparo, qTime medios después del instante de disparo)
		if (currentRest < 0) {																// Si no se ha llegado al instante de disparo, 
			beatTriggered = false;															// se indica que no se ha disparado nada de ese instante
			for(int i = 0; i < 6; i++) qSent[i] = 0;										// y que ningun intrumento ha sido disparado aún.
		} else {																																// Si ya se ha alcanzado o sobrepasado el instante de disparo
			if (!beatTriggered) {																												// y todavía no se ha efectuado el disparo de los intrumentos, 
				for (int i = 0; i < tapCounter; i++) {																							// Para todos los taps:
					if (!qSent[taps[i].instrument]) {																							// Una vez se ha disparado un tap con un intrumento i, ya no se disparan más taps con ese mismo intrumento en este periodo de cuantización. Digamos que la cuantización unifica los diversos golpes de un mismo instrumento en un mismo periodo de cuantización.
						if (taps[i].timeInCompass >= compassTime - (qTime >> 1) && taps[i].timeInCompass < compassTime + (qTime >> 1) || taps[i].timeInCompass - compassLength >= compassTime - (qTime >> 1)) {		// y si el tap se encuentra dentro del intervalo +-qTime/2 alrededor del cursor temporal (compassTime), incluídos en el 'or' los taps que están al final del compás y que entrarían en el -qTime/2 cuando compassTime vuelve a empezar desde cero,
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




