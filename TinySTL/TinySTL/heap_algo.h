#pragma once
// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace mystl {

	/*****************************************************************************************/
	// push_heap
	// 该函数接受两个迭代器，表示一个 heap 底层容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap

	/*****************************************************************************************/

	template <class  RandomIter ,class Disdance,class T>
	void push_heap_aux(RandomIter first, Disdance holeIndex, Disdance topIndex, T value)
	{
		auto parent = (holeIndex - 1) / 2;   //找出要插入节点的父节点
		while (holeIndex > topIndex && *(first + parent) < value)
		{
			//当尚未达到顶端且父节点小于新值
			*(first + holeIndex) = *(first + parent);   //另洞值为父值
			holeIndex = parent;  //调整洞号  向上提升至父节点
			parent = (holeIndex - 1) / 2; //新洞的父节点
		}//持续至顶端，或满足heap的次序特性为止
		*(first + holeIndex) = value; //另洞值为新值，完成插入操作
	}

	template <class RandomIter,class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*)
	{
		mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
	}

	template <class RandomIter>
	void push_heap(RandomIter first, RandomIter last)
	{
		// 此函数被调用时，新元素应该已置于底部容器的最尾端
		mystl::push_heap_d(first, last, distance_type(first));
	}

	//重载版本使用函数对象comp代替比较操作
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
// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
/*****************************************************************************************/
	template <class RandomIter,class T,class Distance>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
	{
		//先进行下溯过程
		auto topIndex = holeIndex;
		auto rchild = 2 * holeIndex + 2;  //洞节点之右子节点
		while (rchiled < len)
		{
			//比较洞节点之左右两个子值，然后以rchild代表较大子节点
			if (*(first + rchild) < *(first + (rchild - 1)))
			{
				rchild--;
			}
			//另较大子值为洞值，再将洞号下移至较大子节点
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			//找出新洞节点的右子节点
			rchild = 2 * (rchild + 1);
		}

		if (rchild == len)
		{
			//如果没有右子节点
			*(first + holeIndex) = *(first + (rchild - 1));
			holeIndex = rchild - 1;
		}
		//再执行一次上溯过程
		mystl::push_heap_aux(first, holeIndex, topIndex, value);
	}

	template <class RandomIter,class T,class Distance>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*)
	{
		//先将首值调至尾节点，然后调整【first,last-1】 使之成为一个max_heap
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
// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
/*****************************************************************************************/

	template <class RamdomIter>
	void sort_heap(RamdomIter first, RamdomIter last)
	{
		//以下 每执行一次pop_heap()，极值（STL中为极大值）即被放置到尾端
		//扣除尾端在执行一次pop_heap()，次极值又被放在新尾端，一直下去，最后即得
		//排序结果

		while (last - first > 1)
		{
			pop_heap(first, last--); //每执行pop_heap()一次，操作范围即退缩一格
		}
	}
	// 重载版本使用函数对象 comp 代替比较操作
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
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
// 将一个现有的数据转化成一个heap
/*****************************************************************************************/
	template <class RamdomIter,class Distance>
	void make_heap_aux(RamdomIter first, RamdomIter last, Distance*)
	{
		if (last - first < 2) return;  //如果长度为0或1，不必重新排列
		Distance len = last - first;
		//找出第一个需要重排的子树头部，以holeIndex标示出，由于任何节点都不需要执行perkocate down,所以有以下计算
		auto holeIndex = (len - 2) / 2;
		while (true)
		{
			//重排以holeIndex为首的子树，len是为了让adjust_heap()判断操作范围
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

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomIter, class Distance, class Compared>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp)
	{
		if (last - first < 2)
			return;
		auto len = last - first;
		auto holeIndex = (len - 2) / 2;
		while (true)
		{
			// 重排以 holeIndex 为首的子树
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
