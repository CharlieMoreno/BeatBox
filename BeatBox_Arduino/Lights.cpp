#include "Lights.h"

int  lightPin[6] = { 5, 9, 12, 3, 7, 10};
long lightOffTime[6] = {0, 0, 0, 0, 0, 0};
	
void lightsSetup() {
  for(int i = 0; i < 6; i++) {
    pinMode(lightPin[i], OUTPUT);
    digitalWrite(lightPin[i], LOW);
  }
}

void lightsUpdate() {							// Apaga los leds cuyo tiempo de encendido haya acabado.
  long t = millis();
  for(int i = 0; i < 6; i++) {
    if (t > lightOffTime[i]) {
      digitalWrite(lightPin[i], LOW);    
    }
  }
}

void lightsPulse(int i, int length) {			// Enciende el led i y establece un tiempo de encendido de duración length.
  long t = millis();
  digitalWrite(lightPin[i], HIGH);
  lightOffTime[i] = t + (long)length;
}

void lightsFlash(int length) {					// Enciende todos los leds y establece un tiempo de encendido de duración length.
  long t = millis();
  for(int i = 0; i < 6; i++) {
    digitalWrite(lightPin[i], HIGH);
    lightOffTime[i] = t + (long)length;
  }
}

void lightsBlink(boolean *flag, long *cycle){
	long t = millis();
	if(flag){
		*cycle = t + 2;
		for (int i=0; i<6; i++){
			digitalWrite(lightPin[i], HIGH);
		}
		*flag = false;
	}
	if(t >= (*cycle/2) && t < *cycle){
		for (int i=0; i<6; i++){
			digitalWrite(lightPin[i], LOW);
		}
	}
	if(t >= *cycle){
		*flag = true;
	}
}


void lightsTestSequence() {						// Enciende cada led durante un instante.
  for(int i = 0; i < 6; i++) {
    digitalWrite(lightPin[i], HIGH);
    delay(50);
    digitalWrite(lightPin[i], LOW);
  }
}

