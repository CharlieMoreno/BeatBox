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
	int mean;
	int shake;
	int restL;
	int restR;
	int acumAccDiff;			// ¿QUÉ DEMONIOS ES ESTO?
	int restTimeIn;
	int gestTimeOut;
	int shakeTimeIn;
	MeanFilter lowPass;
} Trigger;

#define AFTER_TRIGGER_DELAY 50
#define TRIGGER_DIFF        50
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
  MeanFilter lowPass;

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
