#include "event.h"
#include "kernelev.h"

Event::Event(IVTNo ivtNo) {
	hard_lock;
	myImpl = new KernelEv(ivtNo);
	hard_unlock;
}

Event::~Event() {
	hard_lock;
	delete myImpl;
	hard_unlock;
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
