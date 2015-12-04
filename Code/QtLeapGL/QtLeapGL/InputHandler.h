#pragma once
#include <limits>
#include <thread>
#include <memory>
#include <list>
#include "Wrapper.h"
#include "Leap.h"
class InputHandler
{
private:
	/* Thread needed to gather the data of the Leap Motion*/
	std::thread *worker;

	/* Stores the coordinates of the hand to be drawn*/
	Hand *trig;
	
	/* The function that gathers the input data of the Leap motion device*/
	void gatherData();

public:
	InputHandler();
	~InputHandler();

	Hand* getResults();
};

