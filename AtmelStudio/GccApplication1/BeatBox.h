#ifndef BEATBOX_H_
#define BEATBOX_H_

#include <Arduino.h>
#include "Mean.h"
#include "Trigger.h"
#include <SoftwareSerial.h>


#define BLUETOOTH	1
#define PUERTOSERIE	1


#define DATA_LOG
#define ACC_ADXL345

#define TRIGGER_COMPARISON_INTERVAL 20		// Después del disparo de cada eje, se dejan 20 milisegundos para registrar posibles disparos de los otros ejes (20 pueden ser excesivos)


// #define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

// Creación del puerto Bluetooth:
extern SoftwareSerial BT;	// RX | TX		(No todo los pines de la MEGA pueden funcionar como RX)

extern unsigned long triggerComparisonTime;

extern unsigned long lastMillis;
extern unsigned long loopMillis;
extern unsigned long maxLoopMillis;

extern Trigger accX;
extern Trigger accY;
extern Trigger accZ;

extern int maxTriggerValue;
extern int maxTriggerAxis;

#endif /* BEATBOX_H_ */
