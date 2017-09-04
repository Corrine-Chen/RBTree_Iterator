
#include<iostream>
using namespace std;

enum COLOR{RED, BLACK};

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode(const K& key = K(), const V& value = V(), const COLOR& color = RED)
		: _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _color(color)
	{}

	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	COLOR _color;
};


template <class K,class V,class Ref,class Ptr>
class RBTreeIterator
{
	typedef RBTreeIterator<K,V,Ref,Ptr> Self;
	typedef RBTreeNode<K,V> Node;
public:
	RBTreeIterator()
	{}
	RBTreeIterator(Node* pNode)
		:_pNode(pNode)
	{}
	RBTreeIterator<K,V,Ref,Ptr>(const Self& it)
		:_pNode(it._pNode)
	{}
	Self& operator++()
	{
		_Increment(_pNode);
		return *this;
	}
	Self operator++(int)
	{
		Self temp(*this);
		_Increment(_pNode);
		return temp;
	}
	Self& operator--()
	{
		_Decrement(_pNode);
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_Decrement(_pNode);
		return temp;
	}

	Ptr operator->()
	{
		return &_pNode->_value;
	}

	Ref operator *()
	{
		return _pNode->_key;
	}
	bool operator ==(const Self &it)
	{
		return _pNode == it._pNode;
	}
	bool operator !=(const Self &it)
	{
		return _pNode != it._pNode;
	}
private:
	 Node* _Increment(Node* pNode)
	 {
		 if(pNode->_pRight)
		 {
			 pNode = pNode->_pRight;
			 while(pNode->_pLeft)
				pNode = pNode->_pLeft;
		 }
		 else
		 {
			 Node* pParent = pNode->_pParent;
			 while(pParent->_pRight == pNode)
			 {
				 pNode = pParent;
				 pParent = pParent->_pParent;
			 }

			 if(pNode->_pRight != pParent)//特殊情况，根节点没有右子树，获取根节点的下一个结点
				 pNode = pParent;
		 }

		 _pNode = pNode;
		 return pNode;


	 }
	Node* _Decrement(Node* pNode)
	{
		if(pNode->_pLeft)
		{
			Node* pCur = pNode->_pRight;
			 while(pCur->_pLeft)
				 pCur = pCur->_pRight;
		}
		else
		{
			Node* pParent = pNode->_pParent;
			while(_pNode == pParent->_pLeft)
			{
				 pNode = pParent;
				 pParent = pParent->_pParent;
			}
		}

		if(RED == pNode->_color && pNode->_pParent->_pParent == pNode)
			pNode = pNode->_pRight;
		_pNode = pNode;
		return pNode;
	}


	Node* _pNode ;
};


template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	typedef RBTreeIterator<K, V, K&, K*> Iterator;
public:
	
	RBTree()
	{
		_pHead = new Node();//_pHead的左指针域指向最小的节点，右指针域指向最大的节点，双亲指向根节点，根节点的双亲指向头节点
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
	}

	Iterator Begin()
	{
		return Iterator(_pHead->_pLeft);
	}
	Iterator End()
	{
		return Iterator(_pHead);
	}
	Iterator Find(const K& key)
	{
		Node* pCur = _pHead->_pParent;
		while(pCur)
		{
			if(key < pCur->_key)
			{
				pCur = pCur->_pLeft;
			}
			else if(key > pCur->_pRight)
			{
				pCur = pCur->_pRight;
			}
			else
			{
				return Iterator(pCur);
			}
		}
		return Iterator(NULL);
	}

	bool Insert(const K& key, const V& value)
	{
		Node* _pRoot = _pHead->_pParent;
		if(_pRoot == NULL)//根节点为空  直接插入
		{
			_pRoot = new Node(key,value,BLACK);
			_pHead->_pParent = _pRoot;
			_pRoot->_pParent = _pHead;

			_pHead->_pLeft = _pRoot;
			_pHead->_pRight = _pRoot;
			return true;
		}


		Node* pCur = _pRoot;
		Node* parent = pCur;

		while(pCur)
		{
			if(key < pCur->_key)//往节点的 左边走
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if(key > pCur->_key)//往节点的右边走
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else//找到key值，则不需要插入
			{
				return false;
			}
		}

		//找到插入位置,插入元素
		pCur = new Node(key,value);
		if(key < parent->_key)
		{
			parent->_pLeft = pCur;
			pCur->_pParent = parent;
		}
		else
		{
			parent->_pRight = pCur;
			pCur->_pParent = parent;
		}

		while(pCur->_color == RED && parent->_color == RED  )
		{
			Node* grandfather = parent->_pParent;
			Node* uncle = NULL;

			if(grandfather->_pLeft == parent)//节点插在左侧
			{
				uncle = grandfather->_pRight;
				if(uncle && uncle->_color == RED)//情况1
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;

					pCur = grandfather;

				}
				else//情况2,3
				{
					if(uncle && uncle->_color == BLACK &&pCur == parent->_pRight)
					{
                        _RotateL(parent);
                        swap(pCur,parent);
                    }
					_RotateR(grandfather);
                    grandfather->_color = RED;
                    parent->_color = BLACK;

					if(parent->_pParent && parent->_pParent->_color == BLACK && parent->_pParent->_pParent && parent->_pParent->_pParent ->_color==BLACK)
					{
						parent->_pParent->_color = RED;
					}
				}
			}
			else//节点插在右侧
			{
				uncle = grandfather->_pLeft;
				if(uncle && uncle->_color == RED)//情况1
				{
					parent->_color = BLACK;
					uncle->_color = BLACK;
					grandfather->_color = RED;

					pCur = grandfather;
				}
				else//情况2，3
				{
					if(uncle && uncle->_color == BLACK && parent->_pLeft == pCur)
					{
                        _RotateR(parent);
                        swap(pCur,parent);
                    }
					_RotateL(grandfather);
                    grandfather->_color = RED;
                    parent->_color = BLACK;


					

					if(parent->_pParent && parent->_pParent->_color == BLACK && parent->_pParent->_pParent && parent->_pParent->_pParent ->_color==BLACK)
					{
						parent->_pParent->_color = RED;
					}
				}
			}
		}
		_pRoot = _pHead->_pParent;
		_pRoot->_color = BLACK;
		_pHead->_pLeft = _GetMinNode();
		_pHead->_pRight = _GetMaxNode();


	}

	void InOrder()
	{
		cout<<" InOrder: ";
		_InOrder(_pHead->_pParent);
		cout<<endl;
	}

	bool CheckRBTree()
	{
		Node* _pRoot = _pHead->_pParent;
		if(_pRoot == _pHead)
			return true;

		if(_pRoot->_color == RED)
		{
			return false;
		}
		int blackCount=0 ;
		Node* pCur = _pRoot;
		while(pCur)
		{
			if(pCur->_color == BLACK)
				blackCount++;
			pCur = pCur->_pLeft;
		}

		_CheckRBTree(_pRoot, blackCount, 0);
		return true;
	}

protected:
	void _RotateL(Node* pParent)
	{
		Node* subR = pParent->_pRight;
		Node* subRL = subR->_pLeft;
		pParent->_pRight = subRL;

		if(subRL)
		{
			subRL->_pParent = pParent ;
		}

		Node* pPParent = pParent->_pParent;
		subR->_pLeft = pParent;
		pParent->_pParent = subR;
		if(pPParent == _pHead)
		{
			_pHead->_pParent = subR;
			subR->_pParent = _pHead;
		}
		else
		{
			if(pParent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subR;
				subR->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subR;
				subR->_pParent = pPParent;
			}
		}

	}

	void _RotateR(Node* pParent)
	{
		
		Node* subL = pParent->_pLeft;
    	        Node* subLR = subL->_pRight;
		pParent->_pLeft = subLR;

		if(subLR)
		{
			subLR->_pParent = pParent;
		}

		Node* pPParent = pParent->_pParent;

		subL->_pRight = pParent;
		pParent->_pParent = subL;
		if(pPParent == _pHead)
		{
			_pHead->_pParent = subL;
			subL->_pParent = _pHead;
		}
		else
		{
			if(pParent == pPParent->_pLeft)
			{
				pPParent->_pLeft = subL;
				subL->_pParent = pPParent;
			}
			else
			{
				pPParent->_pRight = subL;
				subL->_pParent = pPParent;
			}
		}

	
	}
	void _InOrder(Node* pRoot)
	{
		if(_pHead->_pParent == NULL)
			return ;
		_InOrder(pRoot->_pLeft);
		cout<<_pHead->_pParent->_key<<" ";
		_InOrder(_pHead->_pParent->_pRight);
	}
	bool _CheckRBTree(Node* pRoot, const size_t blackCount, size_t k)
	{
		Node* _pRoot = _pHead->_pParent;
		if(pRoot == _pHead)
			return true;
		if(pRoot->_color == BLACK)
			++k;
		Node* parent = pRoot->_pParent;

		if(parent && parent->_color == RED && pRoot->_color == RED)
		{
			cout<<"违反了规则3，不是红黑树"<<endl;
			return false;
		}
		if(parent && parent->_pLeft== NULL && parent->_pRight ==NULL)
		{
			if(k != blackCount)
			{
				cout<<"违反了规则4，不是红黑树"<<endl;
				return false;
			}
		}
		return (_CheckRBTree(pRoot->_pLeft ,blackCount,k)&&_CheckRBTree(pRoot->_pRight,blackCount,k));
	}
	Node* _GetMinNode()
	{
		Node* _pCur = _pHead->_pParent; 
		while(_pCur->_pLeft)
		{
			if(_pHead->_pParent == NULL)
			{
				return _pHead;
			}
			else
			{
				_pCur = _pCur->_pLeft;
			}
		}
		return _pCur;
	}
	Node* _GetMaxNode()
	{
		Node* _pCur = _pHead->_pParent; 
		while(_pCur->_pRight)
		{
			if(_pHead->_pParent == NULL)
			{
				return _pHead;
			}
			else
			{
				_pCur = _pCur->_pRight;
			}
		}
		return _pCur;
	}
protected:
	Node* _pHead;
};



void TestRBTreeIterator()
{
	int a[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	RBTree<int,int> t;
	for(int idx = 0; idx < sizeof(a)/sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);
	RBTree<int,int>::Iterator it=t.Begin();
	while(it != t.End())
	{
		cout<<*it<<" ";
		++it;
	}
	cout<<endl;


}

