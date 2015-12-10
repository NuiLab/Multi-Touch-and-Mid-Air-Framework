#pragma once
#include <Kinect.h>
#include<iostream>
#include <list>
#include "consoleColors.h"
class Kinectapp
{
public:
	Kinectapp();
	~Kinectapp();
	void initializer();
	void update();
	
	
private:
	IKinectSensor * sensor;
	IBodyFrameSource *body_source;
	IBodyFrameReader* body_reader;
	HRESULT hr;
	ICoordinateMapper *kinectMapper;
	Joint joints[JointType_Count];



};

