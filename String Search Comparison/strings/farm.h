/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#pragma once
#ifndef FARM_H
#define FARM_H

#include "task.h"

#include <queue>
#include <mutex>
#include <thread>
#include <iostream>

using std::unique_lock; //To protect queue
using std::thread;
using std::queue;
using std::mutex;
using std::cout;
using std::endl;

struct farming
{
	std::string pat, text_;
	int threadID;
	int iterID;
	int iterations_;
	int threads;
};

class farm
{
public:

	/** Add a task to the farm.
		The task will be deleted once it has been run. */
	void add_task(task* task);

	/** Run all the tasks in the farm.
		This method only returns once all the tasks in the farm
		have been completed. */
	Position run(int thread_amount, int ID, int iterations);

private:

	mutex collection_mutex_;
	int threads;
	queue<task*> collection;

	//collection.empty(); //Check if queue empty or not, true or false
	//collection.push(task); //Inserts element at the end
	//collection.pop(); //removes first element
	//collection.front(); //Access first element
};

#endif
