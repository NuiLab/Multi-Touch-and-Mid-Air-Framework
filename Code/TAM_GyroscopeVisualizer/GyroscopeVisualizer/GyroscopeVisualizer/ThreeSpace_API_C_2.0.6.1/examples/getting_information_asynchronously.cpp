#include "yei_threespace_api.h"
#include <stdio.h>
#include <Windows.h>

// When getting stream data use a packed structure
#pragma pack(push,1)
typedef struct {
    float quat[4];
    float accel[3];
} stream_packet;
#pragma pack(pop)


void CALLBACK DataPrinter(TSS_Device_Id device, char * output_data,
                          unsigned int output_data_len,  unsigned int * timestamp){
	if(output_data_len == sizeof(float)*9){
		float * float_data = (float *)output_data;
		printf("t:%8u ", *timestamp);
		printf("\n    Gyro: %f,%f,%f\n    Accel:%f,%f,%f\n    Comps:%f,%f,%f\n", float_data[0],
												   float_data[1],
												   float_data[2],
												   float_data[3],
												   float_data[4],
												   float_data[5],
												   float_data[6],
												   float_data[7],
												   float_data[8]);
	}
}
 
int main(){
	TSS_Device_Id  device;
	//TSS_Device_Id  d_device; //Wireless sensor
	TSS_Error tss_error;
	TSS_ComPort comport;
    stream_packet packet;
	unsigned int timestamp;

    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);
    QueryPerformanceCounter(&t2);
	////Wired sensor
	
	if(tss_getComPorts(&comport,1,0,TSS_FIND_ALL_KNOWN^TSS_FIND_DNG)){
		device =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if( device == TSS_NO_DEVICE_ID){
			printf("Failed to create a sensor on %s\n",comport.com_port);
			return 1;
		}
		else{
			TSS_ComInfo com_info;
			if( tss_getTSDeviceInfo(device,&com_info) == TSS_NO_ERROR){
				printf("============(%s)=============\n",comport.com_port);
				printf("DeviceType:%s\nSerial:%08X\nHardwareVersion:%s\nFirmwareVersion:%s\nCompatibility:%d\n",
				   TSS_Type_String[com_info.device_type],
				   com_info.serial_number,
				   com_info.hardware_version,
				   com_info.firmware_version,
				   com_info.fw_compatibility);
				printf("================================\n");
			}
		}

	}
	else{
		printf("No sensors found\n");
		return 1;
	}
	

	////Wireless sensor
	//if(tss_getComPorts(&comport,1,0,TSS_FIND_DNG)){
	//	d_device =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
	//	if( d_device == TSS_NO_DEVICE_ID){
	//		printf("Failed to create a sensor on %s\n",comport.com_port);
	//		return 1;
	//	}
	//	tss_getSensorFromDongle(d_device,0,&device);
	//	if( device == TSS_NO_DEVICE_ID){
	//		printf("Failed to create a wireless sensor on\n");
	//		return 1;
	//	}

	//}
	//else{
	//	printf("No sensors found\n");
	//	return 1;
	//}

	tss_error = tss_setStreamingTiming(device,0, TSS_INFINITE_DURATION, 0, NULL);
	if(tss_error){
		printf("TSS_Error: %s\nFinished press Enter to continue", TSS_Error_String[tss_error]);
		getchar();
		return 0;
	}
	TSS_Stream_Command stream_slots[8] = { TSS_GET_TARED_ORIENTATION_AS_QUATERNION,TSS_GET_CORRECTED_LINEAR_ACCELERATION_IN_GLOBAL_SPACE,
											TSS_NULL, TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL};    
	tss_setStreamingSlots(device, stream_slots, NULL);
	tss_startStreaming(device,NULL);
	QueryPerformanceCounter(&t1);
	//Blocking streaming
    while( 1.0 > ((t2.QuadPart-t1.QuadPart)*1.0f/frequency.QuadPart)){
        QueryPerformanceCounter(&t2);
		tss_getLatestStreamData(device,(char*)&packet,sizeof(packet),1000,&timestamp);
        printf("t:%8u Quat: %f, %f, %f, %f\n             Accel: %f, %f, %f\n",	timestamp,
																	packet.quat[0], 
																	packet.quat[1],
																	packet.quat[2],
																	packet.quat[3],
																	packet.accel[0], 
																	packet.accel[1],
																	packet.accel[2]);
    }
    tss_stopStreaming(device,NULL);

	//Nonblocking streaming
	float eular[3];
	TSS_Stream_Command stream_slots2[8] = { TSS_GET_TARED_ORIENTATION_AS_EULER_ANGLES,TSS_NULL,TSS_NULL, TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL};    
	tss_setStreamingSlots(device, stream_slots2, NULL);
	tss_startStreaming(device,NULL);
	QueryPerformanceCounter(&t1);
    while( 1.0 > ((t2.QuadPart-t1.QuadPart)*1.0f/frequency.QuadPart)){
        QueryPerformanceCounter(&t2);
		Sleep(100);
		tss_error = tss_getLastStreamData(device,(char*)&eular,sizeof(eular),&timestamp);
		if(tss_error != TSS_NO_ERROR){ //if tss_getLastStreamData immediately after start stream the data may not be ready 
			continue;
		}
        printf("t:%8u Eular: %f, %f, %f, \n",	timestamp,
												eular[0], 
												eular[1],
												eular[2]);
    }
    tss_stopStreaming(device,NULL);

	//Callback streaming
	TSS_Stream_Command stream_slots3[8] = { TSS_GET_ALL_CORRECTED_COMPONENT_SENSOR_DATA,TSS_NULL,TSS_NULL, TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL,TSS_NULL};    
	tss_setStreamingSlots(device, stream_slots3, NULL);
	tss_setNewDataCallBack(device,DataPrinter);
	tss_startStreaming(device,NULL);
	Sleep(1000);
    tss_stopStreaming(device,NULL);
 
    printf("Finished press Enter to continue");
    getchar();
    return 0;
}