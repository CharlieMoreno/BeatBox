/*
 * Taps.h
 *
 * (fka Events.h)
 * Created: 19/06/2012 15:03:52
 *  Author: Yago
 */ 


#ifndef TAPS_H_
#define TAPS_H_

#include "BeatBox.h"

#define INSTRUMENTS 8

typedef struct {
	int	timeInCompass;
	unsigned char instrument;
	unsigned char note;
	unsigned char velocity;
	unsigned char sent;
} Tap;

extern Tap     taps[60];
extern int     tapCounter;
extern int     whichTapsNow[32];
extern int     lastBeatIndex;
extern int     compass;
extern int     compassLenght;
extern int     compassTime;
extern int     beatLength;
extern long    beatMillis[32];
extern unsigned long    nextCompass;
extern unsigned long    nextBeat;
extern boolean recordingTaps;
extern boolean identifying;


extern unsigned char noteMapping[INSTRUMENTS];

void deleteCompass();
void deleteAllTaps();
void deleteTapsWithInstrument(int i);

int getBeatLength();
int getCompass();
int getTapCounter();
int getTapTime(int i);
int getTapIntrument(int i);
int getTapVelocity(int i);

int getTapsNumForInstrument(int instrument);

void printTapInfo(int i);


#endif /* TAPS_H_ */
