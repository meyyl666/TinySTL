#ifndef _MYSTACK_H
#define _MYSTACK_H

#include<queue>

using namespace std;

class MyStack {
	queue<int> que1;
	queue<int> que2;  //��������  ��������
	/** Initialize your data structure here. */
	MyStack(){}

	/** Initialize your data structure here. */
	void push(int x) {
		que1.push(x);
	}

	/** Removes the element on top of the stack and returns that element. */
	int pop() {
		int size = que1.size();
		size--;
		while (size--)                 //��que1����que2�����������һ��Ԫ��
		{
			que2.push(que1.front());
			que1.pop();
		}

		int result = que1.front();    //���µ����һ��Ԫ�ؾ���Ҫ���ص�ֵ
		que1.pop();
		que1 = que2;                  //�ٽ�que2��ֵ��que1
		while (!que2.empty())   //���que2
		{
			que2.pop();

		}
		return result;
		
		
	}

	/** Get the top element. */
	int top() {
		return que1.back();
	}


	/** Returns whether the stack is empty. */
	bool empty() {
		return que1.empty();
	}




};



#endif