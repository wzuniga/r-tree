#ifndef LPO_H
#define LPO_H

#include "Polygon.h"

template <class T>
class LPO
{
	private:
		Polygon<T> *firstPolygon;
		int sizeLPO;
	public: 
		LPO()
		{
			this->firstPolygon = NULL;
			this->sizeLPO = 0;
		}

		LPO(int sizeLPO, Polygon<T> *firstPolygon = NULL)
		{
			this->sizeLPO = sizeLPO;
			this->firstPolygon = firstPolygon;
		}

		void insertPolygon(T x, T y);
		void insertPolygon(Polygon<T> *polygon);
		void showPolygons();
		int getSizeLPO();
		Polygon<T>* getPolygon();//is like POP
		Polygon<T>* getFirstPolygon();

};

template <class T>
void LPO<T>::insertPolygon(T x, T y)
{
	this->firstPolygon = new Polygon<T>(this->firstPolygon);
	this->firstPolygon->insertPoint(x,y);
	this->sizeLPO++;
}

template <class T>
void LPO<T>::insertPolygon(Polygon<T> *polygon)
{
	polygon->nextPolygon = this->firstPolygon;
	this->sizeLPO++;
	this->firstPolygon = polygon;
}

template <class T>
void LPO<T>::showPolygons()
{
	Polygon<T> *next = firstPolygon;
	while(next != NULL)
	{
		cout << "Polygon";
		next->showPoints();
		next = next->nextPolygon;
	}
}

template <class T>
int LPO<T>::getSizeLPO()
{
	return sizeLPO;
}

template <class T>
Polygon<T>* LPO<T>::getPolygon()
{
	Polygon<T> *getPolygon = this->firstPolygon;
	this->firstPolygon = this->firstPolygon->nextPolygon;
	getPolygon->nextPolygon = NULL;
	return getPolygon;
}

template <class T>
Polygon<T>* LPO<T>::getFirstPolygon()
{
	return this->firstPolygon;
}

#endif 