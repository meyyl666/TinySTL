#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

//���ͷ�ļ������������� construct  destroy
//construct  ���������Ĺ���
//destroy  :  ������������

#include  <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  //unused parameter
#endif

namespace mystl {

	//construct �������

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