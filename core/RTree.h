template <class T>
class RTree
{
	private:
		RNode<T> *root;
		int order;
	public:
		RTree(int order);

};

template <class T>
RTree<T>::RTree(int order)
{
	this->root = NULL;
	this->order = order;
}