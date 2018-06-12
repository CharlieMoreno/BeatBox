#ifndef BEATBOX_H_
#define BEATBOX_H_

#include <Arduino.h>
#include "Mean.h"
#include "Trigger.h"


#define DATA_LOG
#define ACC_ADXL345

#define TRIGGER_COMPARISON_INTERVAL 20		// Después del disparo de cada eje, se dejan 20 milisegundos para registrar posibles disparos de los otros ejes (20 pueden ser excesivos)


// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

extern unsigned long triggerComparisonTime;

extern unsigned long lastMillis;
extern unsigned long loopMillis;
extern unsigned long maxLoopMillis;

extern Trigger accX;
extern Trigger accY;
extern Trigger accZ;

extern float maxTriggerValue;
extern int maxTriggerAxis;

#endif /* BEATBOX_H_ */
