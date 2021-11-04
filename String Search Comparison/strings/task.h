/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#ifndef task_h
#define task_h

#include "utils.h"

class task
{

	/** Abstract base class: a task to be executed. */
public:
	virtual ~task()
	{
	}

	/** Perform the task. Subclasses must override this. */
	virtual Position run() = 0;

};

#endif

