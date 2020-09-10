#pragma once

#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

class ProducerConsumer
{
	void producer(std::queue<std::string>* downloaded_pages, std::mutex* m, int index);
	void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed);
public:
	void TestCode();
};

