#pragma once

#include <mutex>
#include <queue>

class MySafeQueue {

private:

	std::queue<char*> chunkQueue;
	std::mutex queueLock;

public:

	char* front();

	void push(char* data);

	void pop();

	bool empty();

};