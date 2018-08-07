#ifndef NODELEG_H
#define NODELEG_H

template <class T>
class LEG;

template <class T>
class NodeLEG
{
	private:
		NodeLEG<T> *nextNode;
		T valor;
	public:
		NodeLEG(T valor, NodeLEG<T> *sig = NULL)
		{
			this->valor = valor;
			this->nextNode = sig;
		}
		T getNodeValue();
		NodeLEG<T> *getNextNode();
		friend class LEG<T>;

};

template <class T>
T NodeLEG<T>::getNodeValue()
{
	return this->valor;
}

template <class T>
NodeLEG<T> * NodeLEG<T>::getNextNode()
{
	return this->nextNode;
}

#endif