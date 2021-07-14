#pragma once
#ifndef _MYALLOC_
#define _MYALLOC_

#include <new>
#include<cstddef>
#include<cstdlib>
#include<iostream>
using namespace std;


namespace mystl {
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0) {
			cerr << "out of memery" << endl;
			exit(1);
		}
		return tmp;
	}

	//deallocate ��ʵ��ʵ�֣��򵥷�װdelete
	template <class T>
	inline void _deallocate(T* buffer) { ::operator delete(buffer); }

	//contruct ��ʵ��ʵ�֣�ֱ�ӵ��ö���Ĺ��캯��
	template<class T1,class T2>
	inline void _contruct(T1* p, const T2& value) { new(p) T1(value); }

	//destroy ��ʵ��ʵ�֣�ֱ�ӵ��ö������������
	template <class T>
	inline void _destory(T* ptr) { ptr->~T(); }

	template <class T>
	class allocator {
	public:
		typedef T           value_type;
		typedef T*          pointer;
		typedef const T*    const_pointer; 
		typedef T&          reference;
		typedef const T&    const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;

		//���캯��
		allocator() { return; }
		template <class U>
		allocator(const allocator<U>& c){}

		//rebind allocator of type U
		/*
		* ʵ��������ͬ�����߻����йص����ͣ��������� T �� Node���ͣ���ʹ��ͬһ���ڴ���䷽����
		*/
		template <class U>
		struct rebind { typedef allocator<U> other; };

		//allocate,deallocate,contruct��destory���������������ʵ��ʵ��
		//hint used for lacality
		pointer allocate(size_type n, const void* hind = 0) {
			return _allocate((difference_type)n, (pointer)0);

		}

		void deallocate(pointer p, size_type n) { _deallocate(p); }
		void construct(pointer p, size_type value) { _contruct(p, value); }
		void destory(pointer p) { _destory(p); }

		pointer address(reference x) { return (pointer)&x; }
		const_pointer const_address(const_reference x) { return (const_pointer)&x; }

		size_type max_size() const { return size_type(UINT_MAX / sizeof(T)); }





	};
}  //end of namespace myalloc



#endif