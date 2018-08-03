
template <class T>
class RTree;

template <class T>
class RNode
{
	private:
		LPO<T> listLPO;
		LPO<T> *childs;
		int sizeM;
		int sizem;
	public:
		RNode(int M);
		friend class RTree<T>;

};

template <class T>
RNode<T>::RNode(int M)
{
	this->sizeM = M;
	this->sizem = (M+1)/2;
}