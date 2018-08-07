#ifndef PUNTO_H
#define PUNTO_H

template <class T>
class Polygon;

template <class T>
class Point{
	private:
		T coorX;
		T coorY;
		Point<T> *nextPoint;
	public:
		Point(T x, T y, Point<T> *nextPoint = NULL)
		{
			this->coorX = x;
			this->coorY = y;
			this->nextPoint = nextPoint;
		}

		friend class Polygon<T>;
};

#endif 