#ifndef TRIGGER_H_
#define TRIGGER_H_

#include "Mean.h"

typedef struct  {
	int yPrev;
	int yCurrent;
	int yThresholdL;
	int yPrevThresholdL;
	int yThresholdR;
	int yPrevThresholdR;
	int shake;
	int restL;
	int restR;
	int acumAccDiff;			// ¿QUÉ DEMONIOS ES ESTO?
	int restTimeIn;
	int gestTimeOut;
	int shakeTimeIn;
	MeanFilter filter;			// En filter se van a ir guardando los últimos 32 valores con la función meanFilterIO(). El puntero que contiene se mueve de forma circular.
	int mean;					// En mean se guarda el resultado de la media de los valores guardados en filter, calculada por la función meanFilterIO().
	char enabled;
	unsigned long eTime;
} Trigger;



#define AFTER_TRIGGER_DELAY 50		//Tiempo, tras un disparo, en el que no se puede volver a producir otro disparo. Puede que 50 sea demasiado.
#define TRIGGER_DIFF        100		//Mínima diferencia entre dos valores consecutivos de aceleración para cumplir una de las dos condiciones de disparo
#define TRIGGER_ERASE       40
#define GESTURE_TIME_IN		100	// in loop units

/*Trigger * createTrigger();*/
void      initTrigger(Trigger * t);
void      updateTrigger(Trigger * t, int y);
int	  getCurrentR(Trigger * t);
int	  getPreviousR(Trigger * t);
int	  getCurrentL(Trigger * t);
int	  getPreviousL(Trigger * t);
int	  getShake(Trigger * t);
int   getRestR(Trigger * t);
int   getRestL(Trigger * t);
void getRealTrigger(Trigger * tX, Trigger * tY, Trigger * tZ, int *values, int *maxValue, int *maxAxis, int *counter);

/*
class TriggerClass {
private:
  float yPrev;
  float yCurrent;
  float yThresholdL;
  float yPrevThresholdL;
  float yThresholdR;
  float yPrevThresholdR;
  float mean;
  long  eTime;
  char  enabled;
  MeanFilter filter;

public:
  TriggerClass();
  ~TriggerClass();

  void    initTrigger();
  void    updateTrigger(int y);
  float	  getCurrentR();
  float	  getPreviousR();
  float	  getCurrentL();
  float	  getPreviousL();  
  
};
*/

#endif /* TRIGGER_H_ */
