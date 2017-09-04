#include"Iterator.h"
#include<string>
#include<set>

template<class V>
class Set
{
public:
	typename typedef RBTree<V,V>::Iterator Iterator;

	Set()
	{}
	bool Insert(const V& value)
	{
		return t.Insert(value,value);
	}

	Iterator Begin()
	{
		return t.Begin();
	}
	Iterator End()
	{
		return t.End();
	}

	bool Empty()const
	{
		return t.Empty();
	}

	size_t Size()const
	{
		return t.Size();
	}

	Iterator Find()
	{
		return t.Find();
	}
	V& operator[](const V& key)
	{
		Iterator it = t.Find(key);
		assert(it);
		return it._pNode->_value;
	}

private:
	RBTree<V,V> t;
};

void test()
{
	Set<string> m;
	m.Insert("1");
	m.Insert("2");
	m.Insert("3");

	Set<string>::Iterator it = m.Begin();
	while (it != m.End())
	{
		cout << (*it) << endl;
		++it;
	}
}


void Testset()
{
	set<string> m;
	m.insert("1");
	m.insert("2");
	m.insert("3");

	/*set<string>::iterator it1 = m.begin();
	while (it1 != m.end())
	{
		cout << (*it1)<< endl;
		++it1;
	}
*/
	set<string>::reverse_iterator it2 = m.rbegin();
	while (it2 != m.rend())
	{
		cout << (*it2) << endl;
		++it2;
	}
}