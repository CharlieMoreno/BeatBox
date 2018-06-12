/*
 * Accal_ADXL345.cpp
 *
 * Created: 16/07/2012 15:00:38
 *  Author: Yago
 */ 

#include "Accel.h"

#define AccelAddress 0x53

void adxl345_initAccel() {
	setAccelState(INIT);
	Wire.beginTransmission(AccelAddress);
	Wire.write(0x2D);  // power register
	Wire.write(0x08);  // measurement mode
	Wire.endTransmission();
	delay(5);
	Wire.beginTransmission(AccelAddress);
	Wire.write(0x31);  // Data format register
	Wire.write(0x08);  // set to full resolution
	Wire.endTransmission();
	delay(5);
	// Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwidth)
	Wire.beginTransmission(AccelAddress);
	Wire.write(0x2C);  // Rate
	Wire.write(0x0B);  // set to 200Hz, normal operation
	Wire.endTransmission();
	delay(5);
	for(int i = 0; i < 3; i++) AccelData[i] = 0;
	setAccelState(IDDLE);
}

// Reads x,y and z accelerometer registers
void adxl345_updateAccel() {
	
	if (AccelState == IDDLE || AccelState == NEW) {
		setAccelState(READING);
	} else {
		setAccelState(ERROR);
		return;
	}
	
	int i = 0;
	byte buff[6];
	
	sei();		// interruption
	
	Wire.beginTransmission(AccelAddress);
	Wire.write(0x32);        //sends address to read from
	Wire.endTransmission(); //end transmission
	
	Wire.beginTransmission(AccelAddress); //start transmission to device
	Wire.requestFrom(AccelAddress, 6);    // request 6 bytes from device. Cada pareja de bytes es la lectura de un eje.
	
	// ((Wire.available())&&(i<6))
	while(Wire.available()) {
		buff[i] = Wire.read();  // receive one byte
		i++;
	}
	
	Wire.endTransmission(); //end transmission
	
	// All bytes received?
	int AccData;
	if (i==6) {														// Si se han recibido los 6 bytes,
		AccData = (((int)buff[1]) << 8) | buff[0];					// se guardan en AccData el primer y segundo byte, uno tras otro (lectura de un eje)
		AccelData[0] = AccelSign[0]*(AccData-AccelOffset[0]);		// y se guarda en AccelData[0] tras restarle el offset ponerle el signo apropiado.
		AccData = (((int)buff[3]) << 8) | buff[2];
		AccelData[1] = AccelSign[1]*(AccData-AccelOffset[1]);
		AccData = (((int)buff[5]) << 8) | buff[4];
		AccelData[2] = AccelSign[2]*(AccData-AccelOffset[2]);
	}
	else {
		setAccelState(ERROR);
		return;
	}
	
	if (AccelState == READING) {
		setAccelState(NEW);
	}
	
}