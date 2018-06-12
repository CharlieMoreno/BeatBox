
//Esta biblioteca de funciones realmente solo sirve para ver cuándo y en qué condiciones hay triggers.

#include "Log.h"		

int dataX[LOG_LENGTH];
int dataY[LOG_LENGTH];
int dataZ[LOG_LENGTH];

int recSamples;			// Contador. Cuando se produce un trigger, pasa a valer 32 y va decreciendo.
int preTrigPtr;			// Posición inicial del tramo de 16 posiciones en las que se están guardando valores cíclicamente (excepto durante el guardado de los 32 valores posteriores a un trigger).
int trigPtr;			// Posición inicial del tramo de 32 posiciones en las que se guardan valores tras un trigger. Es igual a la posición de preTrigPtr + 16.
int idx;				// "index", contador de 0 a 15 excepto tras un trigger, que cuenta de 0 a 31.    
int recPreTrigPtr;		// Foto de la posición de PreTrigPtr en el último trigger.
int recPreTrigIdx;		// Foto de idx cuando se produce un trigger.
int lastPtr;			// Guarda la última posición de ptr antes de salir de logValues, pero no se utiliza para nada.

void _logSetup() {
	recSamples    = 0;		
	preTrigPtr    = 0;		
	trigPtr       = preTrigPtr + (LOG_LENGTH >> 2);		
	idx           = 0;
	recPreTrigPtr = preTrigPtr;		
	recPreTrigIdx = 0;				
	lastPtr       = 0;				
	for (int i = 0; i < LOG_LENGTH; i++) {
		dataX[i] = 0;
		dataY[i] = 0;
		dataZ[i] = 0;
	}
}

// PLANTEAMIENTO: Para cada eje hay un "buffer". Se van guardando valores (del acelerómetro) cíclicamente en las 16 primeras posiciones, indicadas por ptr.
// Cuando se produce un trigger, ptr salta a la posición siguiente a ese bloque de 16 y va indicando las 32 posiciones siguientes para que se guarden 32 valores.
// Después, ptr pasa al siguiente bloque de 16 posiciones, en el que se siguen guardando valores cíclicamente a la espera del siguiente trigger.
void _logValues(int valX, int valY, int valZ) {		// MOTIVO DEL GUIÓN BAJO? --> Para activar o desactivar su uso mediante el ifdef que está en Log.h
	int ptr;	
	if (getCurrentR(&accX)>0 || getCurrentL(&accX)>0 || getCurrentR(&accY)>0 || getCurrentL(&accY)>0 || getCurrentR(&accZ)>0 || getCurrentL(&accZ)>0) {
		recSamples = (LOG_LENGTH >> 1);  // 1/2 of LOG_LENGTH
		trigPtr = (preTrigPtr + (LOG_LENGTH >> 2)) & LOG_PTR_MASK; // End of preTrigger area
		recPreTrigPtr = preTrigPtr;
		recPreTrigIdx = idx;
		idx = 0;
	}
	
	if (recSamples != 0) {
		ptr = (trigPtr + idx++) & LOG_PTR_MASK;  // ptr = (trigPtr + idx++) % LOG_LENGTH
		if (--recSamples == 0) {
			preTrigPtr = (ptr + 1) & LOG_PTR_MASK;
			idx = 0;
		}
	} else {
		ptr = (preTrigPtr + idx) & LOG_PTR_MASK; // ptr = (preTrigPtr + idx++) % LOG_LENGTH
		idx = (idx + 1) & (LOG_PTR_MASK >> 2);       // idx = idx % LOG_LENGTH/4
	}
	
	dataX[ptr] = valX;
	dataY[ptr] = valY;
	dataZ[ptr] = valZ;
	
	lastPtr = ptr;		
	
}


void _printMeanAccels() {
	Serial.print(dataX[lastPtr]);
	Serial.print('\t');
	Serial.print(dataY[lastPtr]);
	Serial.print('\t');
	Serial.println(dataZ[lastPtr]);
}

void _printLoggedAccs() {
	int ptr;
	Serial.print(F("pre trigger ptr: "));
	Serial.println(preTrigPtr);
	Serial.print(F("trigger ptr: "));
	Serial.println(trigPtr);
	Serial.print(F("pre trigger idx: "));
	Serial.println(idx);
	Serial.print(F("rec pre trigger ptr: "));
	Serial.println(recPreTrigPtr);
	Serial.print(F("rec pre trigger idx: "));
	Serial.println(recPreTrigIdx);
	Serial.println(F("\tidx\tptr\tAccX\tAccY\tAccZ"));
	for(int i = 0; i < (LOG_LENGTH >> 2); i++) {
		ptr = (recPreTrigPtr + ((recPreTrigIdx + i) & (LOG_PTR_MASK >> 2)) ) & LOG_PTR_MASK;
		Serial.print('\t');
		Serial.print(i);
		Serial.print('\t');
		Serial.print(ptr);
		Serial.print('\t');
		Serial.print(dataX[ptr]);
		Serial.print('\t');
		Serial.print(dataY[ptr]);
		Serial.print('\t');
		Serial.println(dataZ[ptr]);
	}
	for(int i = 0; i < (LOG_LENGTH >> 1) ; i++) {
		ptr = (trigPtr + i) & LOG_PTR_MASK;
		Serial.print('\t');
		Serial.print(i + (LOG_LENGTH >> 2));
		Serial.print('\t');
		Serial.print(ptr);
		Serial.print('\t');
		Serial.print(dataX[ptr]);
		Serial.print('\t');
		Serial.print(dataY[ptr]);
		Serial.print('\t');
		Serial.println(dataZ[ptr]);
	}
}
