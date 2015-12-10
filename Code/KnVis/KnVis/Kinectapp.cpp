#include "Kinectapp.h"


Kinectapp::Kinectapp()
{
	initializer();

}
void Kinectapp:: initializer()
{


	body_source = nullptr;
	body_reader = nullptr;
	//b_frame = nullptr;
	hr = GetDefaultKinectSensor(&sensor);

	if (FAILED(hr))
	{
		std::cout << "Failed to initailize sensor" << std::endl;
		return;
	}
	hr = sensor->Open();
	if (FAILED(hr))
	{
		std::cout << "Unable to Open the Sensor" << std::endl;
		return;
	}
	hr = sensor->get_CoordinateMapper(&kinectMapper);
	if (FAILED(hr))
	{
		std::cout << "Unable to mapp the coordinates" << std::endl;
	}

	hr = sensor->get_BodyFrameSource(&body_source);
	if (FAILED(hr))
	{
		std::cout << "Unable to initailize Body Frame Source" << std::endl;
		return;
	}
	hr = body_source->OpenReader(&body_reader);
	if (FAILED(hr))
	{
		std::cout << "Unable to open Body Reader" << std::endl;
		return;
	}

	body_source->Release();
}
void Kinectapp::update(){
	if (!body_reader)
	{
		return;
	}
	IBodyFrame* pBodyFrame = NULL;
	HRESULT hr1 = body_reader->AcquireLatestFrame(&pBodyFrame);
	//body_reader->Release();
	if (FAILED(hr1))
	{

		//setcolor(12, 9);
		//setConsoleXY(0, 0);
		std::cout << "Failed to get Frame" << std::endl;
		return;
	}
	//INT64 nTime = 0;
	//std::cout << "We got the Frame!" << std::endl;
	//hr = pBodyFrame->get_RelativeTime(&nTime);
	IBody *bodies[BODY_COUNT] = { 0 };//CAN READ UP TO 6 BODIES
	if (SUCCEEDED(hr1))
	{
		//Joint joints[JointType_Count];
		pBodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
		for (int i = 0; i < _countof(bodies); i++)
		{
			IBody *body = bodies[i];
			if (body){
				BOOLEAN tracked = false;
				hr = body->get_IsTracked(&tracked);
				if (SUCCEEDED(hr) && tracked)
					hr = bodies[i]->GetJoints(_countof(joints), joints);
				//draw this body if joints are tracked
				if (SUCCEEDED(hr))
					//for (int j = 0; j < (_countof(joints) - 1); j++){

					//	TrackingState joint0 = joints[j].TrackingState;
					//	TrackingState joint1 = joints[j + 1].TrackingState;
					//	if ((joint0 == TrackingState_NotTracked) || (joint1 == TrackingState_NotTracked))
					//	{
					//		break;
					//	}


					//	// We assume all drawn bones are inferred unless BOTH joints are tracked
					//	//if ((joint0 == TrackingState_Tracked) && (joint1 == TrackingState_Tracked))
					//	else
					//	{
					//		DepthSpacePoint depthPoint0 = { 0 };
					//		DepthSpacePoint depthPoint1 = { 0 };

					//		kinectMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthPoint0);
					//		kinectMapper->MapCameraPointToDepthSpace(joints[j + 1].Position, &depthPoint1);

					//		//trig->setX(depthPoint0.X);
					//		//trig->setY(depthPoint0.Y);
					//		//trig->setX(depthPoint1.X);
					//		//trig->setY(depthPoint1.Y);
					//		std::cout << j << std::endl;
					//		std::cout << depthPoint0.X << std::endl;
					//		std::cout << depthPoint0.Y << std::endl;
					//		std::cout << depthPoint1.X << std::endl;
					//		std::cout << depthPoint1.Y << std::endl;


					//		


					//	}
					//	
					//}
					;
			}
			bodies[i]->Release();


		}

		//Sleep(35);
		pBodyFrame->Release();
	}
}


Kinectapp::~Kinectapp()
{
}
