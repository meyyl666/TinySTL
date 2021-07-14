#ifndef _MYQUEUE_H
#define _MYQUEUE_H


/*��ջʵ�ֶ���*/


#include<stack>

using namespace std;
 
class MyQueue
{
public:
	stack<int> stIn;
	stack<int> stOut;

	MyQueue(){}

	void push(int x)
	{
		stIn.push(x);
	}

	int pop() {
		if (stOut.empty()) {
			//ֻ�е�stdOutΪ�յ�ʱ���ٴ�stdIn �ﵼ������

			while (!stIn.empty()) {
				stOut.push(stIn.top());
				stIn.pop();
			}
		}
		int result = stOut.top();
		stOut.pop();
		return result;
	}

	/** Get the front element. */
	int peek() {
		int res = this->pop();
		stOut.push(res);
		return res;
	}
	/** Returns whether the queue is empty. */

	bool empty() {
		return stIn.empty() && stOut.empty();
	}



};



#endif