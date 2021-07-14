
#include "iterator.h"
#include"allocator.h"

#include<iostream>
using namespace std;

namespace mystl
{
	//C语言实现
	typedef struct MyList {
		int data;
		struct MyList* pNext;

	}LIST_S;

	int InitList(LIST_S **ppList)
	{
		*ppList = (LIST_S*)malloc(sizeof(LIST_S)); 
		if (*ppList == NULL)
		{
			return -1;
		}
		(*ppList)->pNext = NULL;
		return 0;
	}


	template <typename T>
	class CDList
	{
	private:
		class Node {
		public :
			T data;
			Node* pNext;
			Node* pPrev;
			Node(T tdata) : data(tdata), pNext(nullptr), pPrev(nullptr) {}
		};
		Node* pHead;  //指向第一个节点， 不放数据
		Node* pTail;   //最后一个节点  不放数据
		size_t iSize;  //数据个数
	public:
		CDList():iSize(0),pHead(new Node(T())),pTail(new Node(T())){
			//把pHead和pTail连起来
			pHead->pNext = pTail;
			pTail->pPrev = pHead;
		}

		size_t size() { return iSize; }

		~CDList()
		{
			clear();
			delete pHead;
			delete pTail;
		}

		void clear();

		//增删改查
		int insert(size_t pos, T data);
		int erase(size_t pos);
		int modify(size_t pos, T data);
		T& getValue(size_t pos);


		void output()
		{
			Node* pNode = pHead->pNext;
			while (pNode != pTail)
			{
				cout << pNode->data << " ";
				pNode = pNode->pNext;
			}
			cout << endl;


		}

	public:
		class iterator{
			friend class CDList;
		private:
			Node* pNode;//使用这个指针指向各个节点
			iterator(Node* pTmp) : pNode(pTmp){}
		public:
			
			iterator() :pNode(NULL);
			//重载 ++
			iterator operator++() {
				pNode = pNode->pNext;
				return *this;
			}
			iterator operator++(int) {
				iterator tmp = *this;
				pNode = pNode->pNext;
				return tmp;
			}

			bool operator==(iterator it) {
				return pNode == it.pNode
			}
			bool operator!= (iterator it)
			{
				return pNode != it.pNode;
			}

			T&  operator*() {
				return pNode->data;
			}
			iterator begin()
			{
				return iterator(pHead->pNext);
			}
			iterator end()
			{
				return iterator(NULL);
			}

		};

	};

	template < typename T>
	int CDList<T>::insert(size_t pos, T data)
	{
		if (pos > iSize) return -1;
		Node* pNewNode = new Node(data);
		//第一步 查找待插入位置的上一个节点
		int i = 0;
		Node* pNode = pHead;
		while (i++ < pos)
		{
			pNode = pNode->pNext;
		}
		//第二部  连接新节点 和他的后一个节点
		pNewNode->pNext = pNode->pNext;

		pNewNode->pNext->pPrev = pNewNode;
		pNode->pNext = pNewNode; 
		pNewNode->pPrev = pNode;

		iSize++;

		return 0;

	}

	template <typename T>
    int CDList<T>::erase(size_t pos)
	{
		if (pos >= iSize)
		{
			return -1;
		}

		int i = 0;
		Node* pNode = pHead;
		while (i++ < pos)
		{
			pNode = pNode->pNext;
		}
		//第二步，保存待删除节点
		Node* pDelNode = pNode->pNext;
		//第三步，连接待删除节点的前后两个节点
		pNode->pNext = pDelNode->pNext;
		pDelNode->pNext->pPrev = pNode;
		//第四步，删除待删除节点
		delete pDelNode;
		iSize--;
		return 0;



	}

	template <typename T>
	int CDList<T>::modify(size_t pos, T data)
	{
		if (pos >= iSize)
		{
			return -1;
		}
		//查找需要修改的节点，修改后返回
		int i = 0;
		Node* pNode = pHead->pNext;
		while (i++ < pos)
		{
			pNode = pNode->pNext;
		}
		pNode->data = data;
		return 0;
	    
	}

	template <typename T>
	T& CDList<T>::getValue(size_t pos)
	{
		if (pos >= iSize)
		{

		}
		int i = 0;
		Node* pNode = pHead->pNext;
		while (i++ < pos)
		{
			pNode = pNode->pNext;
		}

		return pNode->data;

	}

	template <typename T>
	void CDList<T>::clear()
	{
		if (iSize == 0)
			return;
		
		Node* pDelNode = pHead->pNext;
		while (pDelNode != pTail)
		{
			pHead->pNext = pDelNode->pNext;
			delete pDelNode;
			pDelNode = pHead->pNext;
		}
		pTail->pPrev = pHead;
		return;
	}


	



























	/***********************************************************************************************/











	template <class T> struct list_node_base;
	template <class T> struct list_node;

	template <class T>
	struct node_traits
	{
		typedef list_node_base<T>* base_ptr;
		typedef list_node<T>* node_ptr;
	};



	//list 的节点结构

	template <class T>
	struct list_node_base
	{
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;

		base_ptr prev;  //前一个节点
		base_ptr next;  //下一个节点

		list_node_base() = default;

		node_ptr as_node()
		{
			return static_cast<node_ptr>(self());

		}
		void unlink()
		{
			prev = next = self();
		}

		base_ptr self()
		{
			return static_cast<base_ptr>(&*this);
		}

	};

	template <class T>
	struct list_node : public list_node_base<T>
	{
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;

		T value;//数据域
		list_node() = default;
		list_node(const T& v)
			:value(v)
		{
		}
		//list_node(T&& v) :value(mystl::move(v))
		//{

		//}
		//链表的头节点
		base_ptr as_base()
		{
			return static_cast<base_ptr> (&*this);
		}
		node_ptr self()
		{
			return static_cast<node_ptr>(&*this);
		}

	};

	//list 的迭代器设置
	template <class T>
	struct list_iterator :public mystl::iterator<mystl::bidirectional_iterator_tag, T>
	{
		typedef T       value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;
		typedef list_iterator<T>    self;

		base_ptr node_;   //指向当前节点

		//构造函数
		list_iterator() = default;
		list_iterator(base_ptr x):node_(x){}
		list_iterator(node_ptr x):node_(x->as_base()){}
		list_iterator(const list_iterator& rhs)
			:node_(rhs.node_){}

		//重载操作符
		reference operator*() const { return node_->as_node() -> value; }
		pointer operator->() const { return &(operator*()); }

		self& operator++()
		{
			MYSTL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--()
		{
			MYSTL_DEBUG(node_ != nullptr);
			node_ = node_->prev;
			return *this;

		}
		self operator--(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}

		//重载比较操作符
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

	};

	template <class T>
	struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
	{
		typedef T  value_type;
		typedef const T* pointer;
	    typedef const T& reference;
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;
		typedef list_const_iterator<T>            self;

		base_ptr node_;

		list_const_iterator() = default;
		list_const_iterator(base_ptr x)
			:node_(x){}
		list_const_iterator(node_ptr x)
			:node_(x->as_base()) {}
		list_const_iterator(const list_iterator<T>& rhs)
			:node_(rhs.node_) {}
		list_const_iterator(const list_const_iterator& rhs)
			:node_(rhs.node_) {}

		reference operator*()  const { return node_->as_node()->value; }
		pointer   operator->() const { return &(operator*()); }

		self& operator++()
		{
			MYSTL_DEBUG(node_ != nullptr);
			node_ = node_->next;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++* this;
			return tmp;
		}
		self& operator--()
		{
			MYSTL_DEBUG(node_ != nullptr);
			node_ = node_->prev;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--* this;
			return tmp;
		}

		// 重载比较操作符    `   `   `
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

	};

	//模板类：list 
	//模板参数T嗲表数据类型
	template <class T>
	class list
	{
	public:
		//list 的嵌套性别定义
		typedef mystl::allocator<T>                   allocator_type;
		typedef mystl::allocator<T>                   data_allocator;
		typedef mystl::allocator<list_node_base<T>>   base_allocator;
		typedef mystl::allocator<list_node<T>>        node_allocator;

		typedef typename allocator_type::value_type     value_type;
		typedef typename allocator_type::pointer        pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef list_iterator<T>                         iterator;
		typedef list_const_iterator<T>                   const_iterator;
		typedef mystl::reverse_iterator<iterator>        reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

		typedef typename node_traits<T>::base_ptr        base_ptr;
		typedef typename node_traits<T>::node_ptr        node_ptr;

		allocator_type get_allocator() { return node_allocator(); }

	private:
		base_ptr  node_;  // 指向末尾节点
		size_type size_;  // 大小







	};

}