#pragma once

//这个头文件包含一个模板类rb_tree


#include <initializer_list>
#include <cassert>
#include "iterator.h"
#include "memory"
#include "type_traits.h"
//#include "exceptdef.h"


namespace mystl {

	//rb tree  节点颜色类型
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

	//rb tree 节点设计
	template <class T>
	struct rb_tree_node_base
	{
		typedef rb_tree_color_type     color_type;
		typedef rb_tree_node_base<T>*  base_ptr;
		typedef rb_tree_node<T>*       node_ptr;

		base_ptr       parent;//父节点
		base_ptr       left;  //左子节点
		base_ptr       right;//右子节点
		color_type     color; //节点颜色

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

		T value;//节点值

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


	//rb  tree 的迭代器设置
	template <class T>
	struct rb_tree_iterator_base :public mystl::iterator<mystl::bidirectional_iterator_tag, T>
	{
		typedef typename rb_tree_traits<T>::base_ptr base_ptr;

		base_ptr node; //指向节点本身

		rb_tree_iterator_base():node(nullptr){}

		//使迭代器前进
		void inc()
		{
			if (node->right != nullptr) //如果有右子节点
			{
				node = rb_tree_min(node->right);//就往右走，然后一直往左子树走到底
			}
			else//没有右子节点
			{
				//如果没有右子节点
				auto y = node->parent;  //找出父节点
				while (y->right == node)//如果现行节点本身是个右子节点
				{
					node = y;    //就一直上溯，直到不为右子节点为止
					y = y->parent;
				}
				if (node->right != y)//若此时的右子节点不等于此时的父节点
					node = y;
			}
		}

		//使迭代器后退
		void dec()
		{
			if (node->parent->parent == node&&node->color == rb_tree_is_red(node))//如果父节点的父节点等于自己且是红节点
			{
				node = node->right;//指向整棵树的max节点
				//以上情况发生于node为header时（亦即node为end()时）
				//注意，header之右子节点即mostright，指向整棵树的max节点

			}
			else if (node->left != nullptr) //如果有左子节点
			{
				node = rb_tree_max(node->left)
			}
			else {    //既非根节点，亦无左子节点
				base_ptr y = node->parent;
				while (node == y->left)//现行节点等于左子节点
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

		//构造函数
		rb_tree_iterator(){}
		rb_tree_iterator(base_ptr x) { node = x; }
		rb_tree_iterator(node_ptr x) { node = x; }
		rb_tree_iterator(const iterator& rhs) { node = rhs.node; }
		rb_tree_iterator(const const_iterator& rhs) { node = rhs.node; }

		//重载操作符
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


		//构造函数
		rb_tree_const_iterator(){}
		rb_tree_const_iterator(base_ptr x) { node = x; }
		rb_tree_const_iterator(node_ptr x) { node = x; }
		rb_tree_const_iterator(const iterator& rhs) { node = rhs.node; }
		rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node; }

		//重载操作符
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
// 左旋，参数一为左旋点，参数二为根节点
	template <class NodePtr>
	void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept
	{
		auto y = x->right;  //y为x的右子节点
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
		{
			//如果x为根节点，让y顶替x为根节点
			root = y;
		}
		else if (rb_tree_is_lchild(x))
		{
			//如果x为左子节点
			x->parent->left = y;
		}
		else
		{
			//如果x为右子节点
			x->parent->right = y;
		}
		//调整x与y的关系
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
	// 右旋，参数一为右旋点，参数二为根节点
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
			//如果x为右子节点
			x->parent->right = y;
		}
		else {
			//如果x为左子节点
			x->parent->left = y;

		}
		y->parent = x;
		x->parent = y;
	}

	// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
	//
	// case 1: 新增节点位于根节点，令新增节点为黑
	// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
	// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
	//         然后令祖父节点为当前节点，继续处理
	// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
	//         让父节点成为当前节点，再以当前节点为支点左（右）旋
	// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
	//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋
	//
	template <class NodePtr>
	void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept
	{
		rb_tree_set_red(x);//新增节点为红色
		while (x != root && rb_tree_is_red(x->parent))//x不是根节点且x的父节点为红色
		{
			//分为两种情况
			//a： x的叔叔节点为红色    b:x的叔叔节点为黑色或不存在
			if (rb_tree_is_lchild(x->parent))
			{
				//如果父子节点是左子节点
				auto uncle = x->parent->right;
				if (uncle != nullptr && rb_tree_is_red(uncle))
				{//父节点和叔叔节点都为红
					rb_tree_set_black(x->parent);
					rb_tree_set_bkack(uncle);
					x = x->parent->parent;
					rb_tree_set_red(x);

				}
				else
				{
					//无叔叔节点或叔叔节点为黑
					if (!rb_tree_is_lchild(x))
					{//case 4  当前节点x为右子节点
						x = x->parent;
						rb_tree_rotate_left(x, root);

					}
					//都转换成case5:当前节点为左子节点
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_right(x->parent->parent, root);
					break;
				}
			}
			else //如果父节点是右子节点，对称处理
			{
				auto uncle = x->parent->parent->left;
				if (uncle != nullptr && rb_tree_is_red(uncle))
				{   //case 3:父节点和叔叔节点都为红
					rb_tree_set_black(x->parent);
					rb_tree_set_black(x->parent->parent->left);
					x = x->parent->parent;
					rb_tree_set_red(x);// 祖父节点涂红
					//此时祖父节点为红，可能会破坏红黑树的性质，令当前节点为祖父节点，继续处理

				}
				else {//无叔叔节点或叔叔节点为黑
					if (rb_tree_is_lchild(x))
					{
						//case 4:当前节点x为左子节点
						x = x->parent;
						rb_tree_rotate_right(x, root);
					}
					//都转换成case5:当前节点为左子节点
					rb_tree_set_black(x->parent);
					rb_tree_set_red(x->parent->parent);
					rb_tree_rotate_left(x->parent->parent, root);
					break;




				}
			}
		}
		rb_tree_set_black(root);  //根节点永远为黑
	}

	//删除节点后是rb tree 重新平衡，参数一为要删除的节点，参数二位根节点，参数三为最小节点，参数四为最大节点

	template <class NodePtr>
	NodePtr rb_tree_erase_reblance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
	{
		//y是可能的替换节点，指向最终要删除的节点
		auto y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_next(z);
		//x为y的一个独子节点或NLL节点
		auto x = y->left != nullptr ? y->left : y->right;
		//xp为x的父节点
		NodePtr xp = nullptr;

		//y != z 说明z有两个非空子节点，此时y指向z右子树的最左节点
		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;

			// 如果 y 不是 z 的右子节点，那么 z 的右子节点一定有左孩子
			if (y != z->right)
			{ // x 替换 y 的位置
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

			// 连接 y 与 z 的父节点 
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
		// y == z 说明 z 至多只有一个孩子
		else
		{
			xp = y->parent;
			if (x)
				x->parent = y->parent;

			// 连接 x 与 z 的父节点
			if (root == z)
				root = x;
			else if (rb_tree_is_lchild(z))
				z->parent->left = x;
			else
				z->parent->right = x;

			// 此时 z 有可能是最左节点或最右节点，更新数据
			if (leftmost == z)
				leftmost = x == nullptr ? xp : rb_tree_min(x);
			if (rightmost == z)
				rightmost = x == nullptr ? xp : rb_tree_max(x);
		}

		// 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
		// 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
		// case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
		// case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
		// case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
		//         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
		// case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
		//         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
		if (!rb_tree_is_red(y))
		{ // x 为黑色时，调整，否则直接将 x 变为黑色即可
			while (x != root && (x == nullptr || !rb_tree_is_red(x)))
			{
				if (x == xp->left)
				{ // 如果 x 为左子节点
					auto brother = xp->right;
					if (rb_tree_is_red(brother))
					{ // case 1
						rb_tree_set_black(brother);
						rb_tree_set_red(xp);
						rb_tree_rotate_left(xp, root);
						brother = xp->right;
					}
					// case 1 转为为了 case 2、3、4 中的一种
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
						// 转为 case 4
						brother->color = xp->color;
						rb_tree_set_black(xp);
						if (brother->right != nullptr)
							rb_tree_set_black(brother->right);
						rb_tree_rotate_left(xp, root);
						break;
					}
				}
				else  // x 为右子节点，对称处理
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
						// 转为 case 4
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

	// 模板类 rb_tree
	// 参数一代表数据类型，参数二代表键值比较类型
	template <class T, class Compare>
	class rb_tree
	{
	public:
		// rb_tree 的嵌套型别定义 

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
		// 用以下三个数据表现 rb tree
		base_ptr    header_;      // 特殊节点，与根节点互为对方的父节点
		size_type   node_count_;  // 节点数
		key_compare key_comp_;    // 节点键值比较的准则
	private:
		//以下是哪个函数用于取得header的成员
		base_ptr& root() const { return header_->parent; }
		base_ptr& leftmost() const { return header_->left; }
		base_ptr& rightmost() const { return header_->right; }
	public:
		//构造  复制  析构函数
		rb_tree() { rb_tree_init(); }
		rb_tree(const rb_tree& rhs);
		rb_tree(const rb_tree&& rhs) noexcept;
		rb_tree& operator=(const rb_tree& rhs);
		rb_tree& operator=(rb_tree&& rhs);

		~rb_tree() { clear(); }


	public:
		//迭代器相关操作
		iterator begin() noexcept { return leftmost(); }//RB树的起点为 最左（最小）节点处
		const_iterator begin() const noexcept { return leftmost(); }
		iterator end() noexcept { return header_; }//RB树的终点为header所指处
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

		//容量相关操作
		bool empty()  const noexcept { return node_count_ == 0;}
		size_type size() const noexcept { return node_count_; }
		size_type max_size() const noexcept { return static_cast<size_type>(-1); }

		//插入删除相关操作
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