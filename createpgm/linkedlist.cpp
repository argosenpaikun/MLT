#include <fstream>
#include <iostream>
#include "linkedlist.h"

LinkedList::LinkedList()
	: head(NULL), size(0) {}

LinkedList::~LinkedList() {
	while(!isEmpty())
		remove(1);
}

List LinkedList::retrieve(int position) const {
	if(position >= 1 && position <= size) {
		Node* ptr = head;

		// Move ptr(position - 1) steps
		for(int i = 0; i < position - 1; i++)
			ptr = ptr->next;
		return ptr->item;
	}
}

void LinkedList::insert(List newItem) {
	// create new node and place newItem
	Node* newPtr = new Node;
	newPtr->item = newItem;

	newPtr->next = head;
	head = newPtr;
	size++;
}

void LinkedList::remove(int position) {
	if(position >= 1 && position <= size) {
		Node* cur;

		if(position == 1) {
			cur = head;
			head = head->next;
		} else {
			Node* prv = head;
			for(int i = 0; i < position-2; i++)
				prv = prv->next;
			cur = prv->next;
			prv->next = cur->next;
		}

		cur->next = NULL;
		delete cur;
		cur = NULL;
		--size;
	}
}

// display all items in the list
void LinkedList::display() const {
	for(Node* cur = head; cur != NULL; cur = cur->next)
		cout << cur->item << endl;
}

int LinkedList::getLength() const {
	return size;
}

bool LinkedList::isEmpty() const {
	return head == NULL; 
}