#include "semaphor.h"


Semaphore::Semaphore(int init) {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	myImpl = new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
	hard_lock;
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	hard_unlock;
#endif
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	myImpl->signal();
}

int Semaphore::val() const {
	return myImpl->val();
}
