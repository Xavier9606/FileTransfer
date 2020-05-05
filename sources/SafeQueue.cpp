
#include "headers/SafeQueue.h"


	char* MySafeQueue::front() {
		char* temp = nullptr;
		queueLock.lock();
		temp = chunkQueue.front();
		queueLock.unlock();
		return temp;
	}

	void MySafeQueue::push(char* data) {
		queueLock.lock();
		chunkQueue.push(data);
		queueLock.unlock();

	}
	
	void MySafeQueue::pop() {
		queueLock.lock();
		chunkQueue.pop();
		queueLock.unlock();
	}

	bool MySafeQueue::empty() {
		bool temp = false;
		queueLock.lock();
		temp = chunkQueue.empty();
		queueLock.unlock();
		return temp;
	}
