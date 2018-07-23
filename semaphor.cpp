#include "semaphor.h"


Semaphore::Semaphore(int init = 1) {
	hard_lock;
	myImpl = new KernelSem(init);
	hard_unlock;
}

Semaphore::~Semaphore() {
	hard_lock;
	delete myImpl;
	hard_unlock;
}

int Semaphore::wait(int toBlock) {
	return myImpl->wait(toBlock);
}

void Semaphore::signal() {
	myImpl->signal();
}

int Semaphore::val() const {
	return myImpl->val();
}
