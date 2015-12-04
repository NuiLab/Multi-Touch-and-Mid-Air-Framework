#include "InputHandler.h"

/* Creates the worker thread */
InputHandler::InputHandler()
{
	worker = new std::thread(&InputHandler::gatherData, this);

	/* Create a hand object and give it pixel position x: -10 000 y: -10 000 so it isn't displayed on screen */
	trig = new Hand(-10000, -10000);
}

/* Deletes the memory allocated for the thread */
InputHandler::~InputHandler()
{
	delete worker;
	delete trig;
}

/* Gathers the input from the Leap Motion*/
void InputHandler::gatherData()
{
	Leap::Controller leap;
	Leap::HandList hands;
	Leap::PointableList points;
	Leap::InteractionBox iBox;

	/* Constantly look for fingertips and update the position of the hand object */
	while (true)
	{
		hands = leap.frame().hands();
		points = leap.frame().pointables();
		iBox = leap.frame().interactionBox();

		int count = points.count();

		for (int p = 0; p < points.count(); p++)
		{
			Leap::Pointable point = points[p];
			Leap::Vector normalizedPosition = iBox.normalizePoint(point.stabilizedTipPosition());
			float tx = normalizedPosition.x * 800;
			float ty = 800 - normalizedPosition.y * 800;
			trig->setX(tx);
			trig->setY(ty);
		}

	}
}

/* Returns the coordinates gathered in a list */
Hand* InputHandler::getResults()
{
	return trig;
}

