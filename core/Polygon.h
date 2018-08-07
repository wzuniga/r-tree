#ifndef POLIGONO_H
#define POLIGONO_H

#include <iostream>
#include "Point.h"

using namespace std;

template <class T>
class LPO;

template <class T>
class Polygon
{
	private:
		Point<T> *first;
		Polygon<T> *nextPolygon;
		int sizePolygon;
	public:
		Polygon(Polygon<T> *nextPolygon = NULL)
		{
			this->nextPolygon = nextPolygon;
			this->first = NULL;
			this->sizePolygon = 0;
		}

		void insertPoint(T x, T y);
		void showPoints();
		Polygon<T> *getNextPolygon();

		friend class LPO<T>;
};

template <class T>
void Polygon<T>::insertPoint(T x, T y)
{
	this->first = new Point<T>(x,y,first);
	this->sizePolygon++;
}

template<class T>
void Polygon<T>::showPoints()
{
	Point<T> *next = first;
	while(next != NULL)
	{
		cout << "("<<next->coorX << "-" <<next->coorY << ")"<<endl;
		next = next->nextPoint;
	}
}

template <class T>
Polygon<T> *Polygon<T>::getNextPolygon()
{
	return this->nextPolygon;
}


#endif 