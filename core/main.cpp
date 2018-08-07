#include <iostream>
#include "RTree.h"
#include "Polygon.h"
#include "LEG.h"

using namespace std;

int main()
{
	/*Poligono<double> poligono;
	poligono.insertar(2,3);
	poligono.insertar(0,0);
	poligono.insertar(5,7);	

	poligono.mostrarTodo();*/

	/**LPO<int> listaPoligonos;
	listaPoligonos.insertPolygon(2,3);
	listaPoligonos.insertPolygon(5,1);
	listaPoligonos.insertPolygon(3,1);

	listaPoligonos.showPolygons();*/


	
	RTree<int> rtree(5);
	//rtree.insertInRTree(1,2);
	rtree.insertInRTree(3,4);
	rtree.insertInRTree(5,6);
	rtree.insertInRTree(7,8);
	rtree.insertInRTree(9,10);
	rtree.insertInRTree(11,12);
	rtree.showValues();
	cout << "Order" << rtree.getOrder() << endl;
	cout << rtree.getSizeList() << endl;

	/*LEG<int> lista;
	lista.insertNodeLEG(34);
	lista.insertNodeLEG(4);
	lista.insertNodeLEG(3);
	lista.showAll();*/



	return 0;
}