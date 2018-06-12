#include "Trigger.h"
#include "BeatBox.h"
#include "Lights.h"
#include "Midi.h"

unsigned long  gTime;
//char  enabled;

/*Trigger * createTrigger() {								// Función que devuelve un puntero a variable de tipo Trigger (definido en Trigger.h)
	Trigger * t = (Trigger *)malloc(sizeof(Trigger));		// malloc reserva espacio de memoria del tamaño del tipo Trigger y genera un puntero, que es "casted" a tipo Trigger y guardado en el puntero t (que apunta a un tipo Trigger).
	initTrigger(t);
	return t;
}*/ //No se utiliza esta función. Se declaran las estructuras Trigger en BeatBox de forma estática.

void initTrigger(Trigger * t) {
	t->yPrev           = 0;
	t->yCurrent        = 0;
	t->yThresholdL     = 0;
	t->yPrevThresholdL = 0;
	t->yThresholdR     = 0;
	t->yPrevThresholdR = 0;
	t->acumAccDiff     = 0;
	t->shake           = 0;
	t->shakeTimeIn     = GESTURE_TIME_IN;
	t->restR           = 0;
	t->restL           = 0;
	t->restTimeIn      = GESTURE_TIME_IN;
	t->gestTimeOut     = GESTURE_TIME_IN;
	t->eTime   = 0;
	t->enabled = true;
	initMeanFilter(&(t->filter), MAX_TAPS);
}


void updateTrigger(Trigger * t, int y) {		// El argumento 'y' va a ser la lectura del acelerómetro en raw, y 't' va a ser donde se guarden los diversos valores calculados, entre los cuales destaca 'yThresholdR' e 'yThresholdL'.
	t->yCurrent = y >> 6;	// Divido entre 2^6=256 porque el MPU6050 tiene 6 bits más de resolución (16384 LSB/g) que el ADXL345 (256 LSB/g) para un rango de +-2g.
	
	t->mean = meanFilterIO(&(t->filter), t->yCurrent);		//Se introduce el nuevo valor en el filtro, se elimina el último, y se calcula la media.
	
	t->yPrevThresholdL = t->yThresholdL;
	t->yPrevThresholdR = t->yThresholdR;
	
	// Delay despues de cada disparo
	if (millis() > t->eTime) {
		t->enabled = true;
	}

	// Detección de trigger positivo
	if (t->yCurrent - t->yPrev < -TRIGGER_DIFF && t->yCurrent < t->mean - (TRIGGER_DIFF << 1) && t->enabled) {		//Si la aceleración actual es 50 unidades menor que la anterior, si es menor que la media menos 100 unidades y si está habilitado el disparo:
		t->yThresholdR = abs(t->yCurrent - t->mean);			// Entonces hay disparo, que se representa con un valor mayor que 0 para yThresholdR.
		t->enabled     = false;
		t->eTime       = millis() + AFTER_TRIGGER_DELAY;					// No se permiten más triggers del mismo eje hasta pasado AFTER_TRIGGER_DELAY.
		triggerComparisonTime = millis() + TRIGGER_COMPARISON_INTERVAL;		// Se establece el instante de tiempo en el que se compararán los triggers registrados.
	} else {
		t->yThresholdR = 0;
	}

	// Detección de trigger negativo
	if (t->yCurrent - t->yPrev >  TRIGGER_DIFF && t->yCurrent > t->mean + (TRIGGER_DIFF << 1) && t->enabled) {
		t->yThresholdL = abs(t->yCurrent - t->mean);
		t->enabled     = false;
		t->eTime       = millis() + AFTER_TRIGGER_DELAY;
		triggerComparisonTime = millis() + TRIGGER_COMPARISON_INTERVAL;
	} else {
		t->yThresholdL = 0;
	}
	
	// Detección de agitacion y reposo hacia abajo
	t->acumAccDiff = (t->acumAccDiff >> 1) + abs(t->yCurrent - t->mean);
	if (t->acumAccDiff > TRIGGER_ERASE) {
		if (t->shakeTimeIn == 0) {
			if (t->enabled) {
				t->shake    = t->acumAccDiff;
				t->enabled     = false;
				t->eTime       = millis() + 10 * AFTER_TRIGGER_DELAY;
				t->restL = 0;
				t->restR = 0;
			} else {
				t->shake = 0;
			}			
		} else {
			t->shakeTimeIn--;
		}
		t->restTimeIn = GESTURE_TIME_IN;
		t->gestTimeOut = GESTURE_TIME_IN;
	} else {
		// Si hay reposo hacia L
		if (t->acumAccDiff < 25 && t->mean < 450) {
			if (t->restTimeIn == 0) {
				t->restL = 1;
				t->restR = 0;
				t->shake = 0;
			} else {
				t->restTimeIn--;
			}
			t->shakeTimeIn = GESTURE_TIME_IN;
			t->gestTimeOut = GESTURE_TIME_IN;
		} else {
			// Si hay reposo hacia R
			if (t->acumAccDiff < 25 && t->mean > 550) {
				if (t->restTimeIn == 0) {
					t->restR = 1;
					t->restL = 0;
					t->shake = 0;
				} else {
					t->restTimeIn--;
				}
				t->shakeTimeIn = GESTURE_TIME_IN;
				t->gestTimeOut = GESTURE_TIME_IN;
			} else {
				t->restTimeIn  = GESTURE_TIME_IN;
				t->shakeTimeIn = GESTURE_TIME_IN;
				if (t->gestTimeOut == 0) {
					t->restL = 0;
					t->restR = 0;
					t->shake = 0;
				} else {
					t->gestTimeOut--;
				}				
			}
		}
	}
	
	
	t->yPrev = t->yCurrent;
}

int getCurrentR(Trigger * t) {
	return t->yThresholdR;
}

int getPreviousR(Trigger * t) {
	return t->yPrevThresholdR;
}

int getCurrentL(Trigger * t) {
	return t->yThresholdL;
}

int getPreviousL(Trigger * t) {
	return t->yPrevThresholdL;
}

int getShake(Trigger * t) {
	return t->shake;
}

int getRestL(Trigger * t) {
	return t->restL;
}

int getRestR(Trigger * t) {
	return t->restR;
}

// Devuelve el eje que ha sufrido un mayor pico de aceleración:
void getRealTrigger(Trigger * tX, Trigger * tY, Trigger * tZ, int *values, int *maxValue, int *maxAxis, int *counter) {			// Cuando se llama a la función en BeatBox.cpp, el puntero p es el vector axisTriggerValues.
	
	// triggerComparisonTime vale 0 la mayoría del tiempo. Cuando se produce un trigger, pasa a valer millis()+TRIGGER_COMPARISON_INTERVAL (en updateTrigger). Esto abre la ventana de registro de disparos.
			
	// Si está abierta la ventana de tiempo, se registran los disparos:
	if (millis() < triggerComparisonTime) {		
		/*
		if (tX->yThresholdL > 0) v[0] = tX->yThresholdL;
		if (tX->yThresholdR > 0) v[1] = tX->yThresholdR;
		if (tY->yThresholdL > 0) v[2] = tY->yThresholdL;
		if (tY->yThresholdR > 0) v[3] = tY->yThresholdR;
		if (tZ->yThresholdL > 0) v[4] = tZ->yThresholdL;
		if (tZ->yThresholdR > 0) v[5] = tZ->yThresholdR;
		*/
		if (getCurrentL(tX) > 0 && getCurrentL(tX) > *(values+0)) *(values+0) = getCurrentL(tX);
		if (getCurrentR(tX) > 0 && getCurrentR(tX) > *(values+1)) *(values+1) = getCurrentR(tX);
		if (getCurrentL(tY) > 0 && getCurrentL(tY) > *(values+2)) *(values+2) = getCurrentL(tY);
		if (getCurrentR(tY) > 0 && getCurrentR(tY) > *(values+3)) *(values+3) = getCurrentR(tY);
		if (getCurrentL(tZ) > 0 && getCurrentL(tZ) > *(values+4)) *(values+4) = getCurrentL(tZ);
		if (getCurrentR(tZ) > 0 && getCurrentR(tZ) > *(values+5)) *(values+5) = getCurrentR(tZ);
		
		//Alguno tiene que ser mayor que cero, si no, triggerComparisonTime seguiría valiendo 0 y no se habría entrado en este if.
	}
	
	// Comparación de valores para saber cuál es el mayor en valor absoluto. AVISO: NO SE CONTEMPLAN CASOS DE IGUALDAD DE VALORES!!!.
	if (millis() > triggerComparisonTime && triggerComparisonTime > 0) {
		
		float val = *(values+0);
		int axisNum = 0;
		//char axis[3] = "No";		// Esto pa qué??? Si no se usa para nada... SE BORRA 
			
		for (int i=1; i<6; i++) {		// Comparación de valores en busca del máximo:
			if (val < *(values+i)) {
				val = *(values+i);
				axisNum = i;
			}
		}
		
		triggerComparisonTime = 0; //Inhabilito la comparación de valores.
		
		*maxValue = val;		// Dejo grabado el máximo valor en la variable global para que se pueda asignar a la velocidad en recordTaps().
		
		*maxAxis = axisNum;
		
		sendMidiNoteOn(axisNum, int(*maxValue));			// Se reproduce la nota
		lightsPulse(axisNum, int(*maxValue) >> 5);			// y se ilumina la cara correspondiente.
		
		
		
		// ONLY FOR DEBUGGING PURPOSES...	
			//(*counter)++;
		//
			//if (axisNum == 0) { Serial.print(*counter); Serial.print("\t X+ \t"); Serial.println(*maxValue);}
			//if (axisNum == 1) { Serial.print(*counter); Serial.print("\t X- \t"); Serial.println(*maxValue);}
			//if (axisNum == 2) { Serial.print(*counter); Serial.print("\t Y+ \t"); Serial.println(*maxValue);}
			//if (axisNum == 3) { Serial.print(*counter); Serial.print("\t Y- \t"); Serial.println(*maxValue);}
			//if (axisNum == 4) { Serial.print(*counter); Serial.print("\t Z+ \t"); Serial.println(*maxValue);}
			//if (axisNum == 5) { Serial.print(*counter); Serial.print("\t Z- \t"); Serial.println(*maxValue);}
		//
			//Serial.println();
		//
			//for (int i=0; i<6; i++) { Serial.print("\t\t"); Serial.println(*(values+i)); }
			//Serial.println();
		// TILL HERE.
		
		// Reinicializo los axisTriggerValues:
		for (int i=0; i<6; i++) { *(values+i) = 0; }
			
		//return axisNum;			// Se devuelve el eje (y sentido) del mayor impacto.
		return;
	}
	
	*maxAxis = -1;
	//return -1;
	return;
}