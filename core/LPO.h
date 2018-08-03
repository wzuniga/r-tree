template <class T>
class LPO
{
	private:
		Polygon<T> *firstLPO;
		int sizeLPO;
	public: 
		LPO()
		{
			this->firstLPO = NULL;
			this->sizeLPO = 0;
		}

		LPO(int sizeLPO, Polygon<T> *firstLPO = NULL)
		{
			this->sizeLPO = sizeLPO;
			this->firstLPO = firstLPO;
		}

		void insertPolygon(T x, T y);
		void showPolygons();

};

template <class T>
void LPO<T>::insertPolygon(T x, T y)
{
	this->firstLPO = new Polygon<T>(this->firstLPO);
	this->firstLPO->insertPoint(x,y);
	this->sizeLPO++;
}

template <class T>
void LPO<T>::showPolygons()
{
	Polygon<T> *next = firstLPO;
	while(next != NULL)
	{
		cout << "Polygon";
		next->showPoints();
		next = next->nextPolygon;
	}
}


