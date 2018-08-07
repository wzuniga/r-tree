#include "RNode.h"
#include "Polygon.h"
#include "LPO.h"

template <class T>
class RTree
{
	private:
		RNode<T> *root;
		int order;
	public:
		RTree(int order);
		void insertInRTree(T x, T y);
		void showValues();
		void insert(RNode<T> *rnode, Polygon<T> *polygon);
		void dividirRNode(RNode<T> *rnode);
		int getOrder();
		int getSizeList();
		void showChilds(RNode<T> *father);

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
		Polygon<T> *poly = new Polygon<T>();
		poly->insertPoint(x,y);
		//root->listLPO.insertPolygon(poly);
		//root->listLPO.insertPolygon(x,y);
		//root->childs = NULL;
		insert(root,poly);
		
	}
	else
	{
		//LPO<int> lp;
		this->root = new RNode<T>(order);
		//this->root->listLPO = lp;
		root->listLPO.insertPolygon(x,y);
		//root->childs = NULL;
	}
	
}

template <class T>
void RTree<T>::insert(RNode<T> *rnode, Polygon<T> *polygon)
{
	if(rnode->listLPO.getSizeLPO() >= rnode->sizeM)
	{
		rnode->listLPO.insertPolygon(polygon);
		dividirRNode(rnode);
	}
	else
		rnode->listLPO.insertPolygon(polygon);
}

template <class T>
void RTree<T>::dividirRNode(RNode<T> *father)
{
	RNode<T> *child_f = new RNode<T>(this->order);
	RNode<T> *child_s = new RNode<T>(this->order);
	father->childs.insertNodeLEG(child_f);
	father->childs.insertNodeLEG(child_s);

	for(int i=0; i<father->sizeM+1; i++)
	{
		if(i%2 == 0)
			child_f->listLPO.insertPolygon(father->listLPO.getPolygon());
		else
			child_s->listLPO.insertPolygon(father->listLPO.getPolygon());
	}

	Polygon<T> *poly1 = new Polygon<T>();
	poly1->insertPoint(13,14);
	Polygon<T> *poly2 = new Polygon<T>();
	poly2->insertPoint(15,16);
	
	father->listLPO.insertPolygon(poly1);
	father->listLPO.insertPolygon(poly2);
	
}

template <class T>
void RTree<T>::showValues()
{
	showChilds(this->root);
}

template <class T>
void RTree<T>::showChilds(RNode<T> *father)
{
	NodeLEG<RNode<T>*> *child= father->childs.getFirstNodeLEG();

	if(child != NULL)
	{
		Polygon<T> *polygon = father->listLPO.getFirstPolygon();
		while(polygon != NULL)
		{
			cout << "Polygon" << endl;
			polygon->showPoints();

			RNode<T> *firstRNode = child->getNodeValue();
			showChilds(firstRNode);

			polygon = polygon->getNextPolygon();
			child = child->getNextNode();
		}

		//firstRNode->listLPO.showPolygons();
		//child = child->getNextNode();
	}
	else
	{
		father->listLPO.showPolygons();
	}
}

/*template <class T>
void RTree<T>::showChilds(RNode<T> *childs)
{
	Polygon<T> *polygon = father->listLPO.getFirstPolygon();
	while(polygon != NULL)
	{
		RNode<T> *firstRNode = first->getNodeValue();
		firstRNode->listLPO.showPolygons();

		first = first->getNextNode();
	}
}*/

/*template <class T>
void RTree<T>::showValues()
{
	this->root->listLPO.showPolygons();
	NodeLEG<RNode<T>*> *first= this->root->childs.getFirstNodeLEG();

	while(first != NULL)
	{
		RNode<T> *firstRNode = first->getNodeValue();
		firstRNode->listLPO.showPolygons();

		first = first->getNextNode();
	}
	
}*/

template <class T>
int RTree<T>::getSizeList()
{
	return this->root->listLPO.getSizeLPO();
}