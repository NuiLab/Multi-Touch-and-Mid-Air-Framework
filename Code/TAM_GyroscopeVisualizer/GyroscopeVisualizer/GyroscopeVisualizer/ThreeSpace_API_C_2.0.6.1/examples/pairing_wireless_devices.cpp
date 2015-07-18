//This is example requires a 3-Space Wireless and Dongle
//Dongle and wireless sensor must be plugged into the  same computer
//This example pairs the first wireless sensor it finds to the first dongle
// and assigns it to slot 0 in the logical id table and commits settings
//If using bluetooth read the 3-space bluetooth quickstart for pairing
#include "yei_threespace_api.h"
#include <stdio.h>

int main(){
	//TSS_ID device = NO_DEVICE_ID;
	//TSS_ID dongle = NO_DEVICE_ID;
	//ComPort comport;

	//if(!getFirstAvailableTSSComPort(&comport, TSS_FIND_WL)){
	//	device =createTSDevice(comport);
	//	if( device == NO_DEVICE_ID){
	//		printf("Failed to create a sensor on %s\n",comport.com_port);
	//		return 1;
	//	}
	//}
	//else{
	//	printf("No Wireless Sensor found\n");
	//	return 1;
	//}
	//if(!getFirstAvailableTSSComPort(&comport, TSS_FIND_DNG)){
	//	dongle =createTSDevice(comport);
	//	if( dongle == NO_DEVICE_ID){
	//		printf("Failed to create a sensor on %s\n",comport.com_port);
	//		return 1;
	//	}
	//}
	//else{
	//	printf("No Dongle found\n");
	//	return 1;
	//}

	TSS_Device_Id  wl_sensor;
	TSS_Device_Id  wl_sensor_wireless;
	TSS_Device_Id  dongle;
	TSS_Error tss_error;
	TSS_ComPort comport;

	if(tss_getComPorts(&comport,1,0,TSS_FIND_DNG)){
		dongle =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if( dongle == TSS_NO_DEVICE_ID){
			printf("Failed to create a dongle on %s\n",comport.com_port);
			return 1;
		}
	}

	if(tss_getComPorts(&comport,1,0,TSS_FIND_WL)){
		wl_sensor =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if( wl_sensor == TSS_NO_DEVICE_ID){
			printf("Failed to create a wireless sensor on %s\n",comport.com_port);
			return 1;
		}
	}

	//Pairing requires 3 things, the dongle and wireless sensor pan_id, 
	// channel much match and the wireless sensor serial need
	unsigned short pan_id = 1234;
	unsigned char channel = 25; //Must be an integer value between 11 and 26
	unsigned char logical_id = 0;
	unsigned int serial_number;

	printf("Setting the Pan id to: %d, and channel to: %d\n", pan_id, channel);
	//To simplify the example functions are not validated look at setting_data
	tss_error =tss_getSerialNumber(wl_sensor, &serial_number, NULL);
	if(tss_error){
		printf("TSS_Error: %s\nFinished press Enter to continue", TSS_Error_String[tss_error]);
		getchar();
		return 0;
	}
	tss_setWirelessPanID(wl_sensor, pan_id, NULL);
	tss_setWirelessChannel(wl_sensor, channel, NULL);

	tss_setWirelessPanID(dongle, pan_id, NULL);
	tss_setWirelessChannel(dongle, channel, NULL);
	tss_setSerialNumberAtLogicalID(dongle, logical_id, serial_number,NULL);
	tss_getSensorFromDongle(dongle, logical_id, &wl_sensor_wireless);

	if( wl_sensor_wireless != TSS_NO_DEVICE_ID){
		printf("Sensor pair successful!!!\n");
		printf("Commiting settings\n");
		tss_commitWirelessSettings(wl_sensor, NULL);
		tss_commitWirelessSettings(dongle, NULL);
	}
	else{
		printf("Failed to pair sensor\n");
	}

	printf("Finished press Enter to continue");
	getchar();
	return 0;
}