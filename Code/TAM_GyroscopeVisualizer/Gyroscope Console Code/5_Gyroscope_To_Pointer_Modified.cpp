#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <cmath>
#include "yei_threespace_api.h"

const float M_PI = 3.14159263538979323846f;

class Vector3
{
	public:
		float x, y, z;

		Vector3() : x(0), y(0), z(0) {};
		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

		float dot(Vector3 other);
		Vector3 cross(Vector3 other);

		Vector3 operator * (float scalar);
		Vector3 operator + (Vector3 other);
};

float Vector3::dot(Vector3 other)
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(Vector3 other)
{
	return Vector3(y * other.z - other.y * z,
		x * other.z - other.x * z,
		x * other.y - other.x * y);
}

Vector3 Vector3::operator*(float scalar)
{
	return Vector3(scalar * x, scalar * y, scalar * z);
}

Vector3 Vector3::operator+(Vector3 other)
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

float radToDeg(float rad)
{
	return (rad * 180.0f) / M_PI;
}

// Rotate the 3-D Vector v according to the quaternion quat
Vector3 quatRotateVec(Vector3 v, float *quat)
{
	Vector3 u(quat[1], quat[2], quat[3]);
	float s = quat[0];

	return (u * (2.0f * u.dot(v))) + (v *  (s * s - u.dot(v))) + (u.cross(v) * (2.0f * s));
}

void mouseClick(float x, float y, unsigned char button)
{
	INPUT input;
	DWORD buttonPressed;
	int cButton = static_cast<int>(button);

	if (cButton == 1)
		buttonPressed = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	else
		buttonPressed = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;

	ZeroMemory(&input, sizeof(input));

	input.type = INPUT_MOUSE;
	input.mi.dx = x; 
	input.mi.dy = y;
	input.mi.mouseData = 0;
	input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | buttonPressed;

	SendInput(1, &input, sizeof(input));
}

int main()
{
	TSS_Device_Id  device;
	TSS_Device_Id  d_device;
	TSS_Error tss_error;
	TSS_ComPort comport;
	unsigned int timestamp;

	float quat[4];
	float euler[3];
	float zDistance = 1280.0;

	const int N = 10;
	int index = 0;
	std::pair<float, float> point[N];
	float averageX = 0.0, averageY = 0.0;

	unsigned char button;

	if (tss_getComPorts(&comport, 1, 0, TSS_FIND_DNG))
	{
		d_device = tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if (d_device == TSS_NO_DEVICE_ID)
		{
			printf("Failed to create a sensor on %s\n", comport.com_port);
			return 1;
		}

		tss_getSensorFromDongle(d_device, 0, &device);

		if (device == TSS_NO_DEVICE_ID)
		{
			printf("Failed to create a wireless sensor on\n");
			return 1;
		}

	}
	else
	{
		printf("No sensors found\n");
		return 1;
	}

	tss_tareWithCurrentOrientation(device, &timestamp);

	while (true)
	{
		tss_error = tss_getTaredOrientationAsQuaternion(device, quat, &timestamp);
		if (tss_error)
			break;

		Vector3 forwardVector(0, 0, 1);
		Vector3 rotatedVector = quatRotateVec(forwardVector, quat);

		float scaleFactor = zDistance / rotatedVector.z;

		Vector3 scaledVector = rotatedVector * scaleFactor;

		float xCollision = scaledVector.x;
		float yCollision = scaledVector.y;

		tss_getTaredOrientationAsEulerAngles(device, euler, &timestamp);
		float rollAngle = radToDeg(euler[2]);

		// Average points in order to smooth out mouse movement (Can we do better?)
		if (index < N)
		{
			point[index].first = xCollision;
			point[index].second = yCollision;

			averageX += point[index].first;
			averageY += point[index].second;

			index++;
		}
		else
		{
			averageX /= N; 
			averageY /= N;

			SetCursorPos(averageX, averageY);

			index = 0;
		}
		// end of averaging code

		// Uncomment the line below to use gyroscope as mouse (may be difficult to maneuver)
		//SetCursorPos(xCollision, yCollision);

		tss_getButtonState(device, &button, &timestamp);
		if (button)
		{
			mouseClick(xCollision, yCollision, button);
			Sleep(100);
		}

		printf("X: %f, Y: %f, RollAngle: %f\n", xCollision, yCollision, rollAngle);

		if (_kbhit())
			break;
	}

	return 0;
}