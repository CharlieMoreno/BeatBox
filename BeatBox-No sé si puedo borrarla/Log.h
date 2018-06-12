#ifndef LOG_H_
#define LOG_H_

#include "BeatBox.h"

#define LOG_LENGTH   64  // must be power of 2 (2^n)!!
#define LOG_PTR_MASK (LOG_LENGTH - 1) // for quick compute modulus

#ifdef DATA_LOG
	#define logSetup()				_logSetup()
	#define logValues(A, B, C, D)	_logValues((A), (B), (C), (D))
	#define printLoggedAccs()		_printLoggedAccs()
	#define printMeanAccels()		_printMeanAccels()
#else
	#define logSetup()			
	#define logValues(A, B, C, D)	
	#define printLoggedAccs()	
	#define printMeanAccels()	
#endif
	
void _logSetup();
void _logValues(int valX, int valY, int valZ, boolean trigger);
void _printLoggedAccs();
void _printMeanAccels();

#endif /* LOG_H_ */

