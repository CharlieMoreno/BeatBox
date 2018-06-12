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
Vcc    <---> 5V
GND   <---> GND
TXD   <---> PIN 51  (No todos los pines de la MEGA pueden funcionar como RX)
RXD   <---> PIN 3
*/

#include "BeatBox.h"
#include "Trigger.h"
#include "Lights.h"
#include "Taps.h"
#include "Record.h"
#include "Midi.h"
#include "Play.h"
#include "Log.h"
#include "Debug.h"
//#include "I2Cdev.h"
#include "SparkFunMPU9250-DMP.h"

#define N 25


MPU9250_DMP accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

Trigger accX;
Trigger accY;
Trigger accZ;



unsigned long lastMillis = 0;
unsigned long loopMillis = 0;
unsigned long maxLoopMillis = 0;

/////////////////////////////////////////////

unsigned long triggerComparisonTime = 0;  //Tiempo que se deja tras un primer disparo para registrar los demás posibles disparos. Cuando se acaba, se comparan los disparos y se escoge el de mayor valor absoluto.

long lastMicros = micros();

int counter = 0;  // Contador usado para numerar los resultados en el monitor serie.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int axisTriggerValues[6] = {0, 0, 0, 0, 0, 0};  // getRealTrigger() guarda los valores de los disparos (yThreshold) en este vector [ejeX, ejeY, ejeZ] para después compararlos.

float maxTriggerValue = 0;    // getRealTrigger() va a guardar en esta variable el máximo valor de los disparos de todos los ejes.

int maxTriggerAxis = 0;     // Eje en el que se ha producido el impacto (mayor pico aceleración).


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MPU6050setup() {

  Serial.begin(2000000);
  
  // initialize device
  Serial.println("Initializing I2C devices...");
  
  
  if (accelgyro.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }
  
  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  accelgyro.setSensors(INV_XYZ_ACCEL);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  // imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  accelgyro.setAccelFSR(16); // Set accel to +/-16g
  // Note: the MPU-9250's magnetometer FSR is set at
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  accelgyro.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  accelgyro.setSampleRate(1000); // Set sample rate to 1000Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  // imu.setCompassSampleRate(10); // Set mag rate to 10Hz
  
}




void setup() {

  initTrigger(&accX);
  initTrigger(&accY);
  initTrigger(&accZ);

  lightsSetup();      // Se configuran los pins como salida y se ponen a nivel bajo.
  
  debugSetup();      
  
  logSetup();

  MPU6050setup();   

  for(int i = 0; i < 50; i++) {   //*******PARA QUÉ SE HACÍA ESTO 50 VECES?************** Para la media y el filtro tal vez? Calibración? 
    accelgyro.updateAccel();  
    updateTrigger(&accX, accelgyro.ax);
    updateTrigger(&accY, accelgyro.ay);
    updateTrigger(&accZ, accelgyro.az); 
  }
  
  recordSetup();

  //attachPeriodicInterrupt(updateAccel, 4); // Update Accel ~250 times per second.   // Esto era para el acelerómetro que usaba Yago.
  
  //posBufferSetup();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void loop() {
  
  
  //i = 0;  //Para qué demonios es esta i, declarada más arriba?
  
  accelgyro.updateAccel();

  updateTrigger(&accX, accelgyro.ax);
  updateTrigger(&accY, accelgyro.ay);
  updateTrigger(&accZ, accelgyro.az);
  
  /*Serial.print(getRealTrigger(&accX, &accY, &accZ, v));   //ESTO VA BIEN
  Serial.print("\t");*/


  int maxTriggerAxis = getRealTrigger(&accX, &accY, &accZ, axisTriggerValues, &maxTriggerValue, &counter);    // Devuelve el eje en el que se ha producido el impacto (mayor pico aceleración) y guarda en maxTriggerValue el valor de ese impacto, ó devuelve -1 mientras no haya impacto.

  // Comprobación de acierto por monitor serie:
  if (maxTriggerAxis != -1){  // Si se ha producido un impacto, escribe el número del impacto, el eje y la magnitud de los impactos en los 6 ejes:

    if (maxTriggerAxis == 0) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentL(&accX)); Serial.println("\tX+"); }
    if (maxTriggerAxis == 1) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentR(&accX)); Serial.println("\tX-"); }
    if (maxTriggerAxis == 2) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentL(&accY)); Serial.println("\tY+"); }
    if (maxTriggerAxis == 3) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentR(&accY)); Serial.println("\tY-"); }
    if (maxTriggerAxis == 4) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentL(&accZ)); Serial.println("\tZ+"); }
    if (maxTriggerAxis == 5) { Serial.print(++counter); Serial.print("\t"); Serial.print(getCurrentR(&accZ)); Serial.println("\tZ-"); }
      
    
  }
  
  
  
  //logValues(ax, ay, az);

  //detectShakes();
      
  //recordTaps();

  //playbackTaps();
      
  //lightsUpdate();
      
      
      
  //Cuánto dura la vuelta de loop más larga?:
  loopMillis = millis() - lastMillis;
  maxLoopMillis = max(maxLoopMillis, loopMillis);
  lastMillis = millis();
      
  //debugUpdate();
  
}






////////// FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////




