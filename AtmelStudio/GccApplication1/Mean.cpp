#include "Mean.h"
#include "BeatBox.h"

/*MeanFilter * createMeanFilter() {
	MeanFilter* f  = (MeanFilter *)malloc(sizeof(MeanFilter));
	initMeanFilter(f, MAX_TAPS);
	return f;
}*/	//No se utiliza esta funci�n. Como Las estructuras Trigger se declaran de forma est�tica (no se utiliza createTrigger()), ya no es necesario crear un filtro de forma din�mica.

/*void destroyMeanFilter(MeanFilter * f) {
	free(f);
}*/

void initMeanFilter(MeanFilter * f, int t) {
	// MeanFilter* f  = (MeanFilter *)malloc(sizeof(MeanFilter));
	f->taps        = t;
	f->pointer     = 0;
	f->sum         = 0;
}

int meanFilterIO(MeanFilter * f, int d) {	//Va actualizando los valores y calculando la media con cada valor nuevo.
	f->sum += d;									//Se suma el que entra
	f->sum -= f->filterData[f->pointer];			//y se resta el que sale.
	f->filterData[f->pointer] = d;					//Se sobreescribe el valor m�s antiguo con el nuevo valor.
	f->pointer = (f->pointer + 1) % f->taps;		//El puntero avanza una posici�n. Se utiliza el operador m�dulo (resto de la divisi�n entera) para que el puntero sea circular.
	return f->sum / f->taps;						//Devuelve la media
}

