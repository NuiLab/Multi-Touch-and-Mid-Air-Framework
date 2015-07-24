#include "yei_threespace_api.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

const float M_PI = 3.14159263538979323846;

float radToDeg(float rad)
{
	return (rad * 180.0) / M_PI;
}

int main()
{
	TSS_Device_Id  device;
	TSS_Device_Id  d_device;
	TSS_Error tss_error;
	TSS_ComPort comport;
	unsigned int timestamp;

	float angle[3];
	float pitch, roll, heading;

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

	printf("Streaming...\n");
	while (true)
	{
		tss_getUntaredOrientationAsEulerAngles(device, angle, &timestamp);

		heading = radToDeg(angle[1]);

		if (heading < 0)
			heading += 360.0;

		pitch = -radToDeg(angle[0]);
		roll = -radToDeg(angle[2]);

		printf("Heading: %f, Pitch: %f, Roll: %f\n", heading, pitch, roll);

		if (_kbhit())
			break;
	}

	return 0;
}