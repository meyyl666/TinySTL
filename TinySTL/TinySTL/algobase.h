#pragma once
// 这个头文件包含了 mystl 的基本算法

#include <cstring>

#include "iterator.h"
#include "util.h"


namespace mystl {
#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

/*****************************************************************************************/
// max
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/

	//版本一：使用对象型别T所提供的greater-than操作符来判断大小
	template <class T>
	const T& max(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}

	template <class T,class Compare>
	const T& max(const T& lhs, const T& rhs,Compare comp)
	{
		return comp(lhs, rhs) ? lhs : rhs;
	}

	/*****************************************************************************************/
	// min 
	// 取二者中的较小值，语义相等时保证返回第一个参数
	/*****************************************************************************************/

	template <class T>
	const T& min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}
	template <class T,class Compare>
	const T& min(const T& lhs, const T& rhs,Compare comp)
	{
		return comp(lhs, rhs) ? rhs : lhs;
	}

	/*****************************************************************************************/
	// iter_swap
	// 将两个迭代器所指对象对调
	/*****************************************************************************************/

	template <class FIter1,class FIter2>
	void iter_swap(FIter1 lhs, FIter2 rhs)
	{
		mystl::swap(*lhs, *rhs);
	}



	/*****************************************************************************************/
	// equal
	// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
	/*****************************************************************************************/

	template <class InputIter1,class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
			{
				return false;
			}

		}
		return true;
	}


	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIter1,class InputIter2,class Compared>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!comp(*first1, *first2))
				return false;
		}
		return true;
	}

}