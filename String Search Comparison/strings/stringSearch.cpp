/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#include "stringSearch.h"
#include "utils.h"
#include "channel.h"

#include <iostream>
#include <mutex>
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::mutex;
using std::string;
using std::this_thread::sleep_for;
using std::chrono::duration_cast;
using std::chrono::microseconds; //measurement

//Defining the type of clock
//typedef std::chrono::system_clock each;
//typedef std::chrono::high_resolution_clock the_clock;
typedef std::chrono::steady_clock each;

Position stringSearch::run()
{
	each::time_point start = each::now(); ///////USING CHRONO TO MEASURE TIME////////

	reset.lock();
	stringSearch_args_.matchesBM.clear();
	stringSearch_args_.found_ = 0;
	reset.unlock();

	Position pat_len = stringSearch_args_.pat.size();	
	Position text_len = stringSearch_args_.text_.size();
	Position split = text_len - pat_len;


	for (Position i = 0; i < split; ++i)
	{
		//show_context(stringSearch_args_.text_, i);

		Position skip[256]; //array used as accessing elements is constant time
		for (int i = 0; i < 256; i++)
		{
			skip[i] = pat_len; //setting length of skip to pat lenth
		}

		for (int i = 0; i < pat_len; i++)
		{
			skip[int(stringSearch_args_.pat[i])] = (pat_len - 1) - i;
		}

		Position s = skip[int(stringSearch_args_.text_[i + pat_len - 1])];
		
		if (s != 0)
		{
			i = i + s - 1; //skip forwards by pattern length
			continue; //going through program after check
		}

		Position j;
		for (j = 0; j < pat_len; j++)
		{
			if (stringSearch_args_.text_[i + j] != stringSearch_args_.pat[j]) {
				break; // Doesn't match here.
			}
		}
		
		if (j == pat_len)
		{
			std::unique_lock<mutex> lock(messages); //locking shared resource
			stringSearch_args_.found_ = stringSearch_args_.found_ + 1; //just to compare if algorithms are finding same amount of pat
			stringSearch_args_.matchesBM.push_back(i); //Match, push onto list
		}
	}
	
	// Stop timing
	each::time_point end = each::now();
	auto time_taken = duration_cast<microseconds>(end - start).count(); // Compute the difference between the two times in milliseconds
	
	//cout << "It took " << time_taken << "microseconds to run thread task " << stringSearch_args_.threadID << endl;
	
	//Printing out matches on last iteration//
	if (stringSearch_args_.iterID == stringSearch_args_.iterations_ - 1)
	{
		cout << "Found " << stringSearch_args_.found_ << " matches for '" << stringSearch_args_.pat << "' using boyer moore with thread task " << stringSearch_args_.threadID << endl;
	}

	return time_taken;
	//return stringSearch_args_.found_;
}
