#include "myWrapper.h"



void myBody:: draw() {
	//BOT in y  es el punto bajo
	//BOT in x es origen
	TAM::InputMapper newInput(&mapper, x, y, 0.0f, 0.0f);
	TAM::InputMapper inputEnd(&mapper, endX, endY, 0.0f, 0.0f);
	TAM::Circle newCircle(800, 800, 0.01f, 0, 255, 0, 1.0f, true);

	TAM::Line newLine(0.0f, 0.0f, 255, 1.0f, 10);
	newLine.setEndPoint(inputEnd.getXCoordinate() , inputEnd.getYCoordinate() , inputEnd.getZCoordinate()); //final point
	TAM::InputMapper circleInput = newInput;
	//circle
	newCircle.draw(newInput);
	newLine.draw(newInput); //origrn

	//newLine.setEndPoint(endX, endY, newInput.getZCoordinate()); //final point
	//newLine.compound(TAM::BOT);

	//newLine.setEndPoint(newInput.getXCoordinate() - 0.5f, newInput.getYCoordinate() +0.5f, newInput.getZCoordinate()); //final point
	//newLine.compound(TAM::TOP);

	//newLine.setEndPoint(newInput.getXCoordinate()  , newInput.getYCoordinate() + 0.5f, newInput.getZCoordinate()); //final point
	//newLine.compound(TAM::TOP);

}
void myBody::setX(float x){
	this->x = x;

}
void myBody::setY(float y){
	this->y = y;
}

void myBody::setendX(float x){
	endX=x;
}
void myBody::setendY(float y){
	endY = y;
}
void mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w) {
	int tmp_screen;
	tmp_screen = 200 ;
	*new_x = (x - tmp_screen) /tmp_screen;
	*new_y = -((y - tmp_screen) /tmp_screen);
	*new_z = z;
	*new_w = w;
}
