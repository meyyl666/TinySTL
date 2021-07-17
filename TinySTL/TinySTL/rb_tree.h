#pragma once

//���ͷ�ļ�����һ��ģ����rb_tree


#include <initializer_list>
#include <cassert>
#include "iterator.h"
#include "memory"
#include "type_traits.h"
//#include "exceptdef.h"


namespace mystl {

	//rb tree  �ڵ���ɫ����
	typedef bool rb_tree_color_type;

	static constexpr rb_tree_color_type rb_tree_red = false;
	static constexpr rb_tree_color_type rb_tree_black = true;

	//forward declaration

	template <class T> struct rb_tree_node_base;
	template <class T> struct rb_tree_node;

	template <class T> struct rb_tree_iterator;
	template <class T> struct rb_tree_const_iterator;

	//rb tree value traits

	template <class T,bool>
	struct rb_tree_value_traits_imp
	{
		typedef T key_value;
		typedef T mapped_type;
		typedef T value_type;

		template <class Ty>
		static const key_value& get_key(const Ty& value)
		{
			return value;
		}

		template <class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
		
	};

	template <class T>
	struct rb_tree_value_traits_imp<T, true>
	{
		typedef typename std::remove_cv<typename T::first_type>::type key_type;
		typedef typename T::second_type                               mappef_type;
		typedef typename T                                            value_type;

		template <class Ty>
		static const key_value& get_key(const Ty& value)
		{
			return value.first;
		}

		template <class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value;
		}
	};

	template <class T>
	struct rb_tree_value_traits
	{
		static constexpr bool is_map = mystl::is_pair<T>::value;
		typedef rb_tree_value_traits_imp<T, is_map> value_traits_type;

		typedef typename value_traits_type::key_type    key_type;
		typedef typename value_traits_type::mapped_type mapped_type;
		typedef typename value_traits_type::value_type  value_type;


		template <class Ty>
		static const key_value& get_key(const Ty& value)
		{
			return value_traits_type::get_key(value);
		}
		template <class Ty>
		static const value_type& get_value(const Ty& value)
		{
			return value_traits_type::get_value(value);
		}


	};

	//rb_tree_node_traits

	template <class T>
	struct rb_tree_node_traits
	{
		typedef rb_tree_color_type       color_type;

		typedef rb_tree_value_traits<T>   value_traits;
		typedef typename value_traits::key_type   key_type;
		typedef typename value_traits::mapped_type mapped_type;
		typedef typename value_traits::value_type    value_type;

		typedef rb_tree_node_base<T>*        base_ptr;
		typedef rb_tree_node<T>*             node_ptr; 
	};

	//rb tree �ڵ����
	template <class T>
	struct rb_tree_node_base
	{
		typedef rb_tree_color_type     color_type;
		typedef rb_tree_node_base<T>*  base_ptr;
		typedef rb_tree_node<T>*       node_ptr;

		base_ptr       parent;//���ڵ�
		base_ptr       left;  //���ӽڵ�
		base_ptr       right;//���ӽڵ�
		color_type     color; //�ڵ���ɫ

		base_ptr get_base_ptr()
		{
			return &*this;
		}
		node_ptr get_node_ptr()
		{
			return reinterpret_cast<node_ptr>(&*this);
		}

		node_ptr& get_node_ref()
		{
			return reinterpret_cast<node_ptr&>(*this);
		}
	};

	template <class T>
	struct rb_tree_node :public rb_tree_node_base<T>
	{
		typedef rb_tree_node_base<T>* base_ptr;
		typedef rb_tree_node<T>* node_ptr;

		T value;//�ڵ�ֵ

		base_ptr get_base_ptr()
		{
			return static_cast<base_ptr>(&*this);
		}
		node_ptr get_node_ptr()
		{
			return &*this;
		}

	};

	//rb tree traits
	template <class T>
	struct rb_tree_traits
	{
		typedef rb_tree_value_traits<T>      value_traits;

		typedef typename value_traits::key_value   key_type;
		typedef typename value_traits::mapped_type mapped_type;
		typedef typename value_traits::value_type  value_type;

		typedef value_type*          pointer;
		typedef value_type&          reference; 
		typedef const value_type*    const_pointer;
		typedef const value_type&    const_reference;

		typedef rb_tree_node_base<T>  base_type;
		typedef rb_tree_node<T>       node_type;

		typedef base_type*            base_ptr;
		typedef node_type*            node_ptr;

	};


	//rb  tree �ĵ���������
	template <class T>
	struct rb_tree_iterator_base :public mystl::iterator<mystl::bidirectional_iterator_tag, T>
	{
		typedef typename rb_tree_traits<T>::base_ptr base_ptr;

		base_ptr node; //ָ��ڵ㱾��

		rb_tree_iterator_base():node(nullptr){}

		//ʹ������ǰ��
		void inc()
		{
			if (node->right != nullptr) //��������ӽڵ�
			{
				node = rb_tree_min(node->right);//�������ߣ�Ȼ��һֱ���������ߵ���
			}
			else//û�����ӽڵ�
			{
				//���û�����ӽڵ�
				auto y = node->parent;  //�ҳ����ڵ�
				while (y->right == node)//������нڵ㱾���Ǹ����ӽڵ�
				{
					node = y;    //��һֱ���ݣ�ֱ����Ϊ���ӽڵ�Ϊֹ
					y = y->parent;
				}
				if (node->right != y)//����ʱ�����ӽڵ㲻���ڴ�ʱ�ĸ��ڵ�
					node = y;
			}
		}

		//ʹ����������
		void dec()
		{
			if (node->parent->parent == node&&node->color == rb_tree_is_red(node))//������ڵ�ĸ��ڵ�����Լ����Ǻ�ڵ�
			{
				node = node->right;//ָ����������max�ڵ�
				//�������������nodeΪheaderʱ���༴nodeΪend()ʱ��
				//ע�⣬header֮���ӽڵ㼴mostright��ָ����������max�ڵ�

			}
			else if (node->left != nullptr) //��������ӽڵ�
			{
				node = rb_tree_max(node->left)
			}
			else {    //�ȷǸ��ڵ㣬�������ӽڵ�
				base_ptr y = node->parent;
				while (node == y->left)//���нڵ�������ӽڵ�
				{
					node = y;
					y = y ->parent;
				}
				node = y;
			}

		}

		bool operator==(const rb_tree_iterator_base& rhs) { return node == rhs.node; }
		bool operator!=(const rb_tree_iterator_base& rhs) { return node != rhs.node; }
	};

	template <class T >
	struct rb_tree_iterator: public rb_tree_iterator_base<T>
	{
		typedef rb_tree_traits<T>         tree_traits;
		typedef typename tree_traits::value_type  value_type;
		typedef typename tree_traits::pointer     pointer;
		typedef typename tree_traits::reference   reference;
		typedef typename tree_traits::base_ptr    base_ptr;
		typedef typename tree_traits::node_ptr    node_ptr;

		typedef rb_tree_iterator<T>        iterator;
		typedef rb_tree_const_iterator<T>  const_iterator;
		typedef iterator                   self;

		using rb_tree_iterator_base<T>::node;

		//���캯��
		rb_tree_iterator(){}
		rb_tree_iterator(base_ptr x) { node = x; }
		rb_tree_iterator(node_ptr x) { node = x; }
		rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
		rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

		//���ز�����
		reference operator*() const { return node->get_node_ptr()->value; }
		pointer operator->() const { return &(operator*()); }

		self& operator++()
		{
			this->inc();
			return *this;
		}
		self operator++(int)
		{
			self tmp(*this);
			this->inc();
			return tmp;
		}

		self& operator--()
		{
			this->dec();
			return *this;
		}
		self operator--(int)
		{
			self tmp(*this);
			this->dec();
			return tmp;
		}
	};

	template <class T>
	struct rb_tree_const_iterator :public rb_tree_iterator_base<T>
	{
		typedef rb_tree_traits<T>                 tree_traits;

		typedef typename tree_traits::value_type  value_type;
		typedef typename tree_traits::const_pointer  pointer;
		typedef typename tree_traits::const_reference reference;
		typedef typename tree_traits::base_ptr      base_ptr;
		typedef typename tree_traits::node_ptr      node_ptr;

		typedef rb_tree_iterator<T>         iterator;
		typedef rb_tree_const_iterator<T>   const_iterator;
		typedef const_iterator              self;

		using rb_tree_iterator_base<T>::node;


		//���캯��
		rb_tree_const_iterator(){}
		rb_tree_const_iterator(base_ptr x) { node = x; }
		rb_tree_const_iterator(node_ptr x) { node = x; }
		rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; }
		rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; }

		//���ز�����
		reference operator*() const { return node->get_node_ptr()->value; }
		pointer operator->() const { return &(operator*()); }

		self& operator++()
		{
			this->inc();
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			this->inc();
			return tmp;
		}

		self& operator--()
		{
			this->dec();
			return *this;
		}

		self operator--()
		{
			self tmp(*this);
			this->dec();
			return tmp;
		}

	};

	//tree alorithm
	

	template <class NodePtr> 
	NodePtr rb_tree_min(NodePtr x) noexcept
	{
		while (x->left != nullptr)
		{
			x = x->left;
		}
		return x;

	}
	template <class NodePtr>
	NodePtr rb_tree_max(NodePtr x) noexcept
	{
		while (x->right != nullptr)
		{
			x = x->right;

		}
		return x;
	}

	template <class NodePtr>
	bool rb_tree_is_lchild(NodePtr node) noexcept
	{
		return node == node->parent->left;
	}

	template <class NodePtr>
	bool rb_tree_is_red(NodePtr node) noexcept
	{
		return node->color == rb_tree_red;
	}

	template <class NodePtr>
	bool rb_tree_is_black(NodePtr node) noexcept
	{
		return node->color == rb_tree_black;
	}

	template <class NodePtr>
	void rb_tree_set_red(NodePtr node) noexcept
	{
		node->color = rb_tree_red;
	}

	template <class NodePtr>
	void rb_tree_set_black(NodePtr node) noexcept
	{
		node->color = rb_tree_black;
	}

	template <class NodePtr >
	NodePtr rb_tree_next(NodePtr node) noexcept
	{
		if (node->right != nullptr)
			return rb_tree_min(node->right);
		while (!rb_tree_is_lchild(node))
			node = node->parent;
		return node->parent;
	}
/*---------------------------------------*\
|       p                         p       |
|      / \                       / \      |
|     x   d    rotate left      y   d     |
|    / \       ===========>    / \        |
|   a   y                     x   c       |
|      / \                   / \          |
|     b   c                 a   b         |
\*---------------------------------------*/
// ����������һΪ�����㣬������Ϊ���ڵ�
	template <class NodePtr>
	void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept
	{
		auto y = x->right;  //yΪx�����ӽڵ�
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
		{
			//���xΪ���ڵ㣬��y����xΪ���ڵ�
			root = y;
		}
		else if (rb_tree_is_lchild(x))
		{
			//���xΪ���ӽڵ�
			x->parent->left = y;
		}
		else
		{
			//���xΪ���ӽڵ�
			x->parent->right = y;
		}
		//����x��y�Ĺ�ϵ
		y->left = x;
		x->parnet = y;
	}

	/*----------------------------------------*\
	|     p                         p          |
	|    / \                       / \         |
	|   d   x      rotate right   d   y        |
	|      / \     ===========>      / \       |
	|     y   a                     b   x      |
	|    / \                           / \     |
	|   b   c                         c   a    |
	\*----------------------------------------*/
	// ����������һΪ�����㣬������Ϊ���ڵ�
	template <class NodePtr>
	void rb_tree_rotate_right(NodePtr x, NodePtr& root) noexcept
	{
		auto y = x->left;
		x->left = y->right;
		if (y->right)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (!rb_tree_is_lchild(x))
		{
			//���xΪ���ӽڵ�
			x->parent->right = y;
		}
		else {
			//���xΪ���ӽڵ�
			x->parent->left = y;

		}
		y->parent = x;
		x->parent = y;
	}

	// ����ڵ��ʹ rb tree ����ƽ�⣬����һΪ�����ڵ㣬������Ϊ���ڵ�
	//
	// case 1: �����ڵ�λ�ڸ��ڵ㣬�������ڵ�Ϊ��
	// case 2: �����ڵ�ĸ��ڵ�Ϊ�ڣ�û���ƻ�ƽ�⣬ֱ�ӷ���
	// case 3: ���ڵ������ڵ㶼Ϊ�죬��ڵ������ڵ�Ϊ�ڣ��游�ڵ�Ϊ�죬
	//         Ȼ�����游�ڵ�Ϊ��ǰ�ڵ㣬��������
	// case 4: ���ڵ�Ϊ�죬����ڵ�Ϊ NIL ���ɫ�����ڵ�Ϊ���ң����ӣ���ǰ�ڵ�Ϊ�ң��󣩺��ӣ�
	//         �ø��ڵ��Ϊ��ǰ�ڵ㣬���Ե�ǰ�ڵ�Ϊ֧�����ң���
	// case 5: ���ڵ�Ϊ�죬����ڵ�Ϊ NIL ���ɫ�����ڵ�Ϊ���ң����ӣ���ǰ�ڵ�Ϊ���ң����ӣ�
	//         �ø��ڵ��Ϊ��ɫ���游�ڵ��Ϊ��ɫ�����游�ڵ�Ϊ֧���ң�����
	//
	template <class NodePtr>
	void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept
	{
		rb_tree_set_red(x);//�����ڵ�Ϊ��ɫ
		while (x != root && rb_tree_is_red(x->parent))//x���Ǹ��ڵ���x�ĸ��ڵ�Ϊ��ɫ
		{
			//��Ϊ�������
			//a�� x������ڵ�Ϊ��ɫ    b:x������ڵ�Ϊ��ɫ�򲻴���
			if (rb_tree_is_lchild(x->parent))
			{
				//������ӽڵ������ӽڵ�
				auto uncle = x->parent->right;
				if (uncle != nullptr && rb_tree_is_red(uncle))
				{//���ڵ������ڵ㶼Ϊ��
					rb_tree_set_black(x->parent);
					rb_tree_set_bkack(uncle);
					x = x->parent->parent;
					rb_tree_set_red(x);

				}
				else
				{
					//������ڵ������ڵ�Ϊ��
					if (!rb_tree_is_lchild(x))
					{//case 4  ��ǰ�ڵ�xΪ���ӽڵ�
						x = x->parent;
						rb_tree_rotate_left(x, root);

					}
					//��ת����case5:��ǰ�ڵ�Ϊ���ӽڵ�
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_right(x->parent->parent, root);
					break;
				}
			}
			else //������ڵ������ӽڵ㣬�Գƴ���
			{
				auto uncle = x->parent->parent->left;
				if (uncle != nullptr && rb_tree_is_red(uncle))
				{   //case 3:���ڵ������ڵ㶼Ϊ��
					rb_tree_set_black(x->parent);
					rb_tree_set_black(x->parent->parent->left);
					x = x->parent->parent;
					rb_tree_set_red(x);// �游�ڵ�Ϳ��
					//��ʱ�游�ڵ�Ϊ�죬���ܻ��ƻ�����������ʣ��ǰ�ڵ�Ϊ�游�ڵ㣬��������

				}
				else {//������ڵ������ڵ�Ϊ��
					if (rb_tree_is_lchild(x))
					{
						//case 4:��ǰ�ڵ�xΪ���ӽڵ�
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					//��ת����case5:��ǰ�ڵ�Ϊ���ӽڵ�
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_left(x->parent->parent, root);
					break;




				}
			}
		}
		rb_tree_set_black(root);  //���ڵ���ԶΪ��
	}

	//ɾ���ڵ����rb tree ����ƽ�⣬����һΪҪɾ���Ľڵ㣬������λ���ڵ㣬������Ϊ��С�ڵ㣬������Ϊ���ڵ�

	template <class NodePtr>
	NodePtr rb_tree_erase_reblance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
	{
		//y�ǿ��ܵ��滻�ڵ㣬ָ������Ҫɾ���Ľڵ�
		auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);
		//xΪy��һ�����ӽڵ��NLL�ڵ�
		auto x = y->left != nullptr ? y->left : y->right;
		//xpΪx�ĸ��ڵ�
		NodePtr xp = nullptr;

		//y != z ˵��z�������ǿ��ӽڵ㣬��ʱyָ��z������������ڵ�
		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;

			// ��� y ���� z �����ӽڵ㣬��ô z �����ӽڵ�һ��������
			if (y != z->right)
			{ // x �滻 y ��λ��
				xp = y->parent;
				if (x != nullptr)
					x->parent = y->parent;

				y->parent->left = x;
				y->right = z->right;
				z->right->parent = y;
			}
			else
			{
				xp = y;
			}

			// ���� y �� z �ĸ��ڵ� 
			if (root == z)
				root = y;
			else if (rb_tree_is_lchild(z))
				z->parent->left = y;
			else
				z->parent->right = y;
			y->parent = z->parent;
			mystl::swap(y->color, z->color);
			y = z;
		}
		// y == z ˵�� z ����ֻ��һ������
		else
		{
			xp = y->parent;
			if (x)
				x->parent = y->parent;

			// ���� x �� z �ĸ��ڵ�
			if (root == z)
				root = x;
			else if (rb_tree_is_lchild(z))
				z->parent->left = x;
			else
				z->parent->right = x;

			// ��ʱ z �п���������ڵ�����ҽڵ㣬��������
			if (leftmost == z)
				leftmost = x == nullptr ? xp : rb_tree_min(x);
			if (rightmost == z)
				rightmost = x == nullptr ? xp : rb_tree_max(x);
		}

		// ��ʱ��y ָ��Ҫɾ���Ľڵ㣬x Ϊ����ڵ㣬�� x �ڵ㿪ʼ������
		// ���ɾ���Ľڵ�Ϊ��ɫ����������û�б��ƻ����������������������x Ϊ���ӽڵ�Ϊ������
		// case 1: �ֵܽڵ�Ϊ��ɫ����ڵ�Ϊ�죬�ֵܽڵ�Ϊ�ڣ��������ң�������������
		// case 2: �ֵܽڵ�Ϊ��ɫ���������ӽڵ㶼Ϊ��ɫ�� NIL�����ֵܽڵ�Ϊ�죬���ڵ��Ϊ��ǰ�ڵ㣬��������
		// case 3: �ֵܽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ�� NIL�����ӽڵ�Ϊ��ɫ�� NIL��
		//         ���ֵܽڵ�Ϊ�죬�ֵܽڵ�����ӽڵ�Ϊ�ڣ����ֵܽڵ�Ϊ֧���ң���������������
		// case 4: �ֵܽڵ�Ϊ��ɫ�����ӽڵ�Ϊ��ɫ�����ֵܽڵ�Ϊ���ڵ����ɫ�����ڵ�Ϊ��ɫ���ֵܽڵ�����ӽڵ�
		//         Ϊ��ɫ���Ը��ڵ�Ϊ֧�����ң������������ʵ�����ɣ��㷨����
		if (!rb_tree_is_red(y))
		{ // x Ϊ��ɫʱ������������ֱ�ӽ� x ��Ϊ��ɫ����
			while (x != root && (x == nullptr || !rb_tree_is_red(x)))
			{
				if (x == xp->left)
				{ // ��� x Ϊ���ӽڵ�
					auto brother = xp->right;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_left(xp, root);
						brother = xp->right;
					}
					// case 1 תΪΪ�� case 2��3��4 �е�һ��
					if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
						(brother->right == nullptr || !rb_tree_is_red(brother->right)))
					{ // case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->right == nullptr || !rb_tree_is_red(brother->right))
						{ // case 3
							if (brother->left != nullptr)
								rb_tree_set_black(brother->left);
							rb_tree_set_red(brother);
							rb_tree_rotate_right(brother, root);
							brother = xp->right;
						}
						// תΪ case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->right != nullptr)
							rb_tree_set_black(brother->right);
						rb_tree_rotate_left(xp, root);
						break;
					}
				}
				else  // x Ϊ���ӽڵ㣬�Գƴ���
				{
					auto brother = xp->left;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_right(xp, root);
						brother = xp->left;
					}
					if ((brother->left == nullptr || !rb_tree_is_red(brother->left)) &&
						(brother->right == nullptr || !rb_tree_is_red(brother->right)))
					{ // case 2
						rb_tree_set_red(brother);
						x = xp;
						xp = xp->parent;
					}
					else
					{
						if (brother->left == nullptr || !rb_tree_is_red(brother->left))
						{ // case 3
							if (brother->right != nullptr)
								rb_tree_set_black(brother->right);
							rb_tree_set_red(brother);
							rb_tree_rotate_left(brother, root);
							brother = xp->left;
						}
						// תΪ case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->left != nullptr)
							rb_tree_set_black(brother->left);
						rb_tree_rotate_right(xp, root);
						break;
					}
				}
			}
			if (x != nullptr)
				rb_tree_set_black(x);
		}
		return y;
	}

	// ģ���� rb_tree
	// ����һ�����������ͣ������������ֵ�Ƚ�����
	template <class T, class Compare>
	class rb_tree
	{
	public:
		// rb_tree ��Ƕ���ͱ��� 

		typedef rb_tree_traits<T>                        tree_traits;
		typedef rb_tree_value_traits<T>                  value_traits;

		typedef typename tree_traits::base_type          base_type;
		typedef typename tree_traits::base_ptr           base_ptr;
		typedef typename tree_traits::node_type          node_type;
		typedef typename tree_traits::node_ptr           node_ptr;
		typedef typename tree_traits::key_type           key_type;
		typedef typename tree_traits::mapped_type        mapped_type;
		typedef typename tree_traits::value_type         value_type;
		typedef Compare                                  key_compare;

		typedef mystl::allocator<T>                      allocator_type;
		typedef mystl::allocator<T>                      data_allocator;
		typedef mystl::allocator<base_type>              base_allocator;
		typedef mystl::allocator<node_type>              node_allocator;

		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef rb_tree_iterator<T>                      iterator;
		typedef rb_tree_const_iterator<T>                const_iterator;
		typedef mystl::reverse_iterator<iterator>        reverse_iterator;
		typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

		allocator_type get_allocator() const { return node_allocator(); }
		key_compare    key_comp()      const { return key_comp_; }

	private:
		// �������������ݱ��� rb tree
		base_ptr    header_;      // ����ڵ㣬����ڵ㻥Ϊ�Է��ĸ��ڵ�
		size_type   node_count_;  // �ڵ���
		key_compare key_comp_;    // �ڵ��ֵ�Ƚϵ�׼��
	private:
		//�������ĸ���������ȡ��header�ĳ�Ա
		base_ptr& root() const { return header_->parent; }
		base_ptr& leftmost() const { return header_->left; }
		base_ptr& rightmost() const { return header_->right; }
	public:
		//����  ����  ��������
		rb_tree() { rb_tree_init(); }
		rb_tree(const rb_tree& rhs);
		rb_tree(const rb_tree&& rhs) noexcept;
		rb_tree& operator=(const rb_tree& rhs);
		rb_tree& operator=(rb_tree&& rhs);

		~rb_tree() { clear(); }


	public:
		//��������ز���
		iterator begin() noexcept { return leftmost(); }//RB�������Ϊ ������С���ڵ㴦
		const_iterator begin() const noexcept { return leftmost(); }
		iterator end() noexcept { return header_; }//RB�����յ�Ϊheader��ָ��
		const_iterator end() { return header_; }

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend() noexcept
		{
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const noexcept
		{
			return begin();
		}
		const_iterator cend() const noexcept
		{
			return end();
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		//������ز���
		bool empty()  const noexcept { return node_count_ == 0;}
		size_type size() const noexcept { return node_count_; }
		size_type max_size() const noexcept { return static_cast<size_type>(-1); }

		//����ɾ����ز���
		//emplace
		template <class ...Args>
		iterator emplace_multi(Args&& ...args);

		template <class ...Args>
		mystl::pair<iterator, bool> emplace_unique(Args&& ....args);

		template <class ...Args >
		iterator emplace_multi_use_hint(iterator hint, Args&& ...args);

		template <class ...Args >
		iterator  emplace_unique_use_hint(iterator hint, Args&& ...args);


		//insert

		iterator insert_multi(const value_type& value);
		iterator insert_multi(value_type&& value)
		{
			return emplace_multi(mystl::move(value));
		}
		iterator insert_multi(iterator hint, const value_type& value)
		{
			return emplace_multi_use_hint(hint, value);
		}
		iterator insert_multi(iterator hint, value_type&& value)
		{
			return emplace_multi_use_hint(hint, mystl::move(value));
		}

		template <class InputIterator>
		void insert_multi(InputIterator first, InputIterator last)
		{
			size_type n = mystl::distance(first, last);
			THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
			for (; n > 0; --n, ++first)
				insert_multi(end(), *first);
		}

		mystl::pair<iterator, bool> insert_unique(const value_type& value);
		mystl::pair<iterator,bool> insert_unque(value_type&& value)
		{
			return emplace_unique(mystl::move(value));
		}
		iterator insert_unique(iterator hint, const value_type& value)
		{
			return emplace_unique_use_hint(hint, value);
		}
		iterator insert_unique(iterator hint, value_type&& value)
		{
			return emplace_unique_use_hint(hint, value);
		}

		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{
			size_type n = mystl::distance(first, last);
			THROW_LENGTH_ERROR_IF(node_count_ > max_size() - n, "rb_tree<T, Comp>'s size too big");
			for (; n > 0; --n, ++first)
				insert_unique(end(), *first);
		}















	};

}