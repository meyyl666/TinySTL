#ifndef _MYQUEUE_H
#define _MYQUEUE_H


/*用栈实现队列*/


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
			//只有当stdOut为空的时候，再从stdIn 里导入数据

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