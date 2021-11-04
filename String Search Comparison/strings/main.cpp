/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <fstream>

//Threading
#include <cstdint>
#include <cstdlib>
#include <thread>

//Header Files
#include "utils.h"
#include "stringSearch.h"
#include "farm.h"
#include "task.h"
#include "channel.h"

// Import things I need from the standard library
using std::cout;
using std::endl;
using std::list;
using std::string;
using std::ofstream; //wrting timings to file
using std::cin;

//Threading
using std::thread;

//Globals
list<int> BMtimes; //creating list for Boyer-Moore times
int found = 0;

int main(int argc, char *argv[]) 
{
	//Text files to write timings too
	ofstream Boyer("BM_Timings.txt");
	
	//What will the algorithm be searching?
	string text;
	text = ""; //text to look through
	load_jute_book(text); //reading jute_book.txt as text

	string input = "";
	cout << "What would you like to search?: "; //User can define word to search
	cin >> input;
	string pattern = input; //Pattern to search


	unsigned int num_threads = thread::hardware_concurrency();
	cout << "Current threads your system can run : " << num_threads << endl;
	string modify_thread;
	cout << "Would you like to change thread amount? Yes/No: ";
	cin >> modify_thread;
	
	if(modify_thread=="Yes" || modify_thread =="yes")
	{
		cout << "How many threads would you like to run?: ";
		cin >> num_threads;
	}

	if (num_threads > thread::hardware_concurrency())
	{
		cout << "Please don't enter more threads than your system can run\n";
		return 0;
	}

	float range = static_cast<float>(177377) / num_threads;
	cout << "----------------------------------------------------------------\n";
	cout << "Range of text each thread will search: " << range << endl;

	int iterations = 0;
	cout << "How many iterations would you like to run?: ";
	cin >> iterations;
	
	cout << endl;
	cout << "Running " << iterations << " iterations with " << num_threads <<  " threads and " << num_threads <<  " tasks..." << endl;

	for (int it = 0; it < iterations; it++)
	{
	
		float textSlice = -range;

		//Run a load of tasks using a task farm
		farm f;
		test stringSearch_args_;	
		string currentTxt = "";

		//BOYER MOORE//
	
		if (it == iterations - 1)
		{
			cout << "----------------------------------------------------------------\n";
			cout << "Last iteration results...\n";
		}

		///Splitting the text up for each thread
		for (unsigned int i = 0; i < num_threads; i++) //User can set high amount to get good data
		{
			textSlice += range;
	
			//For each task, setting the paramaters and variables
			stringSearch_args_.pat = pattern;
			stringSearch_args_.text_ = text.substr(textSlice, range); //start and end for each slice
			stringSearch_args_.found_ = found;
			stringSearch_args_.threadID = i;
			stringSearch_args_.iterID = it;
			stringSearch_args_.iterations_ = iterations;

			f.add_task(new stringSearch(stringSearch_args_)); //Adding a task for each thread
			
		}

		BMtimes.push_back(f.run(num_threads, it, iterations)); //Running all the tasks, and pushing time taken returned onto the list.

	}

	cout << "----------------------------------------------------------------\n";
	cout << "All iterations complete" << endl;

	BMtimes.sort(); //sorting times into order before passing to ofstream file

	for (__int64 b : BMtimes)
	{
		Boyer << b << "\n"; //printing Boyer Moore results to text file, which will be used for scipy
	}

	return 0;
}
