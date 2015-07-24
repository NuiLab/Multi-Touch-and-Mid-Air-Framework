#include "yei_threespace_api.h"
#include <stdio.h>
#include <windows.h>
#include <conio.h>

// When getting stream data use a packed structure
#pragma pack(push,1)
typedef struct 
{
	float quat[4];
} stream_packet;
#pragma pack(pop)

int main()
{
	TSS_Device_Id  device;
	TSS_Device_Id  d_device; //Wireless sensor
	TSS_Error tss_error;
	TSS_ComPort comport;
	stream_packet packet;
	unsigned int timestamp;

	//Wireless sensor
	if(tss_getComPorts(&comport,1,0,TSS_FIND_DNG))
	{
		d_device = tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if (d_device == TSS_NO_DEVICE_ID)
		{
			printf("Failed to create a sensor on %s\n",comport.com_port);
			return 1;
		}

		tss_getSensorFromDongle(d_device,0,&device);

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

	tss_error = tss_setStreamingTiming(device, 0, TSS_INFINITE_DURATION, 0, NULL);
	if (tss_error)
	{
		printf("TSS_Error: %s\nFinished press Enter to continue", TSS_Error_String[tss_error]);
		getchar();
		return 0;
	}

	TSS_Stream_Command stream_slots[8] = {TSS_GET_TARED_ORIENTATION_AS_QUATERNION, TSS_NULL, TSS_NULL, TSS_NULL, 
		TSS_NULL, TSS_NULL, TSS_NULL, TSS_NULL};

	tss_setStreamingSlots(device, stream_slots, NULL);
	tss_startStreaming(device, NULL);

	printf("Streaming...\n");
	while (true)
	{
		tss_getLatestStreamData(device, (char*)&packet, sizeof(packet), 1000, &timestamp);
		printf("t:%8u Quat: %f, %f, %f, %f\n", timestamp, packet.quat[0], packet.quat[1], packet.quat[2], packet.quat[3]);

		if (_kbhit())
			break;
	}

	tss_stopStreaming(device, NULL);
	return 0;
}