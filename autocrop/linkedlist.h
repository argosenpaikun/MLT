#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include <string>
using namespace std;

typedef string List;

class LinkedList {
public:
	LinkedList();
	~LinkedList();

	List retrieve(int position) const;
	void insert(List newItem);
	void remove(int position);

	bool isEmpty() const;
	void display() const;
	int getLength() const;
private:
	struct Node {
		List item;
		Node* next;
	};

	Node* head;
	int size;
};

#endif