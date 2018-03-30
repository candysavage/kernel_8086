#include "list.h"
#include "pcb.h"

List::List() {
	first = 0;
	last = 0;
}

List::~List() {
	listElement *temp = first;
	while(first) {
		first = first->nextElement;
		temp->element = 0;
		temp->nextElement = 0;
		#ifndef BCC_BLOCK_IGNORE
		hard_lock;
		delete temp;
		hard_unlock;
		#endif
	}
	first = 0;
	last = 0;
}

void List::put(void *element) {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	listElement *newElement = new listElement;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
	newElement->element = element;
	newElement->nextElement = 0;
	if(!first) {
		first = last = newElement;
	} else {
			last->nextElement = newElement;
			last = newElement;
	}
}

void* List::getFirst() {
	void *returnElement = 0;
	if(first)
	{
		returnElement = first->element;
		listElement *temp = first;
		first = first->nextElement;
		if(!first)
			last = 0;
		#ifndef BCC_BLOCK_IGNORE
		hard_lock;
		delete temp;
		hard_unlock;
		#endif
	}
	return returnElement;
}

void List::remove(void *T) {
	listElement** traverseNode = &first;
	while (*traverseNode && (**traverseNode).element != T)
		traverseNode = &(*traverseNode)->nextElement;
	if(*traverseNode == 0)
		return;

	listElement* deletedNode = *traverseNode;
	*traverseNode = deletedNode->nextElement;
	deletedNode->element = 0;
	deletedNode->nextElement = 0;
	#ifndef BCC_BLOCK_IGNORE
	hard_lock;
	delete deletedNode;
	hard_unlock;
	#endif
}

void List::putSort(void *T, Time timeToSleep) {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	listElement* n = new listElement;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
	n->element = T;
	n->timeToSleep = timeToSleep;
	n->nextElement = 0;
	if(first==0) {
		first = last = n;
	} else {
			listElement *prev = 0, *curr = first;
			for(; curr; prev = curr, curr = curr->nextElement) {
				if(curr->timeToSleep <= n->timeToSleep)
					n->timeToSleep -= curr->timeToSleep;
				else
					break;
			}

			if(curr == 0) {
			last = (!first ? first : last->nextElement) = n;
			} else if(prev == 0) //umetanje na pocetak
				{
					n->nextElement = first;
					first->timeToSleep -= n->timeToSleep;
					first = n;
				} else {
					prev->nextElement = n;
					n->nextElement = curr;
					curr->timeToSleep -= n->timeToSleep;
				}
	}
}

void List::removeSort(void *T) {
	for(listElement *prev = 0, *curr = first; curr; prev = curr, curr = curr->nextElement) {
		if(curr->element == T) {
			if(curr == first) {
				curr->nextElement->timeToSleep += curr->timeToSleep;
				first = curr->nextElement;
				if(!first)
					last = 0;
#ifndef BCC_BLOCK_IGNORE
				hard_lock;
#endif
				delete curr;
#ifndef BCC_BLOCK_IGNORE
				hard_unlock;
#endif
				break;
		} else {
				if(curr != last)
					curr->nextElement->timeToSleep += curr->timeToSleep;
				else
					last = prev;

				prev->nextElement = curr->nextElement;
#ifndef BCC_BLOCK_IGNORE
				hard_lock;
#endif
				delete curr;
#ifndef BCC_BLOCK_IGNORE
				hard_unlock;
#endif
				break;
			}
		}
	}
}

void* List::findByID(ID id) {
	listElement *tmp = first;
	while(tmp)
		if(((PCB*)tmp->element)->ID == id)
			return tmp->element;

	return 0;
}
