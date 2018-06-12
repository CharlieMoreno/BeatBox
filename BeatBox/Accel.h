/*
 * Accel.h
 *
 * Created: 13/07/2012 15:37:16
 *  Author: Yago
 */ 


#ifndef ACCEL_H_
#define ACCEL_H_

#include "BeatBox.h"
#include "Wire.h"


#define AccX	0
#define AccY	1
#define AccZ	2


enum AccelModes {IDDLE, INIT, READING, NEW, ERROR};		// Declaración del enumerated type "AccelModes", que puede valer uno de esos cinco strings.

extern int AccelData[3];
extern AccelModes AccelState;							// Declaración de la variable "AccelState" del tipo enumerado "AccelModes"
extern const int AccelSign[3];
extern const int AccelOffset[3];

AccelModes	getAccelState();
void		setAccelState(AccelModes a);

//////////////////  Definitions for adlx345
#ifdef ACC_ADXL345
	void adxl345_initAccel();
	void adxl345_updateAccel();
	#define initAccel()		{ I2C_Init(); adxl345_initAccel(); }
	#define updateAccel		adxl345_updateAccel
#endif

//////////////////  Definitions for adlx330
#ifdef ACC_ADXL330
	void adxl330_updateAccel();
	#define initAccel()		
	#define updateAccel		adxl330_updateAccel
#endif

#define I2C_Init()	Wire.begin()
#define getAccelX() AccelData[AccX]
#define getAccelY() AccelData[AccY]
#define getAccelZ() AccelData[AccZ]


#endif /* ACCEL_H_ */