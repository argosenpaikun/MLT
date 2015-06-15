#ifndef _STACK_H_
#define _STACK_H_

#include <iostream>
using namespace std;

typedef string StackItem;

class Stack {
public:
	Stack();
	~Stack();

	void push(const StackItem& newItem);
	void pop();
	StackItem top();
	bool isEmpty();

private:
	struct Node {
		StackItem item;
		Node* next;
	};

	Node* head;
};
#endif