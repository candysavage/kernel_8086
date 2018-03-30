#include "event.h"
#include "kernelev.h"

Event::Event(IVTNo ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

Event::~Event() {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
