#include "stack.h"
#include <cstdlib>

Stack::Stack() : head(NULL) {}

Stack::~Stack() {
	while(!isEmpty())
		pop();
}

void Stack::push(const StackItem& newItem) {
	try {
		Node* newPtr = new Node;

		newPtr->item = newItem;
		newPtr->next = head;
		head = newPtr;
	} catch(bad_alloc e) {
		cout << "ERROR: push cannot allocate memory." << endl;
		getchar();
		exit(1);
	}
}

void Stack::pop() {
	if(isEmpty()) {
		cout << "ERROR: stack empty on pop." << endl;
		getchar();
		exit(1);
	} else {
		Node* temp = head;
		head = head->next;

		temp->next = NULL;
		delete temp;
	}
}

StackItem Stack::top() {
	return head->item;
}

bool Stack::isEmpty() {
	return head == NULL;
}