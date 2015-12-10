#include "GLWindow.h"

int width = 800;
int height = 800;
GLWindow::GLWindow()
{
	setWindowTitle("Kinect");
	resize(800, 800);
	initializer();



}
void GLWindow::paintGL()
{

	kinectUpdate();


}
void GLWindow::resizeGL(int width, int height){
	TAM::VisualizerHandler<TAM::Shape>::resizeGL(NULL, width, height);
}
void GLWindow::initializeGL(){
	TAM::VisualizerHandler<TAM::Shape>::initGL(NULL, 0.0f, 0.0f, 0.0f, 1.0f);
	timer.start(1000 / fps, Qt::TimerType::PreciseTimer, this);

}
void GLWindow::draw(std::list<myWrapper*> line){

	foreach(myWrapper *a, line)
	{

		a->draw();

	}
	
	
}

void GLWindow::initializer()
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

void GLWindow::kinectUpdate(){
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
				{

					hr = bodies[i]->GetJoints(_countof(joints), joints); //get the  joints of the first body

					if (SUCCEEDED(hr)){
						//std::list <myWrapper*> bones;

						// Draw the bones

						// Torso
						drawBone( joints[JointType_Head], joints[JointType_Neck]);
						drawBone(joints[JointType_Neck], joints[JointType_SpineShoulder]);
						drawBone(joints[JointType_SpineShoulder], joints [JointType_SpineMid]);
						drawBone(joints[JointType_SpineMid], joints[JointType_SpineBase]);
						drawBone(joints[JointType_SpineShoulder], joints [JointType_ShoulderRight]);
						drawBone(joints[JointType_SpineShoulder], joints[JointType_ShoulderLeft]);
						drawBone(joints[JointType_SpineBase], joints[JointType_HipRight]);
						drawBone(joints[JointType_SpineBase], joints[JointType_HipLeft]);

						// Right Arm    
						drawBone(joints[JointType_ShoulderRight], joints[JointType_ElbowRight]);
						drawBone(joints[JointType_ElbowRight], joints[JointType_WristRight]);
						drawBone(joints [JointType_WristRight], joints [JointType_HandRight]);
						drawBone(joints[JointType_HandRight], joints[JointType_HandTipRight]);
						drawBone(joints[JointType_WristRight], joints[JointType_ThumbRight]);

						// Left Arm
						drawBone(joints[JointType_ShoulderLeft], joints[JointType_ElbowLeft]);
						drawBone(joints[JointType_ElbowLeft], joints[JointType_WristLeft]);
						drawBone(joints [JointType_WristLeft], joints[JointType_HandLeft]);
						drawBone(joints[JointType_HandLeft], joints[JointType_HandTipLeft]);
						drawBone(joints[JointType_WristLeft], joints [JointType_ThumbLeft]);

						// Right Leg
						drawBone(joints[JointType_HipRight], joints[JointType_KneeRight]);
						drawBone(joints [JointType_KneeRight], joints [JointType_AnkleRight]);
						drawBone(joints[JointType_AnkleRight], joints [JointType_FootRight]);

						// Left Leg
						drawBone(joints[JointType_HipLeft], joints [JointType_KneeLeft]);
						drawBone(joints [JointType_KneeLeft], joints [JointType_AnkleLeft]);
						drawBone(joints[JointType_AnkleLeft], joints[JointType_FootLeft]);

						}
						isDrawing = true;

						TAM::VisualizerHandler<myWrapper>::paintGL(&GLWindow::draw, bones);
						isDrawing = false;

					}

				}
			
			bodies[i]->Release();


		}

		//Sleep(35);
		pBodyFrame->Release();
	}
	bones.clear();
}

void GLWindow::drawBone(Joint joint0, Joint joint1)
{
	TrackingState joint0State = joint0.TrackingState;
	TrackingState joint1State = joint1.TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
		return;
	}

	DepthSpacePoint depthPoint0 = { 0 };
	DepthSpacePoint depthPoint1 = { 0 };
	kinectMapper->MapCameraPointToDepthSpace(joint0.Position, &depthPoint0);
	kinectMapper->MapCameraPointToDepthSpace(joint1.Position, &depthPoint1);
	myBody *newBod = new myBody();
	newBod->setX(depthPoint0.X);
	newBod->setY(depthPoint0.Y);
	newBod->setendX(depthPoint1.X);
	newBod->setendY(depthPoint1.Y);
	bones.push_back(newBod);
	}
void GLWindow::timerEvent(QTimerEvent *) {
	// If you're no longer drawing, draw next frame
	// If you're in playback mode, let the playback call when to draw next frame

	if (!isDrawing){
		update();
	}
}


GLWindow::~GLWindow(){

}
