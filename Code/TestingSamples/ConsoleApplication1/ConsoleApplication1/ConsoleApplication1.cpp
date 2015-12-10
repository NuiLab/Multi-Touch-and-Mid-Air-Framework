// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Kinect.h>
#include <iostream>
#include <map>
#include<string>
#include <Windows.h>
#include "consoleColors.h"
#include "api.h"
class Kinectapp
{
private:
	IKinectSensor * sensor;
	IBodyFrameSource *body_source;
	IBodyFrameReader* body_reader ;
	//IBodyFrame *b_frame ;
	HRESULT hr;
	//ICoordinateMapper *mapper;
	
	void update(){
		if (!body_reader)
		{
			return;
		}
		IBodyFrame* pBodyFrame = NULL;
		HRESULT hr1 = body_reader->AcquireLatestFrame(&pBodyFrame);
		if (FAILED(hr1))
		{
			
			setcolor(12, 9);
			setConsoleXY(0,0);
			std::cout << "Failed to get Frame" << std::endl;
			return;
		}
		//INT64 nTime = 0;
		//std::cout << "We got the Frame!" << std::endl;
		//hr = pBodyFrame->get_RelativeTime(&nTime);
		IBody *bodies[BODY_COUNT] = { 0 };//CAN READ UP TO 6 BODIES
		if (SUCCEEDED(hr1))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
			
		}
		if (SUCCEEDED(hr))
		{
			//std::cout << "We got bodies" << std::endl;
			std::map<int, std::string>map;
			map[0]  = "JointType_SpineBas";
			map[1]  = "JointType_SpineMid";
			map[2]  = "JointType_Neck";
			map[3]  = "JointType_Head";
			map[4]  = "JointType_ShoulderLeft";
			map[5]  = "JointType_ElbowLeft";
			map[6]  = "JointType_WristLeft";
			map[7]  = "JointType_HandLeft";
			map[8]  = "JointType_ShoulderRight";
			map[9]  = "JointType_ElbowRight";
			map[10] = "JointType_WristRight";
			map[11] = "JointType_HandRight";
			map[12] = "JointType_HipLeft";
			map[13] = "JointType_KneeLeft";
			map[14] = "JointType_AnkleLeft";
			map[15] = "JointType_FootLeft";
			map[16] = "JointType_HipRight";
			map[17] = "JointType_KneeRight";
			map[18] = "JointType_AnkleRight";
			map[19] = "JointType_FootRight";
			map[20] = "JointType_SpineShoulder";
			map[21] = "JointType_HandTipLeft";
			map[22] = "JointType_ThumbLeft";
			map[23] = "JointType_HandTipRight";
			map[24] = "JointType_ThumbRight";
			auto it = map.begin();
			for (int i = 0; i < _countof(bodies); ++i){
				IBody* body = bodies[i];
				if (body)
				{
					BOOLEAN bTracked = false;
					hr = body->get_IsTracked(&bTracked);
					if (SUCCEEDED(hr)&&bTracked)
					{
						Joint joints[JointType_Count];
						JointType joint_type[JointType_Count];
						int j = 0;
						for (int i = 0; i < JointType_Count; ++i)
						{
							joints[i].JointType;
							float myX = joints[i].Position.X;
							float myY = joints[i].Position.Y;
							float myZ = joints[i].Position.Z;
							if (it != map.end()){
								setcolor(i % 7, (i % 7) + 8);
								setConsoleXY(0, i);
								std::cout << it->second << "X Position: " << myX << std::endl;
								std::cout << it->second << "Y Position: " << myY << std::endl;
								std::cout << it->second << "Z Position: " << myZ << std::endl;
							++it;
							++j;
						}
						}
					}
				}
				//std::cout << "this is body: " << i << std::endl;
				bodies[i]->Release();
			}
		}
		Sleep(30);

		pBodyFrame->Release();
	}

public:
	~Kinectapp()
	{
		if (sensor != nullptr)sensor->Close();
	}

	//sensor initialization
	void initailizer()
	{
		sensor = nullptr;
		body_source = nullptr;
		body_reader = nullptr;
		//b_frame = nullptr;
		hr =GetDefaultKinectSensor(&sensor);
		if (FAILED(hr))
		{
			std::cout << "Failed to initailize sensor" << std::endl;
			return ;
		}
		hr = sensor->Open();
		if (FAILED(hr))
		{
			std::cout << "Unable to Open the Sensor" << std::endl;
			return;
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
	void run(){

		while (true)
		{

			update();

		}
	}


};

int main(int argc, _TCHAR* argv[])
{
	
	Kinectapp app;
	app.initailizer();
	app.run();
	std::cin.get();
	return 0;
}

