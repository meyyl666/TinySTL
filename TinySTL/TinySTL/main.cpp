
#include "allocator.h"
#include<vector>
#include<iostream>
#include"list.h"

using namespace std;
using namespace mystl;

int main()
{
	mystl::CDList<int> iList;

	for (int i = 0; i < 5; i++)
	{
		iList.insert(i, i * i);
	}
	iList.output();

	iList.insert(2, 99);
	iList.output();
	iList.erase(3);
	iList.output();
	iList.modify(3, 66);
	iList.output();
	iList.clear();
	iList.output();

	mystl::CDList<int> ::iterator it;
	for (it = iList.begin(); it != iList.end();it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	
	
	 

}