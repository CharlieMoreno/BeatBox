#include "Trigger.h"
#include "BeatBox.h"

unsigned long  eTime;
char  enabled;

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
	eTime   = 0;
	enabled = true;
	initMeanFilter(&(t->lowPass), MAX_TAPS);
}


void updateTrigger(Trigger * t, int y) {
	t->yCurrent = y;
	
	t->mean = meanFilterIO(&(t->lowPass), t->yCurrent);
	
	t->yPrevThresholdL = t->yThresholdL;
	t->yPrevThresholdR = t->yThresholdR;
	
	// Delay despues de cada disparo
	if (millis() > eTime) {
		enabled = true;
	}

	// Detección de trigger positivo
	if (t->yCurrent - t->yPrev < -TRIGGER_DIFF && t->yCurrent < t->mean - (TRIGGER_DIFF << 1) && enabled) {
		t->yThresholdR = abs(t->yCurrent - t->mean);
		enabled     = false;
		eTime       = millis() + AFTER_TRIGGER_DELAY;
	} else {
		t->yThresholdR = 0;
	}

	// Detección de trigger negativo
	if (t->yCurrent - t->yPrev >  TRIGGER_DIFF && t->yCurrent > t->mean + (TRIGGER_DIFF << 1) && enabled) {
		t->yThresholdL = abs(t->yCurrent - t->mean);
		enabled     = false;
		eTime       = millis() + AFTER_TRIGGER_DELAY;
	} else {
		t->yThresholdL = 0;
	}
	
	// Detección de agitacion y reposo hacia abajo
	t->acumAccDiff = (t->acumAccDiff >> 1) + abs(t->yCurrent - t->mean);
	if (t->acumAccDiff > TRIGGER_ERASE) {
		if (t->shakeTimeIn == 0) {
			if (enabled) {
				t->shake    = t->acumAccDiff;
				enabled     = false;
				eTime       = millis() + 10 * AFTER_TRIGGER_DELAY;
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

