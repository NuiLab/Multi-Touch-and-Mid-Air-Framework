#include "yei_threespace_api.h"
#include <stdio.h>
#include <Windows.h>

int main()
{
	TSS_Device_Id  device;
	TSS_Device_Id  d_device;
	TSS_Error tss_error;
	TSS_ComPort comport;
	unsigned int timestamp;
	float quat[4];

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

	tss_error = tss_getTaredOrientationAsQuaternion(device, quat, &timestamp);
	if (!tss_error)
		printf("Orientation Quaternion: (%f, %f, %f, %f)\n", quat[0], quat[1], quat[2], quat[3]);
	else
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);

	getchar();
	return 0;
}