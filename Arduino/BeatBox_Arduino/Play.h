/*
 * Play.h
 *
 * Created: 18/06/2012 12:01:58
 *  Author: Yago
 */ 


#ifndef PLAY_H_
#define PLAY_H_

#include "BeatBox.h"

void setupPlayback();
void playbackTaps();
void setQuantization(unsigned char q);
unsigned char getQuantization();
int getQTime();

#endif /* PLAY_H_ */
