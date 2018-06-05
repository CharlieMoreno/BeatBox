#ifndef MEAN_H_
#define MEAN_H_

// #include "BeatBox.h"

#define MAX_TAPS 32

typedef struct {
	unsigned char taps;
	unsigned char pointer;		//índice del buffer circular.
	int sum;
	int filterData[MAX_TAPS];	//MAX_TAPS es el tamaño del buffer, pero el valor de taps determina cuántas posiciones del buffer se van a usar.
} MeanFilter;

/*MeanFilter * createMeanFilter(); */
void         initMeanFilter(MeanFilter * f, int t);
void         destroyMeanFilter(MeanFilter * f);
int	         meanFilterIO(MeanFilter * f, int d);

#endif /* MEAN_H_ */

