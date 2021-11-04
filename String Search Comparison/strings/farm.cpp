/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#include "farm.h"
#include "stringSearch.h"
#include "channel.h"

void farm::add_task(task* task)
{
	unique_lock<mutex> lock(collection_mutex_); //locking shared resource
	collection.push(task);
}

Position farm::run(int thread_amount, int ID, int iterations)
{
	std::vector<thread*> threads;
	int total=0;
	channel chan;

	//start as many threads as user chooses
	for (int i = 0; i < thread_amount; i++)
	{
		threads.push_back(new thread([this, &total, &chan, i]()
			{
				//std::thread::id test_id = std::this_thread::get_id();
				//cout << "This thread is " << test_id << endl;
				
				// infinite loop until all tasks completed
				while (true)
				{
					unique_lock<mutex> lock(collection_mutex_);

					// exit the loop if the task queue is empty
					if(!collection.empty())
					{
						
						task* Task = collection.front(); // get a task from the queue
						collection.pop(); //take task of the queue once stored

						//Run the task
						total = Task->run();

						//Write time to channel for later
						chan.write(total);

						// delete task
						delete Task;
						Task = nullptr;
					}
					else
					{
						return; //no more tasks left, exit loop
					}
					
				}
			})
		);
	}

	//Join all the threads
	for (thread *t : threads)
	{
		t->join();
	}

	//Pull times from the channel
	float time = 0;
	for (int i=0; i < thread_amount; i++)
	{
		time += chan.read(); //Add returned time from channel to get total time for all threads
	}

	if (ID == iterations - 1) //If this is the final iteration display results
	{
		cout << endl;
		cout << "It took " << time / 1000 << " milliseconds to run boyer moore with " << thread_amount << " threads\n";
	}

	return time / 1000; //Return in milliseconds instead of microseconds

	//Can return total matches found display that instead
	//cout << "Found " << time << " total matches with boyer moore running " << thread_amount << " threads\n";
}
