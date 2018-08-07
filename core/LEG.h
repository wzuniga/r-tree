#ifndef LEG_H
#define LEG_H

#include <iostream>
#include "NodeLEG.h"

using namespace std;

template <class T>
class LEG
{
	private:
		NodeLEG<T> *firstNodeLEG;
		int sizeLEG;
	public:
		LEG()
		{
			this->firstNodeLEG = NULL;
			this->sizeLEG = 0;
		}

		void insertNodeLEG(T x);
		void showAll();
		NodeLEG<T> *getFirstNodeLEG();
};

template <class T>
void LEG<T>::insertNodeLEG(T x)
{
	this->firstNodeLEG = new NodeLEG<T>(x,this->firstNodeLEG);
}

template <class T>
void LEG<T>::showAll()
{
	NodeLEG<T> * imp = this->firstNodeLEG;
	while(imp != NULL)
	{
		cout << imp->valor << endl;
		imp = imp->siguienteNode;
	}
}

template <class T>
NodeLEG<T> *LEG<T>::getFirstNodeLEG()
{
	return this->firstNodeLEG;
}


#endif
