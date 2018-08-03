#include <iostream>
#include "Polygon.h"
#include "LPO.h"

using namespace std;

int main()
{
	/*Poligono<double> poligono;
	poligono.insertar(2,3);
	poligono.insertar(0,0);
	poligono.insertar(5,7);	

	poligono.mostrarTodo();*/

	LPO<int> listaPoligonos;
	listaPoligonos.insertPolygon(2,3);
	listaPoligonos.insertPolygon(5,1);
	listaPoligonos.insertPolygon(3,1);

	listaPoligonos.showPolygons();


	return 0;
}