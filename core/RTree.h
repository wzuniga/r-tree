#include "RNode.h"
#include "LPO.h"

template <class T>
class RTree
{
	private:
		RNode<T> *root;
		int order;
	public:
		RTree(int order);
		int getOrder();
		void insertInRTree(T x, T y);
		void showValues();

};

template <class T>
RTree<T>::RTree(int order)
{
	this->root = NULL;
	this->order = order;
}

template <class T>
int RTree<T>::getOrder()
{
	return this->order;
}

template <class T>
void RTree<T>::insertInRTree(T x, T y)
{
	if(root != NULL)
	{
		root->listLPO.insertPolygon(x,y);
		root->childs = NULL;
		
	}
	else
	{
		LPO<int> lp;
		this->root = new RNode<T>(order);
		this->root->listLPO = lp;
		root->listLPO.insertPolygon(x,y);
		root->childs = NULL;
	}
	
}

template <class T>
void RTree<T>::showValues()
{
	this->root->listLPO.showPolygons();
}