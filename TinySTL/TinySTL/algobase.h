#pragma once
// ���ͷ�ļ������� mystl �Ļ����㷨

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
// ȡ�����еĽϴ�ֵ���������ʱ��֤���ص�һ������
/*****************************************************************************************/

	//�汾һ��ʹ�ö����ͱ�T���ṩ��greater-than���������жϴ�С
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
	// ȡ�����еĽ�Сֵ���������ʱ��֤���ص�һ������
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
	// ��������������ָ����Ե�
	/*****************************************************************************************/

	template <class FIter1,class FIter2>
	void iter_swap(FIter1 lhs, FIter2 rhs)
	{
		mystl::swap(*lhs, *rhs);
	}



	/*****************************************************************************************/
	// equal
	// �Ƚϵ�һ������ [first, last)�����ϵ�Ԫ��ֵ�Ƿ�͵ڶ��������
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


	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
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