//This example demonstrates getting basic data from the sensor
//This is compatible with all sensors plugged in via USB or Bluetooth
//Will not work with the dongle or wireless sensor wirelessly see
// getting_information_wireless for a wireless example
//For Sensors plugged in with RS232 see creating_class_instances
#include "yei_threespace_api.h"
#include <stdio.h>

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
	printf("Getting the filtered tared quaternion orientation.(xyzw)\n");
	float quat[4];
	unsigned int timestamp;
	tss_error= tss_getTaredOrientationAsQuaternion(device, quat, &timestamp);
	if(!tss_error){
		printf("Quaternion: %f, %f, %f, %f Timestamp=%u\n", quat[0], quat[1], quat[2] ,quat[3], timestamp);
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}
	printf("==================================================\n");
	printf("Getting the Corrected Component Sensor Data.\n");
	float gyro[3];
	float accel[3];
	float compass[3];
	tss_error= tss_getAllCorrectedComponentSensorData(device, gyro, accel, compass, NULL);
	if(!tss_error){
		printf("Gyro:  %f, %f, %f\n", gyro[0],gyro[1],gyro[2]);
		printf("Accel: %f, %f, %f\n", accel[0],accel[1],accel[2]);
		printf("Comp:  %f, %f, %f\n", compass[0],compass[1],compass[2]);
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}
	printf("==================================================\n");
	printf("Getting the LED color  of the device.\n");
	float color[3];
	tss_error= tss_getLEDColor(device, color, NULL);
	if(!tss_error){
		printf("Color: %f, %f, %f\n", color[0], color[1], color[2]);
	}
	else{
		printf("TSS_Error: %s\n", TSS_Error_String[tss_error]);
	}
	printf("==================================================\n");
	tss_closeTSDevice(device);

	printf("Finished press Enter to continue");
	getchar();
	return 0;
}