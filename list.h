#ifndef _list_h_
#define _list_h_

#include "def.h"

struct listElement
{
	void *element;
	Time timeToSleep;
	listElement *nextElement;
};

class List
{
public:
		listElement *first, *last;

		List();
		~List();

		void put(void *element);
		void* getFirst();
		void remove(void *T);
		void* findByID(ID id);

		void putSort(void *T, Time timeToSleep);
		void removeSort(void *T);

};

#endif
