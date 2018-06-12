/*
* GccApplication1.c
*
* Created: 06/06/2012 18:59:34
*  Author: Yago
*/

#include "BeatBox.h"
#include "Accel.h"
#include "Trigger.h"
#include "Lights.h"
#include "Taps.h"
#include "Record.h"
#include "Midi.h"
#include "Play.h"
#include "Log.h"
#include "Debug.h"
#include "I2Cdev.h"
#include "MPU6050.h"
//#include "Wire.h"		//NECESARIO?



// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

Trigger accX;
Trigger accY;
Trigger accZ;

void setup() {

	initTrigger(&accX);
	initTrigger(&accY);
	initTrigger(&accZ);

	//lightsSetup();
	//debugSetup();
	
	//logSetup();

	//MPU6050setup();		// No me compila si pongo este setup en una función. No lo entiendo.
	//initAccel();			// Esto era para el acelerómetro que usaba Yago.
	
	// join I2C bus (I2Cdev library doesn't do this automatically)
	//Wire.begin();
	
	// initialize serial communication
	// (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
	// it's really up to you depending on your project)
	Serial.begin(38400);
	
	// initialize device
	while(true)Serial.println("Initializing I2C devices...");
	accelgyro.initialize();
	
	// verify connection
	Serial.println("Testing device connections...");
	Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
	
	

	for(int i = 0; i < 50; i++) {
		/*
		int x = analogRead(7);
		int y = analogRead(6);
		int z = analogRead(5);
		*/
		
		/*						// Esto era para el acelerómetro que usaba Yago.
		updateAccel();			
		
		int x = getAccelX();
		int y = getAccelY();
		int z = getAccelZ();
		*/
		
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		
		updateTrigger(&accX, ax);
		updateTrigger(&accY, ay);
		updateTrigger(&accZ, az);
	}

	recordSetup();

	attachPeriodicInterrupt(updateAccel, 4); // Update Accel ~250 times per second
	
	Serial.begin(57600);
}


char strBuff[64];
void plotAcc(int a) {
	for(int i = 0; i < 63; i++) {
		strBuff[i] = ' ';
		if (i == (a >> 5)) strBuff[i] = 'x';
	}
	strBuff[63] = 0;
	Serial.println(strBuff);
}

unsigned long lastMillis = 0;
unsigned long loopMillis = 0;
unsigned long maxLoopMillis = 0;

int i = 0;
void process() {
	
	/*
	int x;
	int y;
	int z;
	*/
	
	/*
	int x = analogRead(7);
	int y = analogRead(6);
	int z = analogRead(5);
	*/
	/*
	if (getAccelState() == NEW) {
		i++;
		x = getAccelX();
		y = getAccelY();
		z = getAccelZ();
	} else {
		return;
	}
	*/
	/*	
	Serial.print(x);
	Serial.print('\t');
	Serial.println(y);
	*/
	
	// plotAcc(x);
	
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	updateTrigger(&accX, ax);
	updateTrigger(&accY, ay);
	updateTrigger(&accZ, az);
	
	boolean t = false;
	if (getCurrentR(&accX) > 0) {
		lightsPulse(0, 50);
		t = true;
	}
	if (getCurrentL(&accX) > 0) {
		lightsPulse(1, 50);
		t = true;
	}
	if (getCurrentR(&accY) > 0) {
		lightsPulse(2, 50);
		t = true;
	}
	if (getCurrentL(&accY) > 0) {
		lightsPulse(3, 50);
		t = true;
	}
	if (getCurrentR(&accZ) > 0) {
		lightsPulse(4, 50);
		t = true; 
	}
	if (getCurrentL(&accZ) > 0) {
		lightsPulse(5, 50);
		t = true;
	}

	logValues(ax, ay, az, t);

	if ((getShake(&accX) || getShake(&accY)) && getRestR(&accZ)) {
		lightsPulse(4, 500);
		if (getTapsNumForInstrument(4) == 0) recordSetup();				// Si se hace un shake de una cara en la que no hay ningún tap o ya se han borrado todos, se reinicializa el loop. Podría ocurrir con todas las caras, no?
		deleteTapsWithInstrument(4);
	}
	if ((getShake(&accX) || getShake(&accY)) && getRestL(&accZ)) {
		lightsPulse(5, 500);
		deleteTapsWithInstrument(5);
	}
	if ((getShake(&accX) || getShake(&accZ)) && getRestR(&accY)) {
		lightsPulse(2, 500);
		deleteTapsWithInstrument(2);
	}
	if ((getShake(&accX) || getShake(&accZ)) && getRestL(&accY)) {
		lightsPulse(3, 500);
		deleteTapsWithInstrument(3);
	}

	if ((getShake(&accY) || getShake(&accZ)) && getRestR(&accX)) {
		lightsPulse(0, 500);
		deleteTapsWithInstrument(0);
	}
	if ((getShake(&accY) || getShake(&accZ)) && getRestL(&accX)) {
		lightsPulse(1, 500);
		deleteTapsWithInstrument(1);
	}
	
	recordTaps();

	playbackTaps();
  
	lightsUpdate();

	loopMillis = millis() - lastMillis;
	maxLoopMillis = max(maxLoopMillis, loopMillis);
	lastMillis = millis();

}

long int timeIn;
void loop() {
	i = 0;

	// while(millis() < (unsigned int)(timeIn + 1000)) {
		process();
		debugUpdate();
	// }

	/*
	if (getAccelState() == ERROR) Serial.println(F("Accel Reading Error"));

	Serial.println(i);
	i = 0;
	timeIn = millis();
	*/
}
/*
void MPU6050setup() {
	// join I2C bus (I2Cdev library doesn't do this automatically)
	Wire.begin();	
	
	// initialize serial communication
	// (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
	// it's really up to you depending on your project)
	Serial.begin(38400);
	
	// initialize device
	Serial.println("Initializing I2C devices...");
	accelgyro.initialize();
	
	// verify connection
	Serial.println("Testing device connections...");
	Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}
*/
