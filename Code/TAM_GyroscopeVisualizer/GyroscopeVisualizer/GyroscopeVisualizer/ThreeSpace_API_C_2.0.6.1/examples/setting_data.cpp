//This example demonstrates setting options on a sensor
//This is compatible with all sensors plugged in via USB or Bluetooth
//Will not work with the dongle or wireless sensor wirelessly see
// getting_information_wireless for a wireless example
//For Sensors plugged in with RS232 see creating_class_instances
#include "yei_threespace_api.h"
#include <stdio.h>
#include <Windows.h>

int main(){
	TSS_Device_Id  device;
	TSS_Error tss_error;
	TSS_ComPort comport;

	if(tss_getComPorts(&comport,1,0,TSS_FIND_ALL_KNOWN^TSS_FIND_DNG)){
		device =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if( device == TSS_NO_DEVICE_ID){
			printf("Failed to create a sensor on %s\n",comport.com_port);
			return 1;
		}
	}
	else{
		printf("No sensors found\n");
		return 1;
	}
	printf("==================================================\n");
	printf("Setting the tared data of the device to an arbitrary quaternion.\n");
	float in_quat[4];
	in_quat[0] = 0.7071f;
	in_quat[1] = 0.0f;
	in_quat[2] = 0.7071f;
	in_quat[3] = 0.0f;
	tss_error = tss_tareWithQuaternion(device,in_quat, NULL);
	//Checking the error returned from the function, typicly succeed as long as the
	//parameters are valid on a plugged in sensor, functions sent to a sensor
	//wirelessly should allways be checked
	if(!tss_error){
		float out_quat[4];
		out_quat[0] = 666;
		//Note this is the only function in ther API that doesnt match the description in the docs due to the 
		//simularity to GetTaredOrientationAsQuaternion causing frequent auto complete mistakes.
		tss_error= tss_getTareAsQuaternion(device, out_quat, NULL);
		if(!tss_error){
			printf("TareQuat:%f, %f, %f, %f\n", out_quat[0],
												out_quat[1],
												out_quat[2],
												out_quat[3]);
		}
		else{
			printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
		}
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}
	printf("==================================================\n");
	printf("Setting the LED color of the device to RED.\n");
	float red[3]={ 1.0f, 0.0f, 0.0f};
	tss_error= tss_setLEDColor(device, red, NULL);
	if(!tss_error){
		printf("LED should be be RED\n");
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}
	Sleep(2000);
	printf("==================================================\n");
	printf("Setting the LED color of the device to BLUE.\n");
	float blue[3]={ 0.0f, 0.0f, 1.0f};
	tss_error= tss_setLEDColor(device, blue, NULL);
	if(!tss_error){
		printf("LED should be be BLUE\n");
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}

	//To save settings on the sensor call commitSettings
	//tss_commitSettings(device, NULL);

	tss_closeTSDevice(device);

	printf("Finished press Enter to continue");
	getchar();
	return 0;
}