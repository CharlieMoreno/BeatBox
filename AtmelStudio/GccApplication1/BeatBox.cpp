/*
* GccApplication1.c
*
* Created: 06/06/2012 18:59:34
*  Author: Yago
*/
/*
*** MPU6050 Setup: ***

Acc_Gyro <--->  Arduino
Vcc      <--->  3.3V
GND      <--->  GND
SCL      <--->  PIN 21 (SCL)
SDA      <--->  PIN 20 (SDA)
INT      <--->  2

*** HC-05 Bluetooth Setup: ***

HC_05 <---> Arduino
Vcc	  <---> 5V
GND   <---> GND
TXD   <---> PIN 51	(No todos los pines de la MEGA pueden funcionar como RX)
RXD   <---> PIN 3
*/

#include "BeatBox.h"
//#include "Accel.h"
#include "Trigger.h"
#include "Lights.h"
#include "Taps.h"
#include "Record.h"
#include "Midi.h"
#include "Play.h"
#include "Log.h"
#include "Debug.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Midi.h"
//#include <SoftwareSerial.h>

#define N 25

// Creación del puerto Bluetooth:
SoftwareSerial BT(51,3);	// RX | TX		(No todo los pines de la MEGA pueden funcionar como RX)

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;			// Declaración de clase tipo MPU6050 llamada accelgyro.
//MPU6050 accelgyro(0x69);	// <-- use for AD0 high



int16_t ax, ay, az;
int16_t gx, gy, gz;

Trigger accX;
Trigger accY;
Trigger accZ;

void MPU6050setup() {								
	// join I2C bus (I2Cdev library doesn't do this automatically)
	Wire.begin();
	
	// Initialize USB serial communication
	Serial.begin(115200);
	
	// initialize device
	Serial.println("Initializing I2C devices...");
	accelgyro.initialize();
	
	accelgyro.setXAccelOffset(-2194);
	accelgyro.setYAccelOffset(-1871);
	accelgyro.setZAccelOffset(868);
	
	accelgyro.setXGyroOffset(76);
	accelgyro.setYGyroOffset(-6);
	accelgyro.setZGyroOffset(37);
	
	accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);

	
	// verify connection
	Serial.println("Testing device connections...");
	Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed");
}


////MEDIA
//float posBuffer[N];
//float sum = 0;
//
//void posBufferSetup(){
	//for(int j = 0; j<N; j++){
		//posBuffer[j] = 0;
	//}
//}
//
//float updateAverage(float newValue){
	//sum -= posBuffer[0];
	//sum += newValue;
	//for (int i=0; i < N-1; i++){
		//posBuffer[i] = posBuffer[i+1];
	//}
	//posBuffer[N-1] = newValue;
	//return sum/N;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

	initTrigger(&accX);
	initTrigger(&accY);
	initTrigger(&accZ);

	lightsSetup();			// Se configuran los pins como salida y se ponen a nivel bajo.
	
	debugSetup();			 
	
	logSetup();
	
	#ifdef BLUETOOTH	
	BT.begin(115200);
	#endif
	
	MPU6050setup();		
	
	//initAccel();			// Esto era para el acelerómetro que usaba Yago.

	for(int i = 0; i < 50; i++) {		//*******PARA QUÉ SE HACÍA ESTO 50 VECES?************** Para la media y el filtro tal vez? Calibración?	
		accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);	
		updateTrigger(&accX, ax);
		updateTrigger(&accY, ay);
		updateTrigger(&accZ, az);	
	}
	
	recordSetup();

	//attachPeriodicInterrupt(updateAccel, 4); // Update Accel ~250 times per second.		// Esto era para el acelerómetro que usaba Yago.
	
	//posBufferSetup();
	

}


//char strBuff[64];
//void plotAcc(int a) {
	//for(int i = 0; i < 63; i++) {
		//strBuff[i] = ' ';
		//if (i == (a >> 5)) strBuff[i] = 'x';
	//}
	//strBuff[63] = 0;
	//Serial.println(strBuff);
//}

unsigned long lastMillis = 0;
unsigned long loopMillis = 0;
unsigned long maxLoopMillis = 0;

//long int timeIn;
//int i;	// Para qué demonios es esta i??


/////////YA NO UTILIZO EL MODELO DEL MUELLE
//float pos = 50, vel = 0, aTot = 0, aMuelle = 0, aVisc = 0;
//float posx, velx;
//float len = 100, kMuelle = 200, rozamiento = 0.05, t = 0.03;
//int golpe = 0;
//
//int percute(int a) {
	//aMuelle = (pos - len/2)*kMuelle;
	//aVisc = (vel > 0 ? -rozamiento * vel * vel : (vel < 0 ? rozamiento * vel * vel : 0));
	//aTot = a - aMuelle + aVisc;
	//vel += aTot * t;
	////if (abs(vel) > 0)  
	//pos += vel * t + aTot/2 * t * t ;
	////Condiciones de contorno:
	//if (pos >= len ) {
		//pos = len;
		//vel = 0;
		//golpe += 4000;
		////return pos*100;
	//}
	//if (pos <= 0) {
		//pos = 0;
		//vel = 0;
		//golpe -= 4000;
		////return -pos*100;
	//}
	//if (az > 15000) {
		//kMuelle += 5;
	//}
	//return pos*100;
//}

/////////////////////////////////////////////


unsigned long triggerComparisonTime = 0;	//Tiempo que se deja tras un primer disparo para registrar los demás posibles disparos. Cuando se acaba, se comparan los disparos y se escoge el de mayor valor absoluto.

long lastMicros = micros();
	
int counter = 0;	// Contador usado para numerar los resultados en el monitor serie.


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int axisTriggerValues[6] = {0, 0, 0, 0, 0, 0};	// getRealTrigger() guarda los valores de los disparos (yThreshold) en este vector [ejeX, ejeY, ejeZ] para después compararlos.

int maxTriggerValue = 0;		// getRealTrigger() va a guardar en esta variable el máximo valor de los disparos de todos los ejes.

int maxTriggerAxis = 0;			// Eje en el que se ha producido el impacto (mayor pico aceleración).


void loop() {
	
	
	//i = 0;	//Para qué demonios es esta i, declarada más arriba?
	
	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	updateTrigger(&accX, ax);
	updateTrigger(&accY, ay);
	updateTrigger(&accZ, az);
	


	getRealTrigger(&accX, &accY, &accZ, axisTriggerValues, &maxTriggerValue, &maxTriggerAxis, &counter);		// Devuelve el eje en el que se ha producido el impacto (mayor pico aceleración) y guarda en maxTriggerValue el valor de ese impacto, ó devuelve -1 mientras no haya impacto.

	//Comprobación de acierto por monitor serie:
	//if (maxTriggerAxis != -1){	// Si se ha producido un impacto, escribe el número del impacto, el eje y la magnitud de los impactos en los 6 ejes:	
		//switch (maxTriggerAxis) {
		//case 0: Serial.print(String(++counter) + "\t" + String(getCurrentL(&accX)) + "\tX+");
		//case 1: Serial.print(String(++counter) + "\t" + String(getCurrentR(&accX)) + "\tX-");
		//case 2: Serial.print(String(++counter) + "\t" + String(getCurrentL(&accY)) + "\tY+");
		//case 3: Serial.print(String(++counter) + "\t" + String(getCurrentR(&accY)) + "\tY-");
		//case 4: Serial.print(String(++counter) + "\t" + String(getCurrentL(&accZ)) + "\tZ+");
		//case 5: Serial.print(String(++counter) + "\t" + String(getCurrentR(&accZ)) + "\tZ-");
		//}
	//}
			

	
	//logValues(ax, ay, az);

	//detectShakes();
			
	recordTaps();

	playbackTaps();
			
	//lightsUpdate();
			
			
			
	//Cuánto dura la vuelta de loop más larga?:
	loopMillis = millis() - lastMillis;
	maxLoopMillis = max(maxLoopMillis, loopMillis);
	lastMillis = millis();
			
	debugUpdate();
	
}



