#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

//这个头文件包含两个函数 construct  destroy
//construct  ：负责对象的构造
//destroy  :  负责对象的析构

#include  <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  //unused parameter
#endif

namespace mystl {

	//construct 构造对象

	template <class Ty>
	void construct(Ty* ptr)
	{
		::new ((void*)ptr) Ty();
	}

	template <class Ty1,class Ty2>
	void construct(Ty1* ptr, const Ty2& value)
	{
		::new ((void*)ptr) Ty1(value);
	}

	template <class Ty,class... Args>
	void construct(Ty* ptr, Arg&&... args)
	{
		::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
	}

	template <class Ty>

}







#endif