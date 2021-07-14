
#include "iterator.h"
#include"allocator.h"

#include<iostream>
using namespace std;

namespace mystl
{
	//C����ʵ��
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
		Node* pHead;  //ָ���һ���ڵ㣬 ��������
		Node* pTail;   //���һ���ڵ�  ��������
		size_t iSize;  //���ݸ���
	public:
		CDList():iSize(0),pHead(new Node(T())),pTail(new Node(T())){
			//��pHead��pTail������
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

		//��ɾ�Ĳ�
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
			Node* pNode;//ʹ�����ָ��ָ������ڵ�
			iterator(Node* pTmp) : pNode(pTmp){}
		public:
			
			iterator() :pNode(NULL);
			//���� ++
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
		//��һ�� ���Ҵ�����λ�õ���һ���ڵ�
		int i = 0;
		Node* pNode = pHead;
		while (i++ < pos)
		{
			pNode = pNode->pNext;
		}
		//�ڶ���  �����½ڵ� �����ĺ�һ���ڵ�
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
		//�ڶ����������ɾ���ڵ�
		Node* pDelNode = pNode->pNext;
		//�����������Ӵ�ɾ���ڵ��ǰ�������ڵ�
		pNode->pNext = pDelNode->pNext;
		pDelNode->pNext->pPrev = pNode;
		//���Ĳ���ɾ����ɾ���ڵ�
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
		//������Ҫ�޸ĵĽڵ㣬�޸ĺ󷵻�
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



	//list �Ľڵ�ṹ

	template <class T>
	struct list_node_base
	{
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;

		base_ptr prev;  //ǰһ���ڵ�
		base_ptr next;  //��һ���ڵ�

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

		T value;//������
		list_node() = default;
		list_node(const T& v)
			:value(v)
		{
		}
		//list_node(T&& v) :value(mystl::move(v))
		//{

		//}
		//�����ͷ�ڵ�
		base_ptr as_base()
		{
			return static_cast<base_ptr> (&*this);
		}
		node_ptr self()
		{
			return static_cast<node_ptr>(&*this);
		}

	};

	//list �ĵ���������
	template <class T>
	struct list_iterator :public mystl::iterator<mystl::bidirectional_iterator_tag, T>
	{
		typedef T       value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef typename node_traits<T>::base_ptr base_ptr;
		typedef typename node_traits<T>::node_ptr node_ptr;
		typedef list_iterator<T>    self;

		base_ptr node_;   //ָ��ǰ�ڵ�

		//���캯��
		list_iterator() = default;
		list_iterator(base_ptr x):node_(x){}
		list_iterator(node_ptr x):node_(x->as_base()){}
		list_iterator(const list_iterator& rhs)
			:node_(rhs.node_){}

		//���ز�����
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

		//���رȽϲ�����
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

		// ���رȽϲ�����    `   `   `
		bool operator==(const self& rhs) const { return node_ == rhs.node_; }
		bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

	};

	//ģ���ࣺlist 
	//ģ�����T�Ǳ���������
	template <class T>
	class list
	{
	public:
		//list ��Ƕ���Ա���
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
		base_ptr  node_;  // ָ��ĩβ�ڵ�
		size_type size_;  // ��С







	};

}