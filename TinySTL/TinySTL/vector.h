#pragma once
#ifndef MYFIRST_VECTOR_H
#define MYFIRST_VECTOR_H

#include <initializer_list>
#include "allocator.h"
#include "iterator.h"


namespace mystl {
	template<class T>
	class vector {
	public:
		typedef mystl::allocator<T>      allocator_type;
		typedef mystl::allocator<T>      data_allocator;

		typedef typename allocator_type::value_type      value_type;
		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef typename allocator_type::reference       reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef value_type* iterator;      //vector的迭代器是普通指针
		typedef const value_type* const_iterator;
		typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

		allocator_type get_allocator() { return data_allocator();}

	private:
		iterator begin_;    //表示目前使用空间的头部
		iterator end_;      //表示目前使用空间的尾部
		iterator cap_;      ///表示目前储存空间的尾部

	public:
		//构造 复制 移动 析构函数
		vector() noexcept
		{
			try_init();
		}
		explicit vector(size_type n)
		{
			fill_init(n, value_type());
		}

		vector(size_tyoe n,const value_type& value)
		{
			fill_init(n, value);
		}

		template <class Iter,typename std::enable_if<
			mystl::is_input_iterator<Iter>::value,int>::type = 0>
			vector(Iter first, Iter last)
		{
			MYSTL_DEBUG(!(last < first));
			range_init(first, last);
		}

		vector(const vector& rhs)
		{
			range_init(rhs.begin_, rhs.end_);
		}

		vector(vector&& rhs) noexcept:begin_(rhs.begin_),end_(rhs.end_),cap_(rhs.cap_)
		{
			rhs.begin_ = nullptr;
			rhs.end_ = nullptr;
			rhs.cap_ = nullptr;
		}

		vector(std::initializer_list<value_type> ilist)
		{
			range_init(ilist.begin(), ilist.end());
		}

		vector& operator=(const vector& rhs);
		vector& operator=(vector& rhs) noexcept;

		vector& operator=(std::initializer_list<value_type> ilist)
		{
			vector tmp(ilist.begin(), ilist.end());
			swap(tmp);
			return *this
		}

		~vector()
		{
			destory_and_recover(begin_, end_, cap_ - begin_);
			begin_ = end_ = cap_ = nullptr;
		}

	public:
		//迭代器相关操作
		iterator begin() noexcept
		{
			return begin_;
		}
		const_iterator begin() const noexcept {
			return begin_;
		}
		iterator end() noexcept
		{
			return end_;
		}
		const_iterator end() const noexcept
		{
			return end_;
		}

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
		const_reverse_iterator rend() const noexcept
		{
			return reverse_iterator(end());
		}
		const_iterator cbrgin() const noexcept
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
		bool empty() const noexcept
		{
			return begin_ == end_;
		}
		size_type size() const noexcept
		{
			return static_cast<size_type>(end_ - begin_);
		}
		size_type max_size() const noexcept
		{
			return static_cast<size_type>(-1) / sizeof(T);
		}
		size_type capacity() const noexcept
		{
			return static_cast<size_type>(cap_ - begin_);
		}
		void reserve(size_type n);
		void shrink_to_fit();

		//访问元素相关操作  返回元素的引用
		reference operator[](size_type n)
		{
			MYSTL_DEBUG(n < size());
			return *(brgin_ + n);
		}
		const_reference operator[](size_type n)
		{
			MYSTL_DEBUG(n < size());
			return *(begin_ + n);
		}

		reference at(size_type n)
		{
			THROW_OUT_OF_RANGE_IF(!((n < size()), "vector<T>::at() subscript out of range");
			return (*this)[n];
		}
		const_reference at(size_type n) const
		{
			THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
			return (*this)[n];
		}

		reference front()
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}
		const_reference front() const
		{
			MYSTL_DEBUG(!empty());
			return *begin_;
		}
		reference back()
		{
			MYSTL_DEBUG(!empty());
			return *end_;

		}
		const_reference back() const
		{
			MYSTL_DEBUG(!empty());
			return *(end_ - 1);

		}

		pointer data() noexcept { return begin_; }
		const_pointer data()  const noexcept { return begin_; }

		//修改容器相关操作

		void assign (size_type n,const value_type& value)
		{
			fill_assign(n, value);
		}
		template <class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type = 0>
		void assign(Iter first, Iter last)
		{
			MYSTL_DEBUG(!(last < first));
			copy_assign(first, last, iterator_category(first));

		}

		//assign
		void assign(size_type n,const value_type& value)
		{
			fill_assign(n, value);
		}
		 template <class Iter,typename std::enable_if<mystl::is_input_iterator<Iter>::value,int>::type = 0>
		 void assign(Iter first, Iter last)
		 {
			 MYSTL_DEBUG(!(last < first));
			 copy_assign(first,last,iterator_category(first))
		 }

		void assign(std::initializer_list<value_type> il)
		{}

		//emplace /emplace_back
		template <class... Args>
		iterator emplace(const_iterator pos, Args&& ... args);
		
		template <class... Args>
		void emplace_back(Args&& ...args);

		//push_back / pop_back
		void push_back(const value_type& value);
		void push_back(value_type&& value)
		{
			emplace_back(mystl::move(value));
		}
		void pop_back();







   private:
	
		//helper functions
		//initialize/destory
		void   try_init() noexcept;
		void      fill_init(size_type n, const value_type& value);

		void init_space(size_type size, size_type cap);

		template <class Iter>
		void range_init(Iter first, Iter last);

		void destory_and_recover(iterator first, iterator last, size_type n);

		//assign
		void fill_assign(size_type n, const value_type& value);

		template <class IIter>
		void copy_assign(IIter first,IIter last, input_iterator_tag);

		template <class FIter>
		void copy_assign(FIter first,FIter last, forward_iterator_tag);


		//swap
		void swap(vector& rhs) noexcept;


		// reallocate

		template <class... Args>
		void      reallocate_emplace(iterator pos, Args&& ...args);
		void      reallocate_insert(iterator pos, const value_type& value);

		// calculate the growth size
		size_type get_new_cap(size_type add_size);


	



	










	};





	//helper function 
	//try_init 函数 若分配失败则忽略，不抛出异常
	template <class T>
	void vector<T>::try_init() noexcept
	{
		try {
			begin_ = data_allocator::allocate(16);
			end_ = begin_;
			cap_ = begin_ + 16;
		}
		catch (...)
		{
			begin_ = nullptr;
			end_ = nullptr;
			cap_ = nullptr;

		}
	}






	//init_space 函数
	template <class T>
	void vector<T>::init_space(size_type size, size_type cap)
	{
		try {
			begin_ = data_allocator::allocate(cap);
			end_ = begin_ + size;
			cap_ = begin_ + cap; 
		}
		catch(...){
			begin_ = nullptr;
			end_ = nullptr;
			cap_ = nullptr;
			throw;

		}
	}


	//fill_init 函数
	template <class T>
	void vector<T>::
		fill_init(size_type n, const value_type& value)
	{
		const size_type init_size = mystl::max(static_cast<size_type>(16), n);
		init_space(n, init_size);
		mystl::uninitialized_fill_n(begin_, n, value);

		
	}


	//range_init 函数
	template <class T>
	template <class Iter>
	void vector<T>::range_init(Iter first, Iter last)
	{
		const size_type init_size = mystl::max(static_cast<size_type>(last - first),
			static_cast <size_type>(16));
		init_space(static_cast<size_type>(last - first), init_size);
		mystl::uninitialized_copy(first, last, begin_);
	}

	//destory_and_recover 函数
	template <class T>
	void vector<T>::destory_and_recover(iterator first, iterator last, size_type n)
	{
		data_allocator::destory(first, last);
		data_allocator::deallocate(first, n);
	}

	//预留空间大小，当原容量小鱼要求大小时，重新分配
	template <class T>
	void vector<T>::reserve(size_type n)
	{
		if(capacity()<n)
		{
			THROE_LEMNGTH_ERROR_IF(n > max_size, 'n can not larger than max_size() in vector<T>::reserve(n)');
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);    //重新申请一块内存
			mystl::uninitialized_move(begin_, end_, tmp);  //将原来内存中的元素移动到新申请的内存
			data_allocator::deallocate(begin_, cap_ - begin_); //消回原来的内存
			begin_ = tmp;           //迭代器归位
			end_ = tmp + old_size;
			cap_ = begin_ + n;

		}

	}
	//放弃多余容量
	template <class T>
	void vector<T>::shrink_to_fit()
	{
		if (end_ < cap_)
		{
			reinsert(size());
		}
	}

	//在pos位置就地构造元素，避免额外的复制或者开销
	template <class T>
	template <class ...Args>
	typename vector<T>::iterator
		vector<T>::emplace(const_iterator pos, Args&& ...args)
	{
		MYSTL_DEBUG(pos >= begin() && pos <= end());
		iterator xpos = const_cast<iterator>(pos);
		const size_type n = xpos - begin_;
		if (end_ != cap_ && xpos == end_)
		{
			data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
			++end_;
		}
		else if (end_ != cap_)
		{
			auto new_end = end_;
			data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
			++new_end;
			mystl::copy_backward(xpos, end_ - 1, end_);
			*xpos = value_type(mystl::forward<Args>(args)...);
		}
		else
		{
			reallocate_emplace(end_, mystl::forward<Args>(args)...);
		}
		return begin() + n;
	}

	//在尾部插入元素
	template <class T>
	void vector<T>::push_back(const value_type& value)
	{
		if (end_ < cap_)
		{
			data_allocator::construct(mystl::address_of(*end_), value);
			++end_;
		}
		else
		{
			//因为保证原空间后有足够的大小，故重新开阔空间
			reallocate_insert(end_, value);
		}
	}

	//重新分配空间并在pos出插入元素
	template <class T>
	void vector<T>::reallocate_insert(iterator pos, const value_type& value)
	{ 
		const auto new_size = get_new_cap(1);      //需要分配空间的大小
		auto new_begin = data_allocator::allocate(new_size);   //新分配空间的起始位置
		auto new_end = new_begin;
		const value_type& value_copy = value;

		try {
			//将原容器的开始位置到pos处的元素拷贝到新内存上
			new_end = mystl::uninitialized_move(begin_, pos, new_begin);
			data_allocator::construct(mystl::address_of(*new_end), value_copy);
			++new_end;
			new_end = mystl::uninitialized_move(pos, end_, new_end);
		}
		catch (...)
		{
			data_allocator::deallocate(new_begin, new_size);
			throw;
		}
		destroy_allocator::deallocate(begin_, end_,cap_ - begin_);
		begin_ = new_begin;
		end_ = new_end;
		cap_ = new_begin + new_size;

	}

	//弹出尾部元素
	template <class T >
	void vector<T>::pop_back()
	{
		MYSTL_DEBUG(!empty());
		data_allocator::destroy(end_ - 1);
		--end_;
	}


	// get_new_cap 函数
	template <class T>
	typename vector<T>::size_type
		vector<T>::
		get_new_cap(size_type add_size)
	{
		const auto old_size = capacity();
		THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
			"vector<T>'s size too big");
		if (old_size > max_size() - old_size / 2)
		{
			return old_size + add_size > max_size() - 16
				? old_size + add_size : old_size + add_size + 16;
		}
		const size_type new_size = old_size == 0
			? mystl::max(add_size, static_cast<size_type>(16))
			: mystl::max(old_size + old_size / 2, old_size + add_size);
		return new_size;
	}

	//fill_assign 函数
	template <class T>
	void vector<T>::fill_assign(size_type n, const value_type& value)
	{
		if (n > capacity())
		{
			vector tmp(n, value);
			swap(tmp);
		}
	}

	//与另一个vector交换
	template <class T>
	void vector<T>::swap(vector<T>& rhs) noexcept
	{
		if (this != &rhs)
		{
			mystl::swap(begin_, rhs.begin_);
			mystl::swap(end_, rhs.end_);
			mystl::swap(cap_, rhs.cap_);
		}
	}


	//copy_assign 函数
	template<class T>
	template <class ITter>
	void vector<T>::copy_assign(ITter first, ITter last, input_iterator_tag)
	{
		auto cur = begin_;
		for (; first != last && cur != end_; ++first, ++cur)
		{
			*cur = *first;
		}
		if (first == last)
		{
			erase(cur, end_);
		}
	}



}




#endif
