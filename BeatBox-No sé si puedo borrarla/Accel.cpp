/*
 * Accel.cpp
 *
 * Created: 13/07/2012 15:36:51
 *  Author: Yago
 */ 

#include "Accel.h"
#include <util/atomic.h>

int AccelData[3];

const int AccelSign[3]   = {1, 1, 1};
const int AccelOffset[3] = {0, 0, 0};
	
AccelModes AccelState = IDDLE;						// Declaración de la variable "AccelState" de tipo enumerado "AccelModes", inicializada con el valor "IDDLE".

void setAccelState(AccelModes a) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		AccelState = a;
	}
}

AccelModes getAccelState() {
	AccelModes curr;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {	
		curr = AccelState;
		if (curr == NEW) setAccelState(IDDLE);
	}	
	return curr;
}

