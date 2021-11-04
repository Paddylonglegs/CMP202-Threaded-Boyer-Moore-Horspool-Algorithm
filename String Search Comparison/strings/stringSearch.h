/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#pragma once
#ifndef stringSearch_H
#define stringSearch_H

#include "task.h"

#include <list>
#include <string>
#include <mutex>

using std::list;
using std::string;
using std::mutex;

struct test
{
	std::string pat, text_;
	int found_;
	int time;
	int total_found_;
	list<Position> matchesBM; //creating list for matches
	std::string currentTxt = "";
	int threadID;
	int iterID;
	int iterations_;
};

class stringSearch : public task
{
public:

	stringSearch(test &stringSearch_args)
		: stringSearch_args_(stringSearch_args)
	{
	}

	Position run() override;


private:
	test stringSearch_args_;
	mutex messages;
	mutex reset;
};

#endif

