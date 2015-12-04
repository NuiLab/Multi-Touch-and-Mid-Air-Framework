#include "Wrapper.h"

Hand::Hand(float x, float y)
{
	this->x[0] = this->x[1] = this->x[2] = this->x[3] = this->x[4] = x;
	this->y[0] = this->y[1] = this->y[2] = this->y[3] = this->y[4] = y;
	count = 0;
}

void Hand::draw()
{
	float r, g, b;
	float xI = 0, yI = 0;
	TAM::InputMapper Inputs[5];		/* Inputs mapper to be used for each of the lines*/
	TAM::Helpers newHelp(800, 800);

	/* Get fingertips colors */
	newHelp.generateColor(4, r, g, b);


	for (int i = 0; i < 5; i++)
	{
		/* Map the input coordinates and draw each of the fingertips */
		TAM::InputMapper newInput(&Helpers::mapper, x[i], y[i], 0.5f, 0.0f);		
		TAM::Circle newCircle(800, 800, 0.10f, r, g, b, 1.0f, false);
		TAM::Circle newInnerCircle(800, 800, 0.05f, r, g, b, 1.0f, false);

		newCircle.draw(newInput);
		newInnerCircle.draw(newInput);
		
		Inputs[i] = newInput;	/* Store the InputMapper for subsequent use */
	
		/* Store all coordinate values */
		xI += newInput.getXCoordinate();
		yI += newInput.getYCoordinate();
	}

	/* Generate color for palm and fingers */
	newHelp.generateColor(0, r, g, b);

	/* Get average to position the palm */
	xI /= 5;
	yI /= 5;

	/* Draw each of the connecting lines */
	TAM::Line newLine(r, g, b, 1.0f, 10.0f);
	newLine.setEndPoint(xI, yI - 0.75f, 0.0f);
	newLine.draw(Inputs[0]);

	TAM::Line newLine1(r, g, b, 1.0f, 10.0f);
	newLine1.setEndPoint(xI, yI - 0.75f, 0.0f);
	newLine1.draw(Inputs[1]);

	TAM::Line newLine2(r, g, b, 1.0f, 10.0f);
	newLine2.setEndPoint(xI, yI - 0.75f, 0.0f);
	newLine2.draw(Inputs[2]);

	TAM::Line newLine3(r, g, b, 1.0f, 10.0f);
	newLine3.setEndPoint(xI, yI - 0.75f, 0.0f);
	newLine3.draw(Inputs[3]);

	TAM::Line newLine4(r, g, b, 1.0f, 10.0f);
	newLine4.setEndPoint(xI, yI - 0.75f, 0.0f);
	newLine4.draw(Inputs[4]);

	/* Draw the palm at the averaged position */
	TAM::InputMapper sMapper;
	sMapper.setXCoordinate(xI - 0.25f);
	sMapper.setYCoordinate(yI -0.95f);
	sMapper.setZCoordinate(0.5f);

	
	TAM::Square newSquare(r, g, b, 1.0f, 0.5f);
	newSquare.draw(sMapper);
}

void Hand::setX(float x)
{
	this->x[count % 5] = x;
}

void Hand::setY(float y)
{
	this->y[count % 5] = y;
	count++;
}

/* Maps coordinates to OpenGL coordinates */
void Helpers::mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w) {
	int tmp_screen;
	tmp_screen = SCREEN_SIZE / 2;
	*new_x = (x - tmp_screen) / tmp_screen;
	*new_y = -((y - tmp_screen) / tmp_screen);
	*new_z = z;
	*new_w = w;
}