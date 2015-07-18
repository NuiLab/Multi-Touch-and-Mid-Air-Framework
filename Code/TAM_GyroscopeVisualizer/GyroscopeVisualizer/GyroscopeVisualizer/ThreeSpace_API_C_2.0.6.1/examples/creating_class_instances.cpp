// This example finds and creates the device and returns a TSS_ID which is
// needed to call many functions in the API
#include "yei_threespace_api.h"
#include <stdio.h>

int main(){
	// If the COM port is already known and the device type is known for the 3-Space
	// Sensor device, we can just create the appropriate instance without doing 
	// a search.
	TSS_Device_Id  device;
	const char * com_port = "COM15";
	printf("====Creating TS Devices on %s ====\n", com_port);
	device = tss_createTSDeviceStr(com_port, TSS_TIMESTAMP_SENSOR);
	if( device == TSS_NO_DEVICE_ID){
		printf("Could not create device\n");
	}
	else{
		TSS_ComInfo com_info;
		if( tss_getTSDeviceInfo(device,&com_info) == TSS_NO_ERROR){
			printf("============(%s)=============\n",com_port);
			printf("DeviceType:%s\nSerial:%08X\nHardwareVersion:%s\nFirmwareVersion:%s\nCompatibility:%d\n",
               TSS_Type_String[com_info.device_type],
			   com_info.serial_number,
			   com_info.hardware_version,
			   com_info.firmware_version,
			   com_info.fw_compatibility);
			printf("================================\n");
		}
	}
	tss_closeTSDevice(device);

// Single comport var, loop through all comports (getfirst get next equivalent)

// Gets the comports of YEI devices and other available com ports
//  @param ports_vec the list(or single) of comport structures to return the results
//  @param ports_vec_size the size of the list entered
//  @param offset this allows you to call this function multiple times to get all the comports, increment the offest to continue getting more devices
//  @param filter pass in a combination of TSS_Find enums you want to search for
//  @return the number of comports found or equal to the size of ports_vec_size
	printf("====Creating Known TS Devices (single comport var) ====\n");
	TSS_ComPort comport;
	int offset=0;
	// API 2.0 CHANGE!!!!!!
	// TSS_FIND_ALL_KNOWN is all known YEI comport devices
	// TSS_FIND_ALL is all comports including TSS_FIND_UNKNOWN
	while(tss_getComPorts(&comport,1,offset,TSS_FIND_ALL_KNOWN)){  
		device =tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if( device == TSS_NO_DEVICE_ID){
			printf("Failed to create a sensor on %s\n",comport.com_port);
			//return 1;
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
		tss_closeTSDevice(device);
		offset++;
	}

	printf("====Creating Known TS Devices ( comport array) ====\n");
	TSS_ComPort comport_list[20];
	int device_count =tss_getComPorts(comport_list,20,0,TSS_FIND_ALL_KNOWN);
	printf("Found %d Devices\n", device_count);
	for( int i=0; i< device_count; ++i){
		device =tss_createTSDeviceStr(comport_list[i].com_port, TSS_TIMESTAMP_SENSOR);
		if( device == TSS_NO_DEVICE_ID){
			printf("Failed to create a sensor on %s\n",comport_list[i].com_port);
			//return 1;
		}
		else{
			TSS_ComInfo com_info;
			if( tss_getTSDeviceInfo(device,&com_info) == TSS_NO_ERROR){
				printf("============(%s)=============\n",comport_list[i].com_port);
				printf("DeviceType:%s\nSerial:%08X\nHardwareVersion:%s\nFirmwareVersion:%s\nCompatibility:%d\n",
				   TSS_Type_String[com_info.device_type],
				   com_info.serial_number,
				   com_info.hardware_version,
				   com_info.firmware_version,
				   com_info.fw_compatibility);
				printf("================================\n");
			}
			else{
				printf("Error Occured calling tss_getTSDeviceInfo\n");
			}
		}
		tss_closeTSDevice(device);
	}

// This will write bytes to serial ports that are not recognized as 3-Space virtual comports. 
// If the 3-Space sensors is connected via a serial to usb adapter or a physical serial port this will allow you to get information on what kind of sensor is connected and other useful information
	printf("====Checking Unknown Devices  ====\n");
	device_count =tss_getComPorts(comport_list,20,0,TSS_FIND_UNKNOWN);
	printf("Found %d Devices\n", device_count);
	for( int i=0; i< device_count; ++i){
		TSS_ComInfo com_info;
		TSS_Error error= tss_getTSDeviceInfoFromComPort(comport_list[i].com_port,&com_info);
		if( error == TSS_NO_ERROR){
			printf("============(%s)=============\n",comport_list[i].com_port);
			printf("DeviceType:%s\nSerial:%08X\nHardwareVersion:%s\nFirmwareVersion:%s\nCompatibility:%d\n",
               TSS_Type_String[com_info.device_type],
			   com_info.serial_number,
			   com_info.hardware_version,
			   com_info.firmware_version,
			   com_info.fw_compatibility);
			printf("================================\n");
		}
		else{
			printf("Failed to communicate on %s\n",comport_list[i].com_port);
		}
	}
	printf("Finished press Enter to continue");
	getchar();
	return 0;
}