#include "Mean.h"
#include "BeatBox.h"

/*MeanFilter * createMeanFilter() {
	MeanFilter* f  = (MeanFilter *)malloc(sizeof(MeanFilter));
	initMeanFilter(f, MAX_TAPS);
	return f;
}*/	//No se utiliza esta función. Como Las estructuras Trigger se declaran de forma estática (no se utiliza createTrigger()), ya no es necesario crear un filtro de forma dinámica.

void initMeanFilter(MeanFilter * f, int t) {
	// MeanFilter* f  = (MeanFilter *)malloc(sizeof(MeanFilter));
	f->taps        = t;
	f->pointer     = 0;
	f->sum         = 0;
}

void destroyMeanFilter(MeanFilter * f) {
	free(f);
}

int meanFilterIO(MeanFilter * f, int d) {
	f->sum += d;									//se suma el que entra
	f->sum -= f->filterData[f->pointer];			//y se resta el que sale
	f->filterData[f->pointer] = d;
	f->pointer = (f->pointer + 1) % f->taps;		//módulo: resto de la división entera,  para que el indice sea circular
	return f->sum / f->taps;		
}

