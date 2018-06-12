#ifndef BEATBOX_H_
#define BEATBOX_H_

#include <Arduino.h>
#include "Mean.h"
#include "Trigger.h"

#define DATA_LOG
#define ACC_ADXL345


// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

extern unsigned long lastMillis;
extern unsigned long loopMillis;
extern unsigned long maxLoopMillis;

extern Trigger accX;
extern Trigger accY;
extern Trigger accZ;

#endif /* BEATBOX_H_ */
