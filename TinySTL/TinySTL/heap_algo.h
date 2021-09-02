#pragma once
// ���ͷ�ļ����� heap ���ĸ��㷨 : push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace mystl {

	/*****************************************************************************************/
	// push_heap
	// �ú���������������������ʾһ�� heap �ײ���������β��������Ԫ���Ѿ����뵽�ײ���������β�ˣ����� heap

	/*****************************************************************************************/

	template <class  RandomIter ,class Disdance,class T>
	void push_heap_aux(RandomIter first, Disdance holeIndex, Disdance topIndex, T value)
	{
		auto parent = (holeIndex - 1) / 2;   //�ҳ�Ҫ����ڵ�ĸ��ڵ�
		while (holeIndex > topIndex && *(first + parent) < value)
		{
			//����δ�ﵽ�����Ҹ��ڵ�С����ֵ
			*(first + holeIndex) = *(first + parent);   //��ֵΪ��ֵ
			holeIndex = parent;  //��������  �������������ڵ�
			parent = (holeIndex - 1) / 2; //�¶��ĸ��ڵ�
		}//���������ˣ�������heap�Ĵ�������Ϊֹ
		*(first + holeIndex) = value; //��ֵΪ��ֵ����ɲ������
	}

	template <class RandomIter,class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*)
	{
		mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
	}

	template <class RandomIter>
	void push_heap(RandomIter first, RandomIter last)
	{
		// �˺���������ʱ����Ԫ��Ӧ�������ڵײ���������β��
		mystl::push_heap_d(first, last, distance_type(first));
	}

	//���ذ汾ʹ�ú�������comp����Ƚϲ���
	template <class RandomIter,class Distance,class T,class Compared>
	void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compared comp)
	{
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && copm(*(first + parent), value))
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}
	template <class RandomIter, class Compared, class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp)
	{
		mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
			*(last - 1), comp);
	}

	template <class RandomIter, class Compared>
	void push_heap(RandomIter first, RandomIter last, Compared comp)
	{
		mystl::push_heap_d(first, last, distance_type(first), comp);
	}

/*****************************************************************************************/
// pop_heap
// �ú���������������������ʾ heap ��������β���� heap �ĸ��ڵ�ȡ���ŵ�����β�������� heap
/*****************************************************************************************/
	template <class RandomIter,class T,class Distance>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
	{
		//�Ƚ������ݹ���
		auto topIndex = holeIndex;
		auto rchild = 2 * holeIndex + 2;  //���ڵ�֮���ӽڵ�
		while (rchiled < len)
		{
			//�Ƚ϶��ڵ�֮����������ֵ��Ȼ����rchild����ϴ��ӽڵ�
			if (*(first + rchild) < *(first + (rchild - 1)))
			{
				rchild--;
			}
			//��ϴ���ֵΪ��ֵ���ٽ������������ϴ��ӽڵ�
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			//�ҳ��¶��ڵ�����ӽڵ�
			rchild = 2 * (rchild + 1);
		}

		if (rchild == len)
		{
			//���û�����ӽڵ�
			*(first + holeIndex) = *(first + (rchild - 1));
			holeIndex = rchild - 1;
		}
		//��ִ��һ�����ݹ���
		mystl::push_heap_aux(first, holeIndex, topIndex, value);
	}

	template <class RandomIter,class T,class Distance>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*)
	{
		//�Ƚ���ֵ����β�ڵ㣬Ȼ�������first,last-1�� ʹ֮��Ϊһ��max_heap
		*result = *first;
		mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value);
	}

	template <class RandomIter>
	void pop_heap(RandomIter first, RandomIter last)
	{
		mystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
	}

/*****************************************************************************************/
// sort_heap
// �ú���������������������ʾ heap ��������β������ִ�� pop_heap ������ֱ����β������1
/*****************************************************************************************/

	template <class RamdomIter>
	void sort_heap(RamdomIter first, RamdomIter last)
	{
		//���� ÿִ��һ��pop_heap()����ֵ��STL��Ϊ����ֵ���������õ�β��
		//�۳�β����ִ��һ��pop_heap()���μ�ֵ�ֱ�������β�ˣ�һֱ��ȥ����󼴵�
		//������

		while (last - first > 1)
		{
			pop_heap(first, last--); //ÿִ��pop_heap()һ�Σ�������Χ������һ��
		}
	}
	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class RamdomIter,class Compared>
	void sort_heap(RamdomIter first, RamdomIter last, Compared comp)
	{
		while (last - first > 1)
		{
			mystl::pop_heap(first,last - 1.comp);
		}
	}

/*****************************************************************************************/
// make_heap
// �ú���������������������ʾ heap ��������β���������ڵ����ݱ�Ϊһ�� heap
// ��һ�����е�����ת����һ��heap
/*****************************************************************************************/
	template <class RamdomIter,class Distance>
	void make_heap_aux(RamdomIter first, RamdomIter last, Distance*)
	{
		if (last - first < 2) return;  //�������Ϊ0��1��������������
		Distance len = last - first;
		//�ҳ���һ����Ҫ���ŵ�����ͷ������holeIndex��ʾ���������κνڵ㶼����Ҫִ��perkocate down,���������¼���
		auto holeIndex = (len - 2) / 2;
		while (true)
		{
			//������holeIndexΪ�׵�������len��Ϊ����adjust_heap()�жϲ�����Χ
			mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
			if (holeIndex == 0)
				return;
			holeIndex--;
		}
	}


	template <class RandomIter >
	void make_heap(RandomIter first, RandomIter last)
	{
		mystl::make_heap_aux(first, last, distance_type(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class RandomIter, class Distance, class Compared>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
	{
		if (last - first < 2)
			return;
		auto len = last - first;
		auto holeIndex = (len - 2) / 2;
		while (true)
		{
			// ������ holeIndex Ϊ�׵�����
			mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
			if (holeIndex == 0)
				return;
			holeIndex--;
		}
	}

	template <class RandomIter, class Compared>
	void make_heap(RandomIter first, RandomIter last, Compared comp)
	{
		mystl::make_heap_aux(first, last, distance_type(first), comp);
	}






}
