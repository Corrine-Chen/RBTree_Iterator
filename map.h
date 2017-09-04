#include<map>
#include<string>
#include"Iterator.h"

template <class K,class V>//默认情况下map是从小到大排序
class Map
{
public:
	typename typedef RBTree<K, V>::Iterator Iterator;

	Map()
	{}

	bool Insert(const K& key, const V& value)
	{
		return t.Insert(key, value);
	}

	
	bool Empty()const
	{
		return t.Empty();
	}

	


	Iterator Begin()
	{
		return t.Begin();
	}

	Iterator End()
	{
		return t.End();
	}
    size_t Size()const
	{
		return t.Size();
	}
	Iterator Find()const
	{
		return t.Find();
	}


	V&operator[](const K& key)
	{
		Iterator it = t.Find(key);
		assert(it);
		return it._pNode->_value;
	}

	const V& operator[](const K& key)const
	{
		Iterator it = t.Find(key);
		return it._pNode->_value;
	}
private:
	RBTree<K,V> t;
};


void FunTest()
{

	Map<string, int> m;
	m.Insert("1", 1);
	m.Insert("2", 2);
	m.Insert("3", 3);

	Map<string, int>::Iterator it = m.Begin();
	while (it != m.End())
	{
		cout << (*it) << endl;
		++it;
	}

}


void FunTestMap()
{
{
	map<string, int> m;
	m.insert(pair<string,int>("1", 1));
	m.insert(pair<string, int>("2", 2));
	m.insert(pair<string, int>("3", 3));

	//m.insert(make_pair("3", 3));

	map<string, int>::iterator it1 = m.begin();
	while (it1 != m.end())
	{
		cout << (*it1).first << endl;
		++it1;
	}

	/*map<string, int>::reverse_iterator it2 = m.rbegin();
	while (it2 != m.rend())
	{
		cout << (*it2).first << endl;
		++it2;
	}*/
}


}




	

	

	


