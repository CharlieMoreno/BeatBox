#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "BeatBox.h"

void lightsSetup();
void lightsUpdate();
void lightsTestSequence();
void lightsBlink(boolean *flag, long *cycle);
void lightsPulse(int i, int length);
void lightsFlash(int length);

#endif /* LIGHTS_H_ */

