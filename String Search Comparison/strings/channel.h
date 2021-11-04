/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#pragma once
#include "stringSearch.h"
#include <iostream>
typedef std::unique_lock<mutex> mutex_lock;

class channel
{
public:
	mutex buffer_mutex;
	list<int> buffer;
	std::condition_variable cv;
	bool ready = false;
	
	void write(int data) {
		//std::cout << "Writing" << data << std::endl;
		mutex_lock lock(buffer_mutex);
		buffer.push_back(data);
		ready = true; //data ready to be read
		cv.notify_all();
	}
		

		int read() {
			
			while (true) {
				
				mutex_lock lock(buffer_mutex);
				cv.wait(lock, [&]() { return ready; }); //once data ready to be read continue

				if (!buffer.empty()) {
					
					int item = buffer.front();
					buffer.pop_front();
					//std::cout << "Returning " << item << std::endl;
					return item;
				}
				else {
					
					return NULL;
				}
			}
	
		};
};

