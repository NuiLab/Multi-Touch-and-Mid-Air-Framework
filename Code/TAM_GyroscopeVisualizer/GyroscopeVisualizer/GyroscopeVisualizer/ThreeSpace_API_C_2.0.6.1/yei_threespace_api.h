/********************************************//**
 * \file yei_threespace_api.h
 * \brief  ThreeSpace API 2.0
 * \author Chris George
 * \author Daniel Morrison
 * \copyright Copyright 1998-2014, YEI Corporation.
 *
 * The YEI 3-Space C API is released under the YEI 3-Space Open Source License, which allows for both
 * non-commercial use and commercial use with certain restrictions.
 *
 * For Non-Commercial Use, your use of Covered Works is governed by the GNU GPL v.3, subject to the YEI 3-Space Open
 * Source Licensing Overview and Definitions.
 *
 * For Commercial Use, a YEI Commercial/Redistribution License is required, pursuant to the YEI 3-Space Open Source
 * Licensing Overview and Definitions. Commercial Use, for the purposes of this License, means the use, reproduction
 * and/or Distribution, either directly or indirectly, of the Covered Works or any portion thereof, or a Compilation,
 * Improvement, or Modification, for Pecuniary Gain. A YEI Commercial/Redistribution License may or may not require
 * payment, depending upon the intended use.
 *
 * Full details of the YEI 3-Space Open Source License can be found in license.txt
 * License also available online at http://www.yeitechnology.com/yei-3-space-open-source-license
 ***********************************************/
#ifndef YEI_THREESPACE_API_H_INCLUDED
#define YEI_THREESPACE_API_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <math.h>


#ifdef __cplusplus
    extern "C"
    {
#endif

#if defined(__GNUC__)
#define _inline inline
#endif

#if defined(TSS_STATIC_LIB)
#define TSS_EXPORT
#elif defined(_MSC_VER)
#define TSS_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define TSS_EXPORT __attribute__ ((dllexport))
#endif

/********************************************//**
 * Macro for infinite duration for streaming setup.
 ***********************************************/
#define TSS_INFINITE_DURATION 0xffffffff

/********************************************//**
 * The default number of soft retries on wireless
 * If tss_resetThreeSpaceAPI is called retries will be set back to this value
 ***********************************************/
#define TSS_DEFAULT_WIRLESSS_RETRIES 5

/********************************************//**
 * The default baud rate that sensors will be opened with
 * If tss_resetThreeSpaceAPI is called baud rate will be set back to this value
 ***********************************************/
#define TSS_DEFAULT_BAUD_RATE 115200

/********************************************//**
 * YEI 3-Space API device identifier, a common parameter needed for all 3-Space API calls.
 ***********************************************/
typedef unsigned int TSS_Device_Id;

/********************************************//**
 * An enum expressing the masks used to quickly determine the type of a sensor based on their device ID.
 ***********************************************/
typedef enum TSS_Device_Id_Mask
{
    TSS_NO_DEVICE_ID    = 0x00800000,              /**< Invalid Device ID */
    TSS_BOOTLOADER_ID   = 0x01000000,              /**< Bootloader */
    TSS_DONGLE_ID       = 0x02000000,              /**< Wireless Dongle */
    TSS_USB_ID          = 0x04000000,              /**< USB, Micro USB, or Watertight USB */
    TSS_EMBEDDED_ID     = 0x08000000,              /**< Embedded */
    TSS_WIRELESS_ID     = 0x10000000,              /**< Wireless 2.4GHz DSSS plugged via USB */
    TSS_WIRELESS_W_ID   = 0x20000000,              /**< Wireless 2.4GHz DSSS communicating wirelessly */
    TSS_DATALOGGER_ID   = 0x40000000,              /**< Data-logging */
    TSS_BLUETOOTH_ID    = 0x80000000,              /**< Bluetooth */
    TSS_NO_DONGLE_ID    = 0xfd000000,              /**< Convenience ID, includes all but the Wireless Dongle */
    TSS_ALL_SENSORS_ID  = 0xff000000               /**< Convenience ID, includes all valid id masks */
}TSS_Device_Id_Mask;

/********************************************//**
 * An enum expressing API Timestamp Modes.
 ***********************************************/
typedef enum TSS_Timestamp_Mode{
    TSS_TIMESTAMP_NONE,   /**< Disables timestamp, timestamp will return 0 */
    TSS_TIMESTAMP_SENSOR, /**< 3-Space device's timestamp, this can be set with tss_updateCurrentTimestamp */
    TSS_TIMESTAMP_SYSTEM  /**< The data is timestamped on arrival to the system using the high-resolution performance counter */
}TSS_Timestamp_Mode;

/********************************************//**
 * An enum expressing the different types of errors a 3-Space API call can return.
 ***********************************************/
typedef enum TSS_Error
{
    TSS_NO_ERROR,                   /**< The API call successfully executed */
    TSS_ERROR_COMMAND_FAIL,         /**< The command returned a failed response */
    TSS_INVALID_COMMAND,            /**< The API call was made on a device type that does not support the attempted command */
    TSS_INVALID_ID,                 /**< The TSS_Device_Id parameter passed in to an API call is not associated with a connected 3-Space device */
    TSS_ERROR_PARAMETER,            /**< General parameter fail */
    TSS_ERROR_TIMEOUT,              /**< The command's timeout has been reached */
    TSS_ERROR_WRITE,                /**< The API call executed failed to write all the data necessary to execute the command to the intended 3-Space device */
    TSS_ERROR_READ,                 /**< The API call executed failed to read all the data necessary to execute the command to the intended 3-Space device */
    TSS_ERROR_STREAM_SLOTS_FULL,    /**< The 3-Space device's stream slots are full */
    TSS_ERROR_STREAM_CONFIG,        /**< The 3-Space device's stream configuration is corrupted */
    TSS_ERROR_MEMORY,               /**< A memory error occurred in the API */
    TSS_ERROR_FIRMWARE_INCOMPATIBLE /**< 3-Space device firmware does not support that command, firmware update highly recommended */
}TSS_Error;

static const char* const TSS_Error_String[] = {
    "TSS_NO_ERROR",
    "TSS_ERROR_COMMAND_FAIL",
    "TSS_INVALID_COMMAND",
    "TSS_INVALID_ID",
    "TSS_ERROR_PARAMETER",
    "TSS_ERROR_TIMEOUT",
    "TSS_ERROR_WRITE",
    "TSS_ERROR_READ",
    "TSS_ERROR_STREAM_SLOTS_FULL",
    "TSS_ERROR_STREAM_CONFIG",
    "TSS_ERROR_MEMORY",
    "TSS_ERROR_FIRMWARE_INCOMPATIBLE"
};

/********************************************//**
 * An enum denoting the compatibility level of the 3-Space device.
 ***********************************************/
typedef enum TSS_Firmware_Compatibility
{
    TSS_FW_NOT_COMPATIBLE,       /**< Firmware is not compatible with the API and should be updated */
    TSS_FW_20R7_COMPATIBLE,      /**< 3-Space device compatible with 2.0R7 commands and up */
    TSS_FW_20R10_COMPATIBLE,      /**< 3-Space device compatible with 2.0R10 commands and up */
    TSS_FW_20R13_COMPATIBLE      /**< 3-Space device compatible with 2.0R13 commands and up */
}TSS_Firmware_Compatibility;

static const char* const TSS_Firmware_Version_String[] = {
    "00Jan2000A00",
    "25Apr2013A00",
    "21Jun2013A00",
    "28Aug2013A00"
};

/********************************************//**
 * An enum expressing the command list of Streamable Commands.
 *
 * TSS_Stream_Command_Enum comtains the command bytes of streamable commands, 
 * the commands match up with the functions with similar names
 * 
 * Example: TSS_GET_TARED_ORIENTATION_AS_QUATERNION is tss_getTaredOrientationAsQuaternion
 ***********************************************/
typedef enum TSS_Stream_Command_Enum
{
    TSS_GET_TARED_ORIENTATION_AS_QUATERNION                 = 0x00, /**< \see tss_getTaredOrientationAsQuaternion */
    TSS_GET_TARED_ORIENTATION_AS_EULER_ANGLES               = 0x01, /**< \see tss_getTaredOrientationAsEulerAngles */
    TSS_GET_TARED_ORIENTATION_AS_ROTATION_MATRIX            = 0x02, /**< \see tss_getTaredOrientationAsRotationMatrix */
    TSS_GET_TARED_ORIENTATION_AS_AXIS_ANGLE                 = 0x03, /**< \see tss_getTaredOrientationAsAxisAngle */
    TSS_GET_TARED_ORIENTATION_AS_TWO_VECTOR                 = 0x04, /**< \see tss_getTaredOrientationAsTwoVector */
    TSS_GET_DIFFERENCE_QUATERNION                           = 0x05, /**< \see tss_getDifferenceQuaternion */
    TSS_GET_UNTARED_ORIENTATION_AS_QUATERNION               = 0x06, /**< \see tss_getUntaredOrientationAsQuaternion */
    TSS_GET_UNTARED_ORIENTATION_AS_EULER_ANGLES             = 0x07, /**< \see tss_getUntaredOrientationAsEulerAngles */
    TSS_GET_UNTARED_ORIENTATION_AS_ROTATION_MATRIX          = 0x08, /**< \see tss_getUntaredOrientationAsRotationMatrix */
    TSS_GET_UNTARED_ORIENTATION_AS_AXIS_ANGLE               = 0x09, /**< \see tss_getUntaredOrientationAsAxisAngle */
    TSS_GET_UNTARED_ORIENTATION_AS_TWO_VECTOR               = 0x0a, /**< \see tss_getUntaredOrientationAsTwoVector */
    TSS_GET_TARED_TWO_VECTOR_IN_SENSOR_FRAME                = 0x0b, /**< \see tss_getTaredTwoVectorInSensorFrame */
    TSS_GET_UNTARED_TWO_VECTOR_IN_SENSOR_FRAME              = 0x0c, /**< \see tss_getUntaredTwoVectorInSensorFrame */
    TSS_GET_ALL_NORMALIZED_COMPONENT_SENSOR_DATA            = 0x20, /**< \see tss_getAllNormalizedComponentSensorData */
    TSS_GET_NORMALIZED_GYRO_RATE                            = 0x21, /**< \see tss_getNormalizedGyroRate */
    TSS_GET_NORMALIZED_ACCELEROMETER_VECTOR                 = 0x22, /**< \see tss_getNormalizedAccelerometerVector */
    TSS_GET_NORMALIZED_COMPASS_VECTOR                       = 0x23, /**< \see tss_getNormalizedCompassVector */
    TSS_GET_ALL_CORRECTED_COMPONENT_SENSOR_DATA             = 0x25, /**< \see tss_getAllCorrectedComponentSensorData */
    TSS_GET_CORRECTED_GYRO_RATE                             = 0x26, /**< \see tss_getCorrectedGyroRate */
    TSS_GET_CORRECTED_ACCELEROMETER_VECTOR                  = 0x27, /**< \see tss_getCorrectedAccelerometerVector */
    TSS_GET_CORRECTED_COMPASS_VECTOR                        = 0x28, /**< \see tss_getCorrectedCompassVector */
    TSS_GET_CORRECTED_LINEAR_ACCELERATION_IN_GLOBAL_SPACE   = 0x29, /**< \see tss_getCorrectedLinearAccelerationInGlobalSpace */
    TSS_GET_TEMPERATURE_C                                   = 0x2b, /**< \see tss_getTemperatureC */
    TSS_GET_TEMPERATURE_F                                   = 0x2c, /**< \see tss_getTemperatureF */
    TSS_GET_CONFIDENCE_FACTOR                               = 0x2d, /**< \see tss_getConfidenceFactor */
    TSS_GET_ALL_RAW_COMPONENT_SENSOR_DATA                   = 0x40, /**< \see tss_getAllRawComponentSensorData */
    TSS_GET_RAW_GYROSCOPE_RATE                              = 0x41, /**< \see tss_getRawGyroscopeRate */
    TSS_GET_RAW_ACCELEROMETER_DATA                          = 0x42, /**< \see tss_getRawAccelerometerData */
    TSS_GET_RAW_COMPASS_DATA                                = 0x43, /**< \see tss_getRawCompassData */
    TSS_GET_BATTERY_VOLTAGE                                 = 0xc9, /**< \see tss_getBatteryVoltage */
    TSS_GET_BATTERY_PERCENT_REMAINING                       = 0xca, /**< \see tss_getBatteryPercentRemaining */
    TSS_GET_BATTERY_STATUS                                  = 0xcb, /**< \see tss_getBatteryStatus */
    TSS_GET_BUTTON_STATE                                    = 0xfa, /**< \see tss_getButtonState */
    TSS_NULL                                                = 0xff  /**< TSS_NULL, all unused stream slots must be filled with TSS_NULL */
}TSS_Stream_Command_Enum;

/********************************************//**
 * YEI 3-Space API streamable command.
 ***********************************************/
typedef unsigned char TSS_Stream_Command;

/********************************************//**
 * An enum expressing the find flags for the tss_getComPorts filter parameter.
 ***********************************************/
typedef enum TSS_Find
{
    TSS_FIND_BTL        = 0x00000001, /**< Find Bootloader */
    TSS_FIND_USB        = 0x00000002, /**< Find USB, Micro USB, or Watertight USB */
    TSS_FIND_DNG        = 0x00000004, /**< Find Wireless Dongle */
    TSS_FIND_WL         = 0x00000008, /**< Find Wireless 2.4GHz DSSS plugged via USB */
    TSS_FIND_EM         = 0x00000010, /**< Find Embedded */
    TSS_FIND_DL         = 0x00000020, /**< Find Data-logging */
    TSS_FIND_BT         = 0x00000040, /**< Find Bluetooth */
    TSS_FIND_UNKNOWN    = 0x80000000, /**< Find serial ports that may have 3-Space devices connected to them */
    TSS_FIND_ALL_KNOWN  = 0x7fffffff, /**< Find all known 3-Space devices */
    TSS_FIND_ALL        = 0xffffffff  /**< Find all com ports including "unknown" serial ports that may have 3-Space devices connected */
}TSS_Find;

/********************************************//**
 * An enum expressing the types of 3-Space devices.
 ***********************************************/
typedef enum TSS_Type
{
    TSS_UNKNOWN,    /**< Device type was not able to be determined */
    TSS_BTL,        /**< Bootloader */
    TSS_USB,        /**< USB, Micro USB, or Watertight USB */
    TSS_DNG,        /**< Wireless Dongle */
    TSS_WL,         /**< Wireless 2.4GHz DSSS plugged via USB */
    TSS_WL_W,       /**< Wireless 2.4GHz DSSS communicating wirelessly */
    TSS_EM,         /**< Embedded */
    TSS_DL,         /**< Data-logging */
    TSS_BT,         /**< Bluetooth */
}TSS_Type;

static const char* const TSS_Type_String[] = {
    "TSS_UNKNOWN",
    "TSS_BTL",
    "TSS_USB",
    "TSS_DNG",
    "TSS_WL",        //wireless wired (connected to PC)
    "TSS_WL_W",    //wireless wireless
    "TSS_EM",
    "TSS_DL",
    "TSS_BT",
};

/********************************************//**
 * An enum expressing the alternate directions for each of the natural axes of the sensor.
 ***********************************************/
typedef enum TSS_Axis_Direction
{
    TSS_XYZ, /**< X: Right, Y: Up, Z: Forward (left-handed system, standard operation) */
    TSS_XZY, /**< X: Right, Y: Forward, Z: Up (right-handed system) */
    TSS_YXZ, /**< X: Up, Y: Right, Z: Forward (right-handed system) */
    TSS_YZX, /**< X: Forward, Y: Right, Z: Up (left-handed system) */
    TSS_ZXY, /**< X: Up, Y: Forward, Z: Right (left-handed system) */
    TSS_ZYX, /**< X: Forward, Y: Up, Z: Right (right-handed system) */
}TSS_Axis_Direction;

/********************************************//**
 * An enum expressing the different available filter modes.
 ***********************************************/
typedef enum TSS_Filter_Mode
{
    TSS_FILTER_IMU,                        /**< IMU Mode */
    TSS_FILTER_KALMAN,                     /**< Kalman Filtered Mode */
    TSS_FILTER_ALTERNATING_KALMAN,         /**< Alternating Kalman Filtered Mode */
    TSS_FILTER_COMPLEMENTARY,              /**< Complementary Filter Mode */
    TSS_FILTER_QUATERNION_GRADIENT_DECENT, /**< Quaternion Gradient Descent Filter Mode */
}TSS_Filter_Mode;

/********************************************//**
 * A structure that contains basic information about a com port.
 ***********************************************/
typedef struct TSS_ComPort
{
    char com_port[32];      /**< The com port string. */
    char friendly_name[64]; /**< The frienly name of the com port. */
    int sensor_type;        /**< The type of 3-Space device connected through the com port. */ //Change?
}TSS_ComPort;

/********************************************//**
 * A structure that contains information about the connected 3-Space device.
 ***********************************************/
typedef struct TSS_ComInfo
{
    TSS_Type device_type; /**< The type of 3-Space device connected through the com port. */
    unsigned int serial_number; /**< The serial number for the 3-Space device connected through the com port. */
    char firmware_version[13]; /**< The version of the firmware installed on the connected 3-Space device. */
    char hardware_version[33]; /**< The hardware revision and type of the connected 3-Space device. */
    TSS_Firmware_Compatibility fw_compatibility; /**< Firmware compatibility level (Note level may be lower than current if no functional changes were made). */
}TSS_ComInfo;

/********************************************//**
 * \brief Streaming data callback.
 *
 *  One of the methods of handling stream data is to create a callback, this is the template function.
 *  tss_setNewDataCallBack sets the callback.
 * \param device The device ID of the sensor.
 * \param output_data The data the sensor returned.
 * \param output_data_len The size of the data returned.
 * \param timestamp The timestamp in microseconds if timestamps are enabled.
 ***********************************************/
typedef void CALLBACK TSS_CallBack(TSS_Device_Id device, char * output_data, unsigned int output_data_len, unsigned int * timestamp);

// Allow to call those directly when not using a DLL
#ifdef TSS_STATIC_LIB
TSS_EXPORT int tss_initThreeSpaceAPI();
TSS_EXPORT int tss_delThreeSpaceAPI();
#endif // TSS_STATIC_LIB

//convenience functions
/********************************************//**
 * \brief Convenience function to parse axis_byte into its separate components.
 * \param axis_byte The byte returned from tss_getAxisDirections.
 * \param axis_order Writes the axis order of the natural axes of the sensor.
 * \param neg_x Writes whether the First resulting component is negative, 0 is false 1 is true.
 * \param neg_y Writes whether the Second resulting component is negative, 0 is false 1 is true.
 * \param neg_z Writes whether the Third resulting component is negative, 0 is false 1 is true.
 ***********************************************/
TSS_EXPORT void tss_parseAxisDirections(unsigned char axis_byte, TSS_Axis_Direction * axis_order, char * neg_x, char * neg_y, char * neg_z);

/********************************************//**
 * \brief Convenience function to generate an axis_byte from its separate components.
 * \param axis_order The axis order of the natural axes.
 * \param neg_x Whether the First resulting component is negative, 0 is false 1 is true.
 * \param neg_y Whether the Second resulting component is negative, 0 is false 1 is true.
 * \param neg_z Whether the Third resulting component is negative, 0 is false 1 is true.
 * \return The byte that can be sent to tss_setAxisDirections or stored.
 ***********************************************/
TSS_EXPORT unsigned char tss_generateAxisDirections(TSS_Axis_Direction axis_order, char neg_x, char neg_y, char neg_z);

//API specific functions
// Sets the amount of retries on wireless commands via software, note there is 3 hardware retries by default
/********************************************//**
 * \brief Sets the amount of retries on wireless commands via software.
 *
 * Note: There are 3 hardware retries by default in addition to the soft retries here.
 * \param retries The amount of soft retries.
 ***********************************************/
TSS_EXPORT void tss_setSoftwareWirelessRetries(unsigned int retries);

/********************************************//**
 * \brief Gets the amount of retries on wireless commands via software.
 * \return The amount of soft retries.
 ***********************************************/
TSS_EXPORT unsigned int tss_getSoftwareWirelessRetries();

/********************************************//**
 * \brief Sets the default baud rate for newly created 3-Space devices.
 * \param baud_rate The desired baud rate.
 ***********************************************/
TSS_EXPORT void tss_setDefaultCreateDeviceBaudRate(int baud_rate);

/********************************************//**
 * \brief Gets the default baud rate for newly created 3-Space devices.
 * \return The default baud rate.
 ***********************************************/
TSS_EXPORT int tss_getDefaultCreateDeviceBaudRate();

/********************************************//**
 * \brief Gets the com ports of YEI 3-Space devices and other available com ports.
 * \param ports_vec The list of (or single) TSS_ComPort structures to return the results.
 * \param ports_vec_size The size of the list entered.
 * \param offset This allows you to call this function multiple times to get all the com ports, increment the offset to continue getting more 3-Space devices.
 * \param filter Pass in a combination of TSS_Find enums you want to search for.
 * \return The number of com ports found or equal to the size of ports_vec_size.
 ***********************************************/
TSS_EXPORT int tss_getComPorts(TSS_ComPort * ports_vec, unsigned int ports_vec_size, unsigned int offset, unsigned int filter);

/********************************************//**
 * \brief Gets information from the 3-Space device connected to the com port.
 *
 * This can be used to check if there is anything connected to a com port and if its a 3-Space device.
 * It writes the TSS_ComInfo structure so you can get the device type, serial number, hardware, and firmware.
 * \param com_port The com port string for the port to be queried.
 * \param com_info Writes this structure to return the com port information gathered.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getTSDeviceInfoFromComPort(const char* com_port, TSS_ComInfo * com_info);

/********************************************//**
 * \brief Gets information from the 3-Space device associated with the device ID.
 *
 * It writes the TSS_ComInfo structure so you can check the device type, serial number, hardware, and firmware of the 3-Space device.
 * \param device The device ID of the 3-Space device to check.
 * \param com_info Writes this structure to return the com port information gathered.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getTSDeviceInfo(TSS_Device_Id device, TSS_ComInfo * com_info);

/********************************************//**
 * \brief Creates a device ID with wide char support.
 *
 * This creates and performs initial setup on the connected 3-Space device.
 * The returned device ID is used to interact with the 3-Space device.
 * When a device ID is created, other programs cannot use that com port until tss_closeTSDevice is called or the application is closed.
 * \param com_port The com port string for the com port to be queried.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * \return A device ID, TSS_NO_DEVICE_ID is returned if the creation failed.
 ***********************************************/
TSS_EXPORT TSS_Device_Id tss_createTSDeviceU16Str(const wchar_t* com_port, TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Creates a device ID.
 *
 * This creates and performs initial setup on the connected 3-Space device.
 * The returned device ID is used to interact with the 3-Space device.
 * When a device ID is created, other programs cannot use that com port until tss_closeTSDevice is called or the application is closed.
 * \param com_port The com port string for the com port to be queried.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * \return A device ID, TSS_NO_DEVICE_ID is returned if the creation failed.
 ***********************************************/
TSS_EXPORT TSS_Device_Id tss_createTSDeviceStr(const char* com_port, TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Creates a device ID with extended parameter and wide char support.
 *
 * This creates and performs initial setup on the connected 3-Space device.
 * The returned device ID is used to interact with the 3-Space device.
 * When a device ID is created, other programs cannot use that com port until tss_closeTSDevice is called or the application is closed.
 * \param com_port The com port string for the com port to be queried.
 * \param baud_rate The baud rate of the connected 3-Space device.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * \return A device ID, TSS_NO_DEVICE_ID is returned if the creation failed.
 ***********************************************/
TSS_EXPORT TSS_Device_Id tss_createTSDeviceU16StrEx(const wchar_t* com_port, int baud_rate, TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Creates a device ID with extended parameter.
 *
 * This creates and performs initial setup on the connected 3-Space device.
 * The returned device ID is used to interact with the 3-Space device.
 * When a device ID is created, other programs cannot use that com port until tss_closeTSDevice is called or the application is closed.
 * \param com_port The com port string for the com port to be queried.
 * \param baud_rate The baud rate of the connected 3-Space device.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * \return A device ID, TSS_NO_DEVICE_ID is returned if the creation failed.
 ***********************************************/
TSS_EXPORT TSS_Device_Id tss_createTSDeviceStrEx(const char* com_port, int baud_rate, TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Closes and frees the com port.
 *
 * Once the com port is freed, other programs can use the com port if necessary.
 * \param device The device ID for the 3-Space device.
 ***********************************************/
TSS_EXPORT TSS_Error tss_closeTSDevice(TSS_Device_Id device);

/********************************************//**
 * \brief Creates a device ID from a dongle.
 *
 * This creates and performs initial setup or returns an already created device ID for a wireless sensor from a dongle.
 * The returned device ID is used to interact with the 3-Space device.
 * The wireless sensor must be paired with the dongle or this will fail.
 * The timestamp mode is inherited from the dongle's settings.
 * \param device The device ID for the dongle.
 * \param logical_id The logical identifier of the desired wireless sensor, 0-14 is the valid range.
 * \param w_ts_device Writes the device ID, TSS_NO_DEVICE_ID is written if the creation failed.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getSensorFromDongle(TSS_Device_Id device, int logical_id, unsigned int * w_ts_device);

/********************************************//**
 * \brief Sets a wireless sensor to a dongle.
 *
 * This creates and performs initial setup for a wireless sensor and pairs it to a dongle.
 * The wireless sensor must have the same wireless PanID and Channel as the dongle or this will fail.
 * The timestamp mode is inherited from the dongle's settings.
 * \param device The device ID for the dongle.
 * \param logical_id The desired logical identifier for the wireless sensor, 0-14 is the valid range.
 * \param w_serial_number The serial number of the wireless sensor to be paired to the dongle.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_setSensorToDongle(TSS_Device_Id device, int logical_id, unsigned int w_serial_number);

/********************************************//**
 * \brief Sets the timestamp mode on an existing 3-Space device.
 * \param device The device ID for the 3-Space device.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_setTimestampMode(TSS_Device_Id device, TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Gets the last stream data from a sensor.
 *
 * Non-blocking method of handling stream data, function will immediately return data.
 * Good for real-time applications.
 * \param device The device ID for the sensor.
 * \param output_data The data the sensor returned.
 * \param output_data_len The size of the data returned.
 * \param timestamp The timestamp in microseconds if timestamps are enabled.
 * \return TSS_NO_ERROR(0) if successful, TSS_ERROR_READ if called before any data has returned and
 * non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getLastStreamData(TSS_Device_Id device, char * output_data, unsigned int output_data_len, unsigned int * timestamp);

/********************************************//**
 * \brief Gets the last stream data from a sensor.
 *
 * Non-blocking method of handling stream data, function will immediately return data.
 * Good for real-time applications.
 * This second method is for hooking to UDK and other applications that will not allow multiple casts from the same function.
 * \param device The device ID for the sensor.
 * \param output_data The data the sensor returned.
 * \param output_data_len The size of the data returned.
 * \param timestamp The timestamp in microseconds if timestamps are enabled.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getLastStreamData2(TSS_Device_Id device, char * output_data, unsigned int output_data_len, unsigned int * timestamp);

/********************************************//**
 * \brief Gets the latest stream data from a sensor.
 *
 * Blocking method of handling stream data, function will wait until new data arrives.
 * Good for data logging.
 * \param device The device ID for the sensor.
 * \param output_data The data the sensor returned.
 * \param output_data_len The size of the data returned.
 * \param timeout The time in milliseconds to wait until the function will timeout.
 * \param timestamp The timestamp in microseconds if timestamps are enabled.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getLatestStreamData(TSS_Device_Id device, char * output_data, unsigned int output_data_len, unsigned int timeout, unsigned int * timestamp);

/********************************************//**
 * \brief Sets a new data callback for a sensor.
 *
 * This is used to setup the callback method.
 * The callback will be called every time a new stream data form the sensor arrives.
 * \param device The device ID for the sensor.
 * \param callback_func The function to call when new data arrives.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_setNewDataCallBack(TSS_Device_Id device, TSS_CallBack callback_func);

/********************************************//**
 * \brief Sends a raw command to a 3-Space device formatted for converting to and from big endian.
 *
 * Sending raw commands allows users to call functions that are in custom or beta firmwares that are not exposed in the API.
 * in_data_detail and rtn_data_detail are formatter strings, their input is as follows:
 *  i : int
 *  I : unsigned int
 *  f : float
 *  h : short
 *  H : unsigned short
 *  b : signed char
 *  B : unsigned char
 *  ? : bool (char)
 *  s : char
 *  x : pad byte
 * For example, the command for getting the tared orientation as a quaternion would be formatted as follows:
 *  float quat[4];
 *  unsigned int timestamp;
 *  tss_sendRawCommandFormatted(device, 0x00, NULL, "", (char*)quat, "ffff", &timestamp);
 * \param device The device ID for the sensor.
 * \param command The command byte of the sensor function.
 * \param in_data Pointer to the input data for the command. Pass NULL if the command has no input.
 * \param in_data_detail The formatter string for the input data. Pass "" if the command has no input.
 * \param rtn_data Pointer to the return data for the command. Pass NULL if the command has no output.
 * \param rtn_data_detail The formatter string for the return data. Pass "" if the command has no output.
 * \param timestamp Optionally can get the timestamp of the command packet. Pass NULL if the timestamp is not wanted.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_sendRawCommandFormatted(   TSS_Device_Id device,
                                                    unsigned char command,
                                                    char* in_data,
                                                    char* in_data_detail,
                                                    char * rtn_data,
                                                    char* rtn_data_detail,
                                                    unsigned int * timestamp);

/********************************************//**
 * \brief Sends a raw command to a 3-Space device un-formatted.
 *
 * Sending raw commands allows users to call functions that are in custom or beta firmwares that are not exposed in the API.
 * This function does not convert the bytes to and from big endian, data must be formatted by the user.
 * For example, the command for getting the tared orientation as a quaternion would be formatted as follows:
 *  float quat[4];
 *  unsigned int timestamp;
 *  tss_sendRawCommand(device, 0x00, NULL, 0, (char*)quat, 16, &timestamp);
 * \param device The device ID for the sensor.
 * \param command The command byte of the sensor function.
 * \param in_data Pointer to the input data for the command. Pass NULL if the command has no input.
 * \param in_data_size The size in bytes of input data.
 * \param rtn_data Pointer to the return data for the command. Pass NULL if the command has no output.
 * \param rtn_data_size The size in bytes of return data.
 * \param timestamp Optionally can get the timestamp of the command packet. Pass NULL if the timestamp is not wanted.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_sendRawCommand(TSS_Device_Id device,
                                        unsigned char command,
                                        char* in_data,
                                        int in_data_size,
                                        char * rtn_data,
                                        int rtn_data_size,
                                        unsigned int * timestamp);

//Remove debug func
//TSS_EXPORT TSS_Error tss_getRecordCount(TSS_Device_Id device);

/********************************************//**
 * \brief Tests to check if a 3-Space device is connected.
 *
 * This sends a command to check if the device is still available.
 * \param device The device ID for the 3-Space device.
 * \param reconnect Whether to attempt a serial reconnection, 0 is false 1 is true.
 * \return 1 if connected, 0 if unreachable/not connected.
 ***********************************************/
TSS_EXPORT int tss_isConnected(TSS_Device_Id device, int reconnect);

/********************************************//**
 * \brief Gets the device type of a 3-Space device.
 * \param device The device ID for the 3-Space device.
 * \param device_type Writes the type of the 3-Space device.
 * \return TSS_NO_ERROR(0) if successful, non zero if an error occurred.
 ***********************************************/
TSS_EXPORT TSS_Error tss_getTSDeviceType(TSS_Device_Id device, TSS_Type * device_type);

/********************************************//**
 * \brief Reset the ThreeSpace API
 *
 * This will stop streaming and close all created device ids.
 * All device ids created before the reset are invalid and should be discarded.
 * \return 0 if no error else nonzero for error
 ***********************************************/
TSS_EXPORT int tss_resetThreeSpaceAPI();

/********************************************//**
 * \brief Create ts devices by serial number
 *
 * This convenience function will take a list of serial numbers and will write a list of device ids
 * \param serial_list The list of serial devices to find
 * \param device_list The list of devices ids that will be written to, this list must be the same size as the serial_list
 * \param list_length The length of serial_list, which should be the same length as the device_list
 * \param search_wireless If set to non-zero(true) this will create any dongles it finds automatically and will search its
 *  logical table to see if it can wirelessly communicate with any matching serial numbered devices
 * \param search_unknown_devices If set to non-zero(true) this will search all available comports. Com ports that cannot
 *  be passively identified as ThreeSpace devices will have bytes written to them to get more information.
 *  This is for physical comports and 3rd party serial to USB adapters.
 * \param mode The desired timestamp mode the 3-Space device is to be configured with.
 * if TSS_NO_DEVICE_ID is returned then sensor not found
 * \return number of found devices, -1 if error
 ***********************************************/
TSS_EXPORT int tss_createTSDevicesBySerialNumber(   unsigned int * serial_list,
                                                    TSS_Device_Id * device_list,
                                                    int list_length,
                                                    int search_wireless,
                                                    int search_unknown_devices,
                                                    TSS_Timestamp_Mode mode);

/********************************************//**
 * \brief Get device IDs
 *
 * This convenience function will return a list of already created device ids
 * \param device_list The list of devices ids that will be written to
 * \param list_length The length of device_list
 * \param offset The offset applied to where in the filtered data starts writing to the device list.
 * This is useful to write a loop to get all the devices
 * \param filter Pass in a combination of TSS_Find enums you want to search for.
 * \return number of found devices, -1 if error
 ***********************************************/
TSS_EXPORT int tss_getDeviceIDs( TSS_Device_Id * device_list, int list_length, int offset, int filter);

//Orientation Commands
//   0(0x00)
 /*****************AutoGenerated*************//**
 * \brief Get tared orientation as quaternion
 *
 * Returns the filtered, tared orientation estimate in quaternion form
 *
 * Input:
 * None
 *
 * Output:
 * Quaternion (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredOrientationAsQuaternion(TSS_Device_Id device, float * quat4, unsigned int * timestamp);
//   1(0x01)
 /*****************AutoGenerated*************//**
 * \brief Get tared orientation as euler angles
 *
 * Returns the filtered, tared orientation estimate in euler angle form
 *
 * Input:
 * None
 *
 * Output:
 * Euler Angles (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredOrientationAsEulerAngles(TSS_Device_Id device, float * euler3, unsigned int * timestamp);
//   2(0x02)
 /*****************AutoGenerated*************//**
 * \brief Get tared orientation as rotation matrix
 *
 * Returns the filtered, tared orientation estimate in rotation matrix form
 *
 * Input:
 * None
 *
 * Output:
 * Rotation Matrix (float x9)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredOrientationAsRotationMatrix(TSS_Device_Id device, float * matrix9, unsigned int * timestamp);
//   3(0x03)
 /*****************AutoGenerated*************//**
 * \brief Get tared orientation as axis angle
 *
 * Returns the filtered, tared orientation estimate in axis-angle form
 *
 * Input:
 * None
 *
 * Output:
 * Axis (float x3), Angle in Radians (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredOrientationAsAxisAngle(TSS_Device_Id device, float * axis_angle4, unsigned int * timestamp);
//   4(0x04)
 /*****************AutoGenerated*************//**
 * \brief Get tared orientation as two vector.
 *
 * Returns the filtered, tared orientation estimate in two vector form, where the first vector refers to forward and the second refers to down.
 *
 * Input:
 * None
 *
 * Output:
 * Forward Vector (float x3), Down Vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredOrientationAsTwoVector(TSS_Device_Id device, float * forward3, float * down3, unsigned int * timestamp);
//   5(0x05)
 /*****************AutoGenerated*************//**
 * \brief Get difference quaternion
 *
 * Returns the difference between the measured orientation from last frame and this frame.
 *
 * Input:
 * None
 *
 * Output:
 * Quaternion (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getDifferenceQuaternion(TSS_Device_Id device, float * quat4, unsigned int * timestamp);
//   6(0x06)
 /*****************AutoGenerated*************//**
 * \brief Get untared orientation as quaternion
 *
 * Returns the filtered, untared orientation estimate in quaternion form.
 *
 * Input:
 * None
 *
 * Output:
 * Quaternion (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredOrientationAsQuaternion(TSS_Device_Id device, float * quat4, unsigned int * timestamp);
//   7(0x07)
 /*****************AutoGenerated*************//**
 * \brief Get untared orientation as euler angles
 *
 * Returns the filtered, untared orientation estimate in euler angle form
 *
 * Input:
 * None
 *
 * Output:
 * Euler Angles (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredOrientationAsEulerAngles(TSS_Device_Id device, float * euler3, unsigned int * timestamp);
//   8(0x08)
 /*****************AutoGenerated*************//**
 * \brief Get untared orientation as rotation matrix
 *
 * Returns the filtered, untared orientation estimate in rotation matrix form
 *
 * Input:
 * None
 *
 * Output:
 * Rotation Matrix (float x9)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredOrientationAsRotationMatrix(TSS_Device_Id device, float * matrix9, unsigned int * timestamp);
//   9(0x09)
 /*****************AutoGenerated*************//**
 * \brief Get untared orientation as axis angle
 *
 * Returns the filtered, untared orientation estimate in axis-angle form
 *
 * Input:
 * None
 *
 * Output:
 * Axis (float x3), Angle in Radians (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredOrientationAsAxisAngle(TSS_Device_Id device, float * axis_angle4, unsigned int * timestamp);
//  10(0x0a)
 /*****************AutoGenerated*************//**
 * \brief Get untared orientation as two vector
 *
 * Returns the filtered, untared orientation estimate in two vector form, where the first vector refers to north and the second refers to gravity.
 *
 * Input:
 * None
 *
 * Output:
 * North Vector (float x3), Gravity Vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredOrientationAsTwoVector(TSS_Device_Id device, float * north3, float * gravity3, unsigned int * timestamp);
//  11(0x0b)
 /*****************AutoGenerated*************//**
 * \brief Get tared two vector in sensor frame
 *
 * Returns the filtered, tared orientation estimate in two vector form, where the first vector refers to forward and the second refers to down. These vectors are given in the sensor reference frame and not the global reference frame.
 *
 * Input:
 * None
 *
 * Output:
 * Forward Vector (float x3), Down Vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTaredTwoVectorInSensorFrame(TSS_Device_Id device, float * forward3, float * down3, unsigned int * timestamp);
//  12(0x0c)
 /*****************AutoGenerated*************//**
 * \brief Get untared two vector in sensor frame
 *
 * Returns the filtered, untared orientation estimate in two vector form, where the first vector refers to north and the second refers to gravity. These vectors are given in the sensor reference frame and not the global reference frame.
 *
 * Input:
 * None
 *
 * Output:
 * North Vector (float x3), Gravity Vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUntaredTwoVectorInSensorFrame(TSS_Device_Id device, float * north3, float * gravity3, unsigned int * timestamp);

//Embedded Commands
//  29(0x1d)
 /*****************AutoGenerated*************//**
 * \brief Set pin mode
 *
 * Sets the pin mode of the sensor. First parameter is mode, which will be 0 for off, 1 for pulse mode, 2 for level, 3 for SPI pulse and 4 for button. Second parameter is pin, which will be 0 for TXD(for button, also RXD) or 1 for MISO(for button, also MOSI).
 *
 * Input:
 * Mode (Byte), Pin (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setInterruptType(TSS_Device_Id device, unsigned char mode, unsigned char pin, unsigned int * timestamp);
//  30(0x1e)
 /*****************AutoGenerated*************//**
 * \brief Get pin mode
 *
 * Read the interrupt mode of the sensor. First parameter is mode, which will be 0 for off, 1 for pulse mode, 2 for level, 3 for SPI pulse and 4 for button. Second parameter is pin, which will be 0 for TXD(for button, also RXD) or 1 for MISO(for button, also MOSI).
 *
 * Input:
 * None
 *
 * Output:
 * Mode (Byte), Pin (Byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getInterruptType(TSS_Device_Id device, unsigned char * mode, unsigned char * pin, unsigned int * timestamp);
//  31(0x1f)
 /*****************AutoGenerated*************//**
 * \brief Get interrupt status
 *
 * Read the current interrupt status. This value will be 1 if the filter has updated since the last time the value was read or 0 otherwise.
 *
 * Input:
 * None
 *
 * Output:
 * Status (Byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getInterruptStatus(TSS_Device_Id device, unsigned char * status, unsigned int * timestamp);

//Normalized Data Commands
//  32(0x20)
 /*****************AutoGenerated*************//**
 * \brief Get all normalized component sensor data
 *
 * Returns the normalized gyro rate vector, accelerometer vector, and compass vector. Note that the gyro vector is in units of radians/sec, while the accelerometer and compass are unit-length vectors indicating the direction of gravity and north, respectively. These two vectors do not have any magnitude data associated with them.
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in units of radians/sec (Vector x3), Gravity Direction (Vector x3), North Direction (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAllNormalizedComponentSensorData(TSS_Device_Id device, float * gyro_rate3, float * gravity_direction3, float * north_direction3, unsigned int * timestamp);
//  33(0x21)
 /*****************AutoGenerated*************//**
 * \brief Get normalized gyro rate
 *
 * Returns the normalized gyro rate vector, which is in units of radians/sec.
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in units of radians/sec (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getNormalizedGyroRate(TSS_Device_Id device, float * gyro_rate3, unsigned int * timestamp);
//  34(0x22)
 /*****************AutoGenerated*************//**
 * \brief Get normalized accelerometer vector
 *
 * Returns the normalized accelerometer vector. Note that this is a unit-vector indicating the direction of gravity. This vector does not have any magnitude data associated with it.
 *
 * Input:
 * None
 *
 * Output:
 * Gravity Direction (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getNormalizedAccelerometerVector(TSS_Device_Id device, float * gravity_direction3, unsigned int * timestamp);
//  35(0x23)
 /*****************AutoGenerated*************//**
 * \brief Get normalized compass vector
 *
 * Returns the normalized compass vector. Note that this is a unit-vector indicating the direction of gravity. This vector does not have any magnitude data associated with it.
 *
 * Input:
 * None
 *
 * Output:
 * North Direction (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getNormalizedCompassVector(TSS_Device_Id device, float * north_direction3, unsigned int * timestamp);

//Corrected Data Commands
//  37(0x25)
 /*****************AutoGenerated*************//**
 * \brief Get all corrected component sensor data
 *
 * Returns the corrected gyro rate vector, accelerometer vector, and compass vector. Note that the gyro vector is in units of radians/sec, the accelerometer vector is in units of G, and the compass vector is in units of gauss. 
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in units of radians/sec (Vector x3), Acceleration Vector in units of G (Vector x3), Compass Vector in units of gauss (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAllCorrectedComponentSensorData(TSS_Device_Id device, float * gyro_rate3, float * accelerometer3, float * compass3, unsigned int * timestamp);
//  38(0x26)
 /*****************AutoGenerated*************//**
 * \brief Get corrected gyro rate
 *
 * Returns the corrected gyro rate vector, which is in units of radians/sec. Note that this result is the same data returned by the normalized gyro rate command.
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in units of radians/sec (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCorrectedGyroRate(TSS_Device_Id device, float * gyro_rate3, unsigned int * timestamp);
//  39(0x27)
 /*****************AutoGenerated*************//**
 * \brief Get corrected accelerometer vector
 *
 * Returns the acceleration vector in units of G. Note that this acceleration will include the static component of acceleration due to gravity.
 *
 * Input:
 * None
 *
 * Output:
 * Acceleration Vector in units of G (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCorrectedAccelerometerVector(TSS_Device_Id device, float * accelerometer3, unsigned int * timestamp);
//  40(0x28)
 /*****************AutoGenerated*************//**
 * \brief Get corrected compass vector
 *
 * Returns the compass vector in units of gauss.
 *
 * Input:
 * None
 *
 * Output:
 * Compass Vector in units of gauss (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCorrectedCompassVector(TSS_Device_Id device, float * compass3, unsigned int * timestamp);
//  41(0x29)
 /*****************AutoGenerated*************//**
 * \brief Get corrected linear acceleration in global space
 *
 * Returns the linear acceleration of the device, which is the overall acceleration which has been orientation compensated and had the component of acceleration due to gravity removed. Uses the untared orientation.
 *
 * Input:
 * None
 *
 * Output:
 * Acceleration Vector in units of G (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCorrectedLinearAccelerationInGlobalSpace(TSS_Device_Id device, float * accelerometer3, unsigned int * timestamp);

//Other Data Commands
//  43(0x2b)
 /*****************AutoGenerated*************//**
 * \brief Get temperature C
 *
 * Returns the temperature of the sensor in Celsius.
 *
 * Input:
 * None
 *
 * Output:
 * Temperature (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTemperatureC(TSS_Device_Id device, float * temperature, unsigned int * timestamp);
//  44(0x2c)
 /*****************AutoGenerated*************//**
 * \brief Get temperature F
 *
 * Returns the temperature of the sensor in Fahrenheit
 *
 * Input:
 * None
 *
 * Output:
 * Temperature (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTemperatureF(TSS_Device_Id device, float * temperature, unsigned int * timestamp);
//  45(0x2d)
 /*****************AutoGenerated*************//**
 * \brief Get confidence factor
 *
 * Returns a value indicating how much the sensor is being moved at the moment. This value will return 1 if the sensor is completely stationary, and will return 0 if it is in motion. This command can also return values in between indicating how much motion the sensor is experiencing. 
 *
 * Input:
 * None
 *
 * Output:
 * Confidence Factor (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getConfidenceFactor(TSS_Device_Id device, float * confidence, unsigned int * timestamp);

//Data-Logging Commands
//  57(0x39)
 /*****************AutoGenerated*************//**
 * \brief Turn on mass storage
 *
 * Enables SD card access, but prevents data logs from being taken.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_turnOnMassStorage(TSS_Device_Id device, unsigned int * timestamp);
//  58(0x3a)
 /*****************AutoGenerated*************//**
 * \brief Turn off mass storage
 *
 * Disables SD card access, but allows data logs to be taken.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_turnOffMassStorage(TSS_Device_Id device, unsigned int * timestamp);
//  59(0x3b)
 /*****************AutoGenerated*************//**
 * \brief Format and Initialize SD Card
 *
 * Erases the contents of the SD card.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_formatAndInitializeSDCard(TSS_Device_Id device, unsigned int * timestamp);
//  60(0x3c)
 /*****************AutoGenerated*************//**
 * \brief Begin data logging session
 *
 * Initiates a data logging section with the specified attributes as indicated in the provided data logging configuration file.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_beginDataLoggingSession(TSS_Device_Id device, unsigned int * timestamp);
//  61(0x3d)
 /*****************AutoGenerated*************//**
 * \brief End data logging session
 *
 * Terminates the ongoing data logging session
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_endDataLoggingSession(TSS_Device_Id device, unsigned int * timestamp);
//  62(0x3e)
 /*****************AutoGenerated*************//**
 * \brief Set clock values
 *
 * Sets the current time on the onboard real-time clock.
 *
 * Input:
 * Month (Byte), Day (Byte), Year (Byte), Hour (Byte), Minute (Byte), Second (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setClockValues(TSS_Device_Id device,   unsigned char month,
                                                                unsigned char day,
                                                                unsigned char year,
                                                                unsigned char hour,
                                                                unsigned char minute,
                                                                unsigned char second,
                                                                unsigned int * timestamp);
//  63(0x3f)
 /*****************AutoGenerated*************//**
 * \brief Get clock values
 *
 * Returns the current time as read by the onboard real-time clock.
 *
 * Input:
 * None
 *
 * Output:
 * Month (Byte), Day (Byte), Year (Byte), Hour (Byte), Minute (Byte), Second (Byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getClockValues(TSS_Device_Id device,    unsigned char * month,
                                                                unsigned char * day,
                                                                unsigned char * year,
                                                                unsigned char * hour,
                                                                unsigned char * minute,
                                                                unsigned char * second,
                                                                unsigned int * timestamp);

//Raw Data Commands
//  64(0x40)
 /*****************AutoGenerated*************//**
 * \brief Get all raw component sensor data
 *
 * Returns the raw gyro rate vector, accelerometer vector and compass vector as read directly from the component sensors without any additional post-processing. The range of values is dependent on the currently selected range for each respective sensor.
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in counts per degrees/sec (Vector x3), Acceleration Vector in counts per g (Vector x3), Compass Vector in counts per gauss (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAllRawComponentSensorData(TSS_Device_Id device, float * gyro_rate3, float * accelerometer3, float * compass3, unsigned int * timestamp);
//  65(0x41)
 /*****************AutoGenerated*************//**
 * \brief Get raw gyroscope rate
 *
 * Returns the raw gyro rate vector as read directly from the gyroscope without any additional post-processing.
 *
 * Input:
 * None
 *
 * Output:
 * Gyro Rate in counts per degrees/sec (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getRawGyroscopeRate(TSS_Device_Id device, float * gyro_rate3, unsigned int * timestamp);
//  66(0x42)
 /*****************AutoGenerated*************//**
 * \brief Get raw accelerometer data
 *
 * Returns the raw acceleration vector as read directly from the accelerometer without any additional post-processing.
 *
 * Input:
 * None
 *
 * Output:
 * Acceleration Vector in counts per g (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getRawAccelerometerData(TSS_Device_Id device, float * accelerometer3, unsigned int * timestamp);
//  67(0x43)
 /*****************AutoGenerated*************//**
 * \brief Get raw compass data
 *
 * Returns the raw compass vector as read directly from the compass without any additional post-processing.
 *
 * Input:
 * None
 *
 * Output:
 * Compass Vector in counts per gauss (Vector x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getRawCompassData(TSS_Device_Id device, float * compass3, unsigned int * timestamp);

//Streaming Commands
//  80(0x50)
 /*****************AutoGenerated*************//**
 * \brief Set streaming slots
 *
 * Configures data output slots for streaming mode. Command accepts a list of eight bytes, where each byte corresponds to a different data command. Every streaming iteration, each command will be executed in order and the resulting data will be output in the specified slot. Valid commands are commands in the ranges 0x0  0x10, 0x20  0x30, 0x40  0x50, 0xC9  0xCA (for battery-powered sensors) and 0xFA. A slot value of 0xFF 'clears' the slot and prevents any data from being written in that position. This command can fail if there is an invalid command passed in as any of the parameters or if the total allotted size is exceeded. Upon failure, all slots will be reset to 0xFF. This setting can be saved to non-volatile flash memory using the Commit Settings command. 
 *
 * Input:
 * Commands (Byte x8)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setStreamingSlots(TSS_Device_Id device, const TSS_Stream_Command * slots8, unsigned int * timestamp);
//  81(0x51)
 /*****************AutoGenerated*************//**
 * \brief Get streaming slots
 *
 * Returns the current streaming slots configuration.
 *
 * Input:
 * None
 *
 * Output:
 * Commands (Byte x8)
 ********************************************/
TSS_EXPORT TSS_Error tss_getStreamingSlots(TSS_Device_Id device, TSS_Stream_Command * slots8, unsigned int * timestamp);
//  82(0x52)
 /*****************AutoGenerated*************//**
 * \brief Set streaming timing
 *
 * Configures timing information for a streaming session. All parameters are specified in microseconds. The first parameter is the interval, which specifies how often data will be output. A value of 0 means that data will be output at the end of every filter loop. Aside from 0, values lower than 1000 will be clamped to 1000. The second parameter is the duration, which specifies the length of the streaming session. If this value is set to 0xFFFFFFFF, streaming will continue indefinitely until it is stopped via command 0x56. The third parameter is the delay, which specifies a n amount of time the sensor will wait before outputting the first packet of streaming data. This setting can be saved to non-volatile flash memory using the Commit Settings command. 
 *
 * Input:
 * Interval (Unsigned int), Duration (Unsigned int), Delay (Unsigned int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setStreamingTiming(TSS_Device_Id device, unsigned int interval, unsigned int duration, unsigned int delay, unsigned int * timestamp);
//  83(0x53)
 /*****************AutoGenerated*************//**
 * \brief Get streaming timing
 *
 * Returns the current streaming timing information. 
 *
 * Input:
 * None
 *
 * Output:
 * Interval (Unsigned int), Duration (Unsigned int), Delay (Unsigned int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getStreamingTiming(TSS_Device_Id device, unsigned int * interval, unsigned int * duration, unsigned int * delay, unsigned int * timestamp);
//  84(0x54)
 /*****************AutoGenerated*************//**
 * \brief Get streaming batch
 *
 * Return a single packet of streaming data using the current slot configuration.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_getStreamingBatch(TSS_Device_Id device, char * output_data, unsigned int output_data_len, unsigned int * timestamp);
//  85(0x55)
 /*****************AutoGenerated*************//**
 * \brief Start streaming 
 *
 * Start a streaming session using the current slot and timing configuration.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_startStreaming(TSS_Device_Id device, unsigned int * timestamp);
//  86(0x56)
 /*****************AutoGenerated*************//**
 * \brief Stop streaming
 *
 * Stop the current streaming session. 
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_stopStreaming(TSS_Device_Id device, unsigned int * timestamp);
//  95(0x5f)
 /*****************AutoGenerated*************//**
 * \brief Update current timestamp
 *
 * Set the current internal timestamp to the specified value.
 *
 * Input:
 * Timestamp (Unsigned int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_updateCurrentTimestamp(TSS_Device_Id device, unsigned int set_timestamp, unsigned int * timestamp);

//Configuration Write Commands
//  16(0x10)
 /*****************AutoGenerated*************//**
 * \brief Set euler angle decomposition order
 *
 * Sets the current euler angle decomposition order, which determines how the angles returned from command 0x1 are decomposed from the full quaternion orientation. Possible values are 0x0 for XYZ, 0x1 for YZX, 0x2 for ZXY, 0x3 for ZYX, 0x4 for XZY or 0x5 for YXZ (default).
 *
 * Input:
 * Euler angle decomposition order(byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setEulerAngleDecompositionOrder(TSS_Device_Id device, unsigned char order, unsigned int * timestamp);
//  17(0x11)
 /*****************AutoGenerated*************//**
 * \brief Set magnetoresistive threshold
 *
 * Sets required parameters that are necessary to trigger magnetometer resistance mode. First parameter to the command specifies the change in magnetometer field strength that is required to trigger the resistance. Once this field has been detected, the magnetometer will enter a period where it is completely locked out of the orientation calculationthis period will increase while magnetic perturbations are still being detected, but will dissipate as the sensor remains stationary. Once this period is over, the sensor orientation will slowly begin trusting the magnetometer again. The second parameter represents the number of frames that must elapse before the magnetometer is fully trusted again. The third parameter represents a decay value between 0 and 1 that indicates how quickly the outright magnetometer rejection state will fall off. Values closer to 1 result in the magnetometer rejection lasting longer. The final parameter represents how quickly a magnetic perturbation is detected. Values closer to 1 result in the magnetometer rejection occurring more slowly. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Magnetoresistive threshold in gauss(float), Number of magnetometer trust frames (unsigned int), magnetometer lockout decay value (float), magnetometer perturbation detection value (float)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setMagnetoresistiveThreshold(TSS_Device_Id device, float threshold, unsigned int trust_frames, float lockout_decay, float perturbation_detection_value, unsigned int * timestamp);
//  18(0x12)
 /*****************AutoGenerated*************//**
 * \brief Set accelerometer resistance threshold
 *
 * Sets required parameters that are necessary to trigger accelerometer rejection. During the accelerometer rejection period, the contribution of the accelerometer to the selected orientation estimation algorithm will be zero. The arguments to this command specify the accelerometer threshold and the number of frames that the rejection is active, respectively. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Accelerometer threshold in g's (float), Number of accelerometer lockout frames (unsigned int), 
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAccelerometerResistanceThreshold(TSS_Device_Id device, float threshold, unsigned int lockout_frames, unsigned int * timestamp);
//  19(0x13)
 /*****************AutoGenerated*************//**
 * \brief Offset with current orientation
 *
 * Sets the offset orientation to be the same as the current filtered orientation.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_offsetWithCurrentOrientation(TSS_Device_Id device, unsigned int * timestamp);
//  20(0x14)
 /*****************AutoGenerated*************//**
 * \brief Reset base offset
 *
 * Sets the base offset to an identity quaternion.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_resetBaseOffset(TSS_Device_Id device, unsigned int * timestamp);
//  21(0x15)
 /*****************AutoGenerated*************//**
 * \brief Offset with quaternion
 *
 * Sets the offset orientation to be the same as the supplied orientation, which should be passed as a quaternion.
 *
 * Input:
 * Quaternion (float x4)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_offsetWithQuaternion(TSS_Device_Id device, const float * quat4, unsigned int * timestamp);
//  22(0x16)
 /*****************AutoGenerated*************//**
 * \brief Set base offset with current orientation
 *
 * Sets the base offset orientation to be the same as the current filtered orientation.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setBaseOffsetWithCurrentOrientation(TSS_Device_Id device, unsigned int * timestamp);
//  96(0x60)
 /*****************AutoGenerated*************//**
 * \brief Tare with current orientation
 *
 * Sets the tare orientation to be the same as the current filtered orientation.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_tareWithCurrentOrientation(TSS_Device_Id device, unsigned int * timestamp);
//  97(0x61)
 /*****************AutoGenerated*************//**
 * \brief Tare with quaternion
 *
 * Sets the tare orientation to be the same as the supplied orientation, which should be passed as a quaternion.
 *
 * Input:
 * Quaternion (float x4)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_tareWithQuaternion(TSS_Device_Id device, const float * quat4, unsigned int * timestamp);
//  98(0x62)
 /*****************AutoGenerated*************//**
 * \brief Tare with rotation matrix
 *
 * Sets the tare orientation to be the same as the supplied orientation, which should be passed as a rotation matrix.
 *
 * Input:
 * Rotation Matrix (float x9)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_tareWithRotationMatrix(TSS_Device_Id device, const float * matrix9, unsigned int * timestamp);
//  99(0x63)
 /*****************AutoGenerated*************//**
 * \brief Set static accelerometer trust value
 *
 * Determines how trusted the accelerometer contribution is to the overall orientation estimation. Trust is 0 to 1, with 1 being fully trusted and 0 being not trusted at all.
 *
 * Input:
 * Accelerometer trust value (float)
 *
 * Output:
 * None
 ********************************************/
//TSS_EXPORT TSS_Error tss_setStaticAccelerometerRhoMode(TSS_Device_Id device, float rho_value, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_setStaticAccelerometerTrustValue(TSS_Device_Id device, float trust_value, unsigned int * timestamp);
// 100(0x64)
 /*****************AutoGenerated*************//**
 * \brief Set confidence accelerometer trust values
 *
 * Determines how trusted the accelerometer contribution is to the overall orientation estimation. Instead of using a single value, uses a minimum and maximum value. Trust values will be selected from this range depending on the confidence factor. This can have the effect of smoothing out the accelerometer when the sensor is in motion. 
 *
 * Input:
 * Minimum accelerometer trust value (float), Maximum accelerometer trust value (float)
 *
 * Output:
 * None
 ********************************************/
//TSS_EXPORT TSS_Error tss_setConfidenceAccelerometerRhoMode(TSS_Device_Id device, float min_rho_value, float max_rho_value, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_setConfidenceAccelerometerTrustValues(TSS_Device_Id device, float min_trust_value, float max_trust_value, unsigned int * timestamp);
// 101(0x65)
 /*****************AutoGenerated*************//**
 * \brief Set static compass trust value
 *
 * Determines how trusted the accelerometer contribution is to the overall orientation estimation. tribution is to the overall orientation estimation. Trust is 0 to 1, with 1 being fully trusted and 0 being not trusted at all.
 *
 * Input:
 * Compass trust value (float)
 *
 * Output:
 * None
 ********************************************/
//TSS_EXPORT TSS_Error tss_setStaticCompassRhoMode(TSS_Device_Id device, float rho_value, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_setStaticCompassTrustValue(TSS_Device_Id device, float trust_value, unsigned int * timestamp);
// 102(0x66)
 /*****************AutoGenerated*************//**
 * \brief Set confidence compass trust values
 *
 * Determines how trusted the compass contribution is to the overall orientation estimation. Instead of using a single value, uses a minimum and maximum value. Trust values will be selected from this range depending on the confidence factor. This can have the effect of smoothing out the compass when the sensor is in motion. 
 *
 * Input:
 * Minimum compass trust value (float), Maximum compass trust value (float)
 *
 * Output:
 * None
 ********************************************/
//TSS_EXPORT TSS_Error tss_setConfidenceCompassRhoMode(TSS_Device_Id device, float min_rho_value, float max_rho_value, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_setConfidenceCompassTrustValues(TSS_Device_Id device, float min_trust_value, float max_trust_value, unsigned int * timestamp);
// 103(0x67)
 /*****************AutoGenerated*************//**
 * \brief Set desired update rate
 *
 * Causes the processor to wait for the specified number of microseconds at the end of each update loop. Can be useful for bounding the overall update rate of the sensor if necessary.
 *
 * Input:
 * Microsecond update rate (unsigned integer)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setDesiredUpdateRate(TSS_Device_Id device, unsigned int update_rate, unsigned int * timestamp);
// 105(0x69)
 /*****************AutoGenerated*************//**
 * \brief Set reference vector mode
 *
 * Set the current reference vector mode. Parameter can be 0 for single static mode, which uses a certain reference vector for the compass and another certain vector for the accelerometer at all times, 1 for single auto mode, which uses (0, -1, 0) as the reference vector for the accelerometer at all times and uses the average angle between the accelerometer and compass to calculate the compass reference vector once upon initiation of this mode, 2 for single auto continuous mode, which works similarly to single auto mode, but calculates this continuously, or 3 for multi-reference mode, which uses a collection of reference vectors for the compass and accelerometer both, and selects which ones to use before each step of the filter.
 *
 * Input:
 * Mode (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setReferenceVectorMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 106(0x6a)
 /*****************AutoGenerated*************//**
 * \brief Set oversample rate
 *
 * Sets the number of times to sample each component sensor for each iteration of the filter. This can smooth out readings at the cost of performance. If this value is set to 0 or 1, no oversampling occursotherwise, the number of samples per iteration depends on the specified parameter, up to a maximum of 10. This setting can be saved to non-volatile flash memory using the Commit Settings command. 
 *
 * Input:
 * Samples Per Iteration (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setOversampleRate(TSS_Device_Id device, unsigned char samples_per_iteration, unsigned int * timestamp);
// 107(0x6b)
 /*****************AutoGenerated*************//**
 * \brief Set gyroscope enabled
 *
 * Enable or disable gyroscope readings as inputs to the orientation estimation. Note that updated gyroscope readings are still accessible via commands. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Enabled (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setGyroscopeEnabled(TSS_Device_Id device, unsigned char enabled, unsigned int * timestamp);
// 108(0x6c)
 /*****************AutoGenerated*************//**
 * \brief Set accelerometer enabled
 *
 * Enable or disable accelerometer readings as inputs to the orientation estimation. Note that updated accelerometer readings are still accessible via commands. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Enabled (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAccelerometerEnabled(TSS_Device_Id device, unsigned char enabled, unsigned int * timestamp);
// 109(0x6d)
 /*****************AutoGenerated*************//**
 * \brief Set compass enabled
 *
 * Enable or disable compass readings as inputs to the orientation estimation. Note that updated compass readings are still accessible via commands. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Enabled (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setCompassEnabled(TSS_Device_Id device, unsigned char enabled, unsigned int * timestamp);
// 116(0x74)
 /*****************AutoGenerated*************//**
 * \brief Set axis directions
 *
 * Sets alternate directions for each of the natural axes of the sensor. The only parameter is a bitfield representing the possible combinations of axis swapping. The lower 3 bits specify where each of the natural axes appears: 000: X: Right, Y: Up, Z: Forward (left-handed system, standard operation) 001: X: Right, Y: Forward, Z: Up (right-handed system) 002: X: Up, Y: Right, Z: Forward (right-handed system) 003: X: Forward, Y: Right, Z: Up (left-handed system) 004: X: Up, Y: Forward, Z: Right (left-handed system) 005: X: Forward, Y: Up, Z: Right (right-handed system) (For example, using X: Right, Y: Forward, Z: Up means that any values that appear on the positive vertical(Up) axis of the sensor will be the third(Z) component of any vectors and will have a positive sign, and any that appear on the negative vertical axis will be the Z component and will have a negative sign.) The 3 bits above those are used to indicate which axes, if any, should be reversed. If it is cleared, the axis will be pointing in the positive direction. Otherwise, the axis will be pointed in the negative direction. (Note: These are applied to the axes after the previous conversion takes place). Bit 4: Positive/Negative Z (Third resulting component) Bit 5: Positive/Negative Y (Second resulting component) Bit 6: Positive/Negative X (First resulting component) Note that for each negation that is applied, the handedness of the system flips. So, if X and Z are negative and you are using a left-handed system, the system will still be left handed, but if only X is negated, the system will become right-handed.
 *
 * Input:
 * Axis Direction Byte (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAxisDirections(TSS_Device_Id device, unsigned char axis_direction_byte, unsigned int * timestamp);
// 117(0x75)
 /*****************AutoGenerated*************//**
 * \brief Set running average percent
 *
 * Sets what percentage of running average to use on the sensor's orientation. This is computed as follows:total_orient = total_orient * percenttotal_orient = total_orient + current_orient * (1  percent)current_orient = total_orientIf the percentage is 0, the running average will be shut off completely. Maximum value is 97%. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Running Average Percent (float)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setRunningAveragePercent(TSS_Device_Id device, float running_average_percent, unsigned int * timestamp);
// 118(0x76)
 /*****************AutoGenerated*************//**
 * \brief Set compass reference vector
 *
 * Sets the static compass reference vector for Single Reference Mode.
 *
 * Input:
 * Compass Reference Vector (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setCompassReferenceVector(TSS_Device_Id device, const float * reference_vector3, unsigned int * timestamp);
// 119(0x77)
 /*****************AutoGenerated*************//**
 * \brief Set accelerometer reference vector
 *
 * Sets the static accelerometer reference vector for Single Reference Mode.
 *
 * Input:
 * Accelerometer Reference Vector (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAccelerometerReferenceVector(TSS_Device_Id device, const float * reference_vector3, unsigned int * timestamp);
// 120(0x78)
 /*****************AutoGenerated*************//**
 * \brief Reset filter
 *
 * Resets the state of the currently selected filter
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_resetKalmanFilter(TSS_Device_Id device, unsigned int * timestamp);
// 121(0x79)
 /*****************AutoGenerated*************//**
 * \brief Set accelerometer range
 *
 * Only parameter is the new accelerometer range, which can be 0 for 2g (Default range), which can be 1 for 4g, or 2 for 8g. Higher ranges can detect and report larger accelerations, but are not as accurate for smaller accelerations. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Accelerometer range setting (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAccelerometerRange(TSS_Device_Id device, unsigned char accelerometer_range_setting, unsigned int * timestamp);
// 123(0x7b)
 /*****************AutoGenerated*************//**
 * \brief Set filter mode
 *
 * Used to disable the orientation filter or set the orientation filter mode. Changing this parameter can be useful for tuning filter-performance versus orientation-update rates. Passing in a parameter of 0 places the sensor into IMU mode, a 1 places the sensor into Kalman Filtered Mode (Default mode), a 2 places the sensor into Alternating Kalman Filter Mode, a 3 places the sensor into Complementary Filter Mode, a 4 places the sensor into Quaternion Gradient Descent Filter Mode, and a 5 places the sensor into Magnetoresistive Quaternion Gradient Descent Filter Mode. More information can be found in Section 3.1.5. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Mode (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setFilterMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 124(0x7c)
 /*****************AutoGenerated*************//**
 * \brief Set running average mode
 *
 * Used to further smooth out the orientation at the cost of higher latency. Passing in a parameter of 0 places the sensor into a static running average mode, a 1 places the sensor into a confidence-based running average mode, which changes the running average factor based upon the confidence factor, which is a measure of how 'in motion' the sensor is. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Mode (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setRunningAverageMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 125(0x7d)
 /*****************AutoGenerated*************//**
 * \brief Set gyroscope range
 *
 * Only parameter is the new gyroscope range, which can be 0 for 250 DPS, 1 for 500 DPS, or 2 for 2000 DPS (Default range). Higher ranges can detect and report larger angular rates, but are not as accurate for smaller angular rates. This setting can be saved to non-volatile flash memory using the Commit Settings command. 
 *
 * Input:
 * Gyroscope range setting (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setGyroscopeRange(TSS_Device_Id device, unsigned char gyroscope_range_setting, unsigned int * timestamp);
// 126(0x7e)
 /*****************AutoGenerated*************//**
 * \brief Set compass range
 *
 * Only parameter is the new compass range, which can be 0 for 0.88G, 1 for 1.3G (Default range), 2 for 1.9G, 3 for 2.5G, 4 for 4.0G, 5 for 4.7G, 6 for 5.6G, or 7 for 8.1G. Higher ranges can detect and report larger magnetic field strengths but are not as accurate for smaller magnetic field strengths. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Compass range setting (Byte) 
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setCompassRange(TSS_Device_Id device, unsigned char compass_range_setting, unsigned int * timestamp);

// Configuration Read Commands
// 128(0x80)  Get tare orientation as quaternion
 /*****************AutoGenerated*************//**
 * \brief Get tare orientation as quaternion
 *
 * Returns the current tare orientation as a quaternion.
 *
 * Input:
 * None
 *
 * Output:
 * Quaternion (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTareAsQuaternion(TSS_Device_Id device, float * quat4, unsigned int * timestamp);
// 129(0x81)  Get tare orientation as rotation matrix
 /*****************AutoGenerated*************//**
 * \brief Get tare orientation as rotation matrix
 *
 * Returns the current tare orientation as a rotation matrix.
 *
 * Input:
 * None
 *
 * Output:
 * Rotation Matrix (float x9)
 ********************************************/
TSS_EXPORT TSS_Error tss_getTareAsRotationMatrix(TSS_Device_Id device, float * matrix9, unsigned int * timestamp);
// 130(0x82)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer trust values
 *
 * Returns the current accelerometer min and max trust values. If static trust values were set, both of these will be the same.
 *
 * Input:
 * None
 *
 * Output:
 * Accelerometer trust values, min and max (float x2)
 ********************************************/
//TSS_EXPORT TSS_Error tss_getAccelerometerRhoValue(TSS_Device_Id device, unsigned char * rho_mode, float * rho_min, float * rho_max, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_getAccelerometerTrustValues(TSS_Device_Id device, float * min_trust_value, float * max_trust_value, unsigned int * timestamp);
// 131(0x83)
 /*****************AutoGenerated*************//**
 * \brief Get compass trust values
 *
 * Returns the current compass min and max trust values. If static trust values were set, both of these will be the same.
 *
 * Input:
 * None
 *
 * Output:
 * Compass trust values, min and max (float x2)
 ********************************************/
//TSS_EXPORT TSS_Error tss_getCompassRhoValue(TSS_Device_Id device, unsigned char * rho_mode, float * rho_min, float * rho_max, unsigned int * timestamp);
TSS_EXPORT TSS_Error tss_getCompassTrustValues(TSS_Device_Id device, float * min_trust_value, float * max_trust_value, unsigned int * timestamp);
// 132(0x84)
 /*****************AutoGenerated*************//**
 * \brief Get current update rate
 *
 * Reads the amount of time taken by the last filter update step.
 *
 * Input:
 * None
 *
 * Output:
 * Last update time in microseconds (int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCurrentUpdateRate(TSS_Device_Id device, unsigned int * last_update, unsigned int * timestamp);
// 133(0x85)
 /*****************AutoGenerated*************//**
 * \brief Get compass reference vector
 *
 * Reads the current compass reference vector. Note that this is not valid if the sensor is in Multi Reference Vector mode. 
 *
 * Input:
 * None
 *
 * Output:
 * Compass reference vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCompassReferenceVector(TSS_Device_Id device, float * reference_vector, unsigned int * timestamp);
// 134(0x86)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer reference vector
 *
 * Reads the current compass reference vector. Note that this is not valid if the sensor is in Multi Reference Vector mode. 
 *
 * Input:
 * None
 *
 * Output:
 * Accelerometer reference vector (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAccelerometerReferenceVector(TSS_Device_Id device, float * reference_vector, unsigned int * timestamp);
// 135(0x87)
 /*****************AutoGenerated*************//**
 * \brief Get reference vector mode
 *
 * Reads the current reference vector mode. Return value can be 0 for single static, 1 for single auto, 2 for single auto continuous or 3 for multi.
 *
 * Input:
 * None
 *
 * Output:
 * Mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getReferenceVectorMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 140(0x8c)
 /*****************AutoGenerated*************//**
 * \brief Get gyroscope enabled state
 *
 * Returns a value indicating whether the gyroscope contribution is currently part of the orientation estimate: 0 for off, 1 for on. 
 *
 * Input:
 * None
 *
 * Output:
 * Gyroscope enabled value (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getGyroscopeEnabledState(TSS_Device_Id device, unsigned char * enabled, unsigned int * timestamp);
// 141(0x8d)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer enabled state
 *
 * Returns a value indicating whether the accelerometer contribution is currently part of the orientation estimate: 0 for off, 1 for on. 
 *
 * Input:
 * None
 *
 * Output:
 * Accelerometer enabled value (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAccelerometerEnabledState(TSS_Device_Id device, unsigned char * enabled, unsigned int * timestamp);
// 142(0x8e)
 /*****************AutoGenerated*************//**
 * \brief Get compass enabled state
 *
 * Returns a value indicating whether the compass contribution is currently part of the orientation estimate: 0 for off, 1 for on. 
 *
 * Input:
 * None
 *
 * Output:
 * Compass enabled value (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCompassEnabledState(TSS_Device_Id device, unsigned char * enabled, unsigned int * timestamp);
// 143(0x8f)
 /*****************AutoGenerated*************//**
 * \brief Get axis direction
 *
 * Returns a value indicating the current axis direction setup. For more information on the meaning of this value, please refer to the Set Axis Direction command (116).
 *
 * Input:
 * None
 *
 * Output:
 * Axis direction value (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAxisDirections(TSS_Device_Id device, unsigned char * axis_directions, unsigned int * timestamp);
// 144(0x90)
 /*****************AutoGenerated*************//**
 * \brief Get oversample rate
 *
 * Returns a value indicating how many times each component sensor is sampled before being stored as raw data. A value of 1 indicates that no oversampling is taking place, while a value that is higher indicates the number of samples per component sensor per filter update step.
 *
 * Input:
 * None
 *
 * Output:
 * Oversample rate (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getOversampleRate(TSS_Device_Id device, unsigned char * oversample_rate, unsigned int * timestamp);
// 145(0x91)
 /*****************AutoGenerated*************//**
 * \brief Get running average percent
 *
 * Returns a value indicating how heavily the orientation estimate is based upon the estimate from the previous frame. For more information on the meaning of this value, please refer to the Set Running Average Percent command (117).
 *
 * Input:
 * None
 *
 * Output:
 * Running average percent (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getRunningAveragePercent(TSS_Device_Id device, float * running_average_percent, unsigned int * timestamp);
// 146(0x92)
 /*****************AutoGenerated*************//**
 * \brief Get desired update rate
 *
 * Returns the current desired update rate. Note that this value does not indicate the actual update rate, but instead indicates the value that should be spent 'idling' in the main loop. Thus, without having set a specified desired update rate, this value should read 0.
 *
 * Input:
 * None
 *
 * Output:
 * Desired update rate in microseconds (int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getDesiredUpdateRate(TSS_Device_Id device, unsigned int * update_rate, unsigned int * timestamp);
// 148(0x94)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer range
 *
 * Return the current accelerometer measurement range, which can be a 0 for 2g, 1 for 4g or a 2 for 8g. 
 *
 * Input:
 * None
 *
 * Output:
 * Accelerometer range setting (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAccelerometerRange(TSS_Device_Id device, unsigned char * accelerometer_range_setting, unsigned int * timestamp);
// 152(0x98)
 /*****************AutoGenerated*************//**
 * \brief Get filter mode
 *
 * Returns the current filter mode, which can be 0 for IMU mode, 1 for Kalman, 2 for Alternating Kalman, 3 for Complementary, or 4 for Quaternion Gradient Descent. For more information, please refer to the Set Filter Mode command (123).
 *
 * Input:
 * None
 *
 * Output:
 * Filter mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getFilterMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 153(0x99)
 /*****************AutoGenerated*************//**
 * \brief Get running average mode
 *
 * Reads the selected mode for the running average, which can be 0 for normal or 1 for confidence.
 *
 * Input:
 * None
 *
 * Output:
 * Running average mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getRunningAverageMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 154(0x9a)
 /*****************AutoGenerated*************//**
 * \brief Get gyroscope range
 *
 * Reads the current gyroscope measurement range, which can be 0 for 250 DPS, 1 for 500 DPS or 2 for 2000 DPS.
 *
 * Input:
 * None
 *
 * Output:
 * Gyroscope range setting (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getGyroscopeRange(TSS_Device_Id device, unsigned char * gyroscope_range_setting, unsigned int * timestamp);
// 155(0x9b)
 /*****************AutoGenerated*************//**
 * \brief Get compass range
 *
 * Reads the current compass measurement range, which can be 0 for 0.88G, 1 for 1.3G, 2 for 1.9G, 3 for 2.5G, 4 for 4.0G, 5 for 4.7G, 6 for 5.6G or 7 for 8.1G.
 *
 * Input:
 * None
 *
 * Output:
 * Compass range setting (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCompassRange(TSS_Device_Id device, unsigned char * compass_range_setting, unsigned int * timestamp);
// 156(0x9c)
 /*****************AutoGenerated*************//**
 * \brief Get euler angle decomposition order
 *
 * Reads the current euler angle decomposition order.
 *
 * Input:
 * None
 *
 * Output:
 * Euler angle decomposition order (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getEulerAngleDecompositionOrder(TSS_Device_Id device, unsigned char * order, unsigned int * timestamp);
// 157(0x9d)
 /*****************AutoGenerated*************//**
 * \brief Get magnetoresistive threshold
 *
 * Reads the current magnetoresistive threshold parameters.
 *
 * Input:
 * None
 *
 * Output:
 * Magnetoresistive threshold in gauss(float), Number of magnetometer trust frames (unsigned int), magnetometer lockout decay value (float), magnetometer perturbation detection value (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getMagnetoresistiveThreshold(TSS_Device_Id device, float * threshold, unsigned int * trust_frames, float * lockout_decay, float * perturbation_detection_value, unsigned int * timestamp);
// 158(0x9e)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer resistance threshold
 *
 * Reads the current accelerometer threshold parameters.
 *
 * Input:
 * None
 *
 * Output:
 * Accelerometer threshold in g's (float), Number of accelerometer lockout frames (unsigned int), 
 ********************************************/
TSS_EXPORT TSS_Error tss_getAccelerometerResistanceThreshold(TSS_Device_Id device, float * threshold, unsigned int * lockout_frames, unsigned int * timestamp);
// 159(0x9f)
 /*****************AutoGenerated*************//**
 * \brief Get offset orientation as quaternion
 *
 * Returns the current offset orientation as a quaternion.
 *
 * Input:
 * None
 *
 * Output:
 * Quaternion (float x4)
 ********************************************/
TSS_EXPORT TSS_Error tss_getOffsetOrientationAsQuaternion(TSS_Device_Id device, float * quat4, unsigned int * timestamp);

//Calibration Commands
// 160(0xa0)
 /*****************AutoGenerated*************//**
 * \brief Set compass calibration coefficients
 *
 * Sets the current compass calibration parameters to the specified values. These consist of a bias which is added to the raw data vector and a matrix by which the value is multiplied. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Matrix (float x9), Bias (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setCompassCalibrationCoefficients(TSS_Device_Id device, const float * matrix9, const float * bias3, unsigned int * timestamp);
// 161(0xa1)
 /*****************AutoGenerated*************//**
 * \brief Set accelerometer calibration coefficients
 *
 * Sets the current accelerometer calibration parameters to the specified values. These consist of a bias which is added to the raw data vector and a matrix by which the value is multiplied. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Matrix (float x9), Bias (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setAccelerometerCalibrationCoefficients(TSS_Device_Id device,const float * matrix9, const float * bias3, unsigned int * timestamp);
// 162(0xa2)
 /*****************AutoGenerated*************//**
 * \brief Get compass calibration coefficients
 *
 * Return the current compass calibration parameters.
 *
 * Input:
 * None
 *
 * Output:
 * Matrix (float x9), Bias (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCompassCalibrationCoefficients(TSS_Device_Id device, float * matrix9, float * bias3, unsigned int * timestamp);
// 163(0xa3)
 /*****************AutoGenerated*************//**
 * \brief Get accelerometer calibration coefficients
 *
 * Return the current accelerometer calibration parameters.
 *
 * Input:
 * None
 *
 * Output:
 * Matrix (float x9), Bias (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getAccelerometerCalibrationCoefficients(TSS_Device_Id device, float * matrix9, float * bias3, unsigned int * timestamp);
// 164(0xa4)
 /*****************AutoGenerated*************//**
 * \brief Get gyroscope calibration coefficients
 *
 * Return the current gyroscope calibration parameters.
 *
 * Input:
 * None
 *
 * Output:
 * Matrix (float x9), Bias (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getGyroscopeCalibrationCoefficients(TSS_Device_Id device, float * matrix9, float * bias3, unsigned int * timestamp);
// 165(0xa5)
 /*****************AutoGenerated*************//**
 * \brief Begin gyroscope auto-calibration
 *
 * Performs auto-gyroscope calibration. Sensor should remain still while samples are taken. The gyroscope bias will be automatically placed into the bias part of the gyroscope calibration coefficient list.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_beginGyroscopeAutoCalibration(TSS_Device_Id device, unsigned int * timestamp);
// 166(0xa6)
 /*****************AutoGenerated*************//**
 * \brief Set gyroscope calibration coefficients
 *
 * Sets the current gyroscope calibration parameters to the specified values. These consist of a bias which is added to the raw data vector and a matrix by which the value is multiplied. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Matrix (float x9), Bias (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setGyroscopeCalibrationCoefficients(TSS_Device_Id device, const float * matrix9, const float * bias3, unsigned int * timestamp);
// 169(0xa9)
 /*****************AutoGenerated*************//**
 * \brief Set calibration mode
 *
 * Sets the current calibration mode, which can be 0 for Bias, 1 for Scale-Bias and 2 for Ortho-Calibration. For more information, refer to section 3.1.3 Additional Calibration. This setting can be saved to non-volatile flash memory using the Commit Settings command.
 *
 * Input:
 * Mode (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setCalibrationMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 170(0xaa)
 /*****************AutoGenerated*************//**
 * \brief Get calibration mode
 *
 * Reads the current calibration mode, which can be 0 for Bias, 1 for Scale-Bias or 2 for Ortho-Calibration. For more information, refer to section 3.1.3 Additional Calibration.
 *
 * Input:
 * None
 *
 * Output:
 * Mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getCalibrationMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 171(0xab)
 /*****************AutoGenerated*************//**
 * \brief Set ortho-calibration data point from current orientation
 *
 * Set the ortho-calibration compass and accelerometer vectors corresponding to this orthogonal orientation. Intended for advanced users.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setOrthoCalibrationDataPointFromCurrentOrientation(TSS_Device_Id device, unsigned int * timestamp);
// 172(0xac)
 /*****************AutoGenerated*************//**
 * \brief Set ortho-calibration data point from vector
 *
 * Directly set a vector corresponding to this orthogonal orientation. First parameter is type, where 0 is for compass and 1 is for accelerometer. Second parameter is index, which indicates the orthogonal orientation. Intended for advanced users.
 *
 * Input:
 * Type (Byte), Index (Byte), Accelerometer or Compass Vector (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setOrthoCalibrationDataPointFromVector(TSS_Device_Id device, unsigned char type, unsigned char index, const float * vector3, unsigned int * timestamp);
// 173(0xad)
 /*****************AutoGenerated*************//**
 * \brief Get ortho-calibration data point
 *
 * Return the vector corresponding to the orthogonal orientation given by index. First parameter is type, where 0 is for compass and 1 is for accelerometer. Second parameter is index, which indicates the orthogonal orientation. Intended for advanced users.
 *
 * Input:
 * Type (Byte), Index (Byte)
 *
 * Output:
 * Accelerometer or compass vector (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getOrthoCalibrationDataPoint(TSS_Device_Id device, unsigned char type, unsigned char index, float * vector3, unsigned int * timestamp);
// 174(0xae)
 /*****************AutoGenerated*************//**
 * \brief Perform ortho-calibration
 *
 * Stores accelerometer and compass data in the ortho-lookup table for use in the orientation fusion algorithm. For best results, each of the 24 orientations should be filled in with component sensor data. Note also that ortho-calibration data will not be used unless the calibration mode is set to Ortho-Calibration. For more information, refer to Section 3.1.3 Additional Calibration. Intended for advanced users. 
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_performOrthoCalibration(TSS_Device_Id device, unsigned int * timestamp);
// 175(0xaf)
 /*****************AutoGenerated*************//**
 * \brief Clear ortho-calibration data
 *
 * Clear out all ortho-lookup table data. Intended for advanced users.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_clearOrthoCalibrationData(TSS_Device_Id device, unsigned int * timestamp);

//Dongle Commands
// 176(0xb0)
 /*****************AutoGenerated*************//**
 * \brief Set wireless stream mode
 *
 * Set the wireless communication's streaming flush mode. If this value is set to 0 (default), data must be 'released' using manual flush commands. If this value is set to 1, data will be output immediately via the dongle's USB connection. Note that this only exists for wireless communication. For more information, refer to Section 3.2.2 and 3.3.3. This setting can be set to non-volatile flash memory by using the Commit Settings command.
 *
 * Input:
 * Auto-flush mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessStreamingAutoFlushMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 177(0xb1)
 /*****************AutoGenerated*************//**
 * \brief Get wireless stream mode
 *
 * Returns the wireless communication's current asynchronous flush mode, which can be 0 for auto flush and 1 for manual flush. For more information, refer to Section 3.2.2 and Section 3.3.3.
 *
 * Input:
 * None
 *
 * Output:
 * Auto-flush mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessStreamingAutoFlushMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 178(0xb2)
 /*****************AutoGenerated*************//**
 * \brief Set wireless streaming manual flush bitfield
 *
 * Allows the dongle to control which wirelessly received data is output via manual flush mode. The parameter represents a bitfield that represents which wireless sensors' logical IDs can currently output data. If a bit for a corresponding sensor is set to 0, no data at all will be output for that sensor in any condition, even if data is received for that sensor. This setting can be set to non-volatile flash memory by using the Commit Settings command.
 *
 * Input:
 * Manual flush bitfield (short)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessStreamingManualFlushBitfield(TSS_Device_Id device, unsigned short manual_flush_bitfield, unsigned int * timestamp);
// 179(0xb3)
 /*****************AutoGenerated*************//**
 * \brief Get async flush bitfield
 *
 * Returns the current manual flush bitfield representing which logical Ids will respond to asynchronous requests.
 *
 * Input:
 * None
 *
 * Output:
 * Manual flush bitfield (short)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessStreamingManualFlushBitfield(TSS_Device_Id device, unsigned short * manual_flush_bitfield, unsigned int * timestamp);
// 180(0xb4)
 /*****************AutoGenerated*************//**
 * \brief Manual flush single
 *
 * Flush data output for a single logical ID. For more information, please refer to Section 3.2.2 and Section 3.2.3
 *
 * Input:
 * Logical ID (Byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_getManualFlushSingle(TSS_Device_Id device, unsigned char index, char * data, int in_data_size, int * out_data_size, unsigned int * timestamp);
// 181(0xb5)
 /*****************AutoGenerated*************//**
 * \brief Manual flush bulk
 *
 * Flush data output for all logical Ids. For more information, please refer to Section 3.2.2 and Section 3.2.3
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_getManualFlushBulk(TSS_Device_Id device, char * data, int in_data_size, int * out_data_size, unsigned int * timestamp);
// 182(0xb6)
 /*****************AutoGenerated*************//**
 * \brief Broadcast synchronization pulse
 *
 * Sends out a timestamp synchronization broadcast message to all wireless sensors that are listening on the same channel and PanID as the dongle. The message will essentially set each receiving sensor's timestamp to the same timestamp as stored in the dongle. 
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_broadcastSynchronizationPulse(TSS_Device_Id device, unsigned int * timestamp);
// 208(0xd0)
 /*****************AutoGenerated*************//**
 * \brief Get serial number at logical ID
 *
 * Return the mapped serial number for the given logical ID. 
 *
 * Input:
 * Logical ID (Byte)
 *
 * Output:
 * Serial number (int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getSerialNumberAtLogicalID(TSS_Device_Id device, unsigned char logical_id, unsigned int * serial_number, unsigned int * timestamp);
// 209(0xd1)
 /*****************AutoGenerated*************//**
 * \brief Set serial number at logical ID
 *
 * Set the mapped serial number given by the logical ID. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * Logical ID (Byte), Serial number (int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setSerialNumberAtLogicalID(TSS_Device_Id device, unsigned char logical_id, unsigned int serial_number, unsigned int * timestamp);
// 210(0xd2)
 /*****************AutoGenerated*************//**
 * \brief Get wireless channel noise levels
 *
 * Return the noise levels for each of the 16 wireless channels. A higher value corresponds to a noisier channel, which can significantly impact wireless reception and throughput.
 *
 * Input:
 * None
 *
 * Output:
 * Channel strengths (byte x16)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessChannelNoiseLevels(TSS_Device_Id device, unsigned char * channel_strengths16, unsigned int * timestamp);
// 211(0xd3)
 /*****************AutoGenerated*************//**
 * \brief Set wireless retries
 *
 * Set the number of times a dongle will attempt to re-transmit a data request after timing out. Default value is 3. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * Retries (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessRetries(TSS_Device_Id device, unsigned char retries, unsigned int * timestamp);
// 212(0xd4)
 /*****************AutoGenerated*************//**
 * \brief Get wireless retries
 *
 * Read the number of times a dongle will attempt to re-transmit a data request after timing out. Default value is 3.
 *
 * Input:
 * None
 *
 * Output:
 * Retries (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessRetries(TSS_Device_Id device, unsigned char * retries, unsigned int * timestamp);
// 213(0xd5)
 /*****************AutoGenerated*************//**
 * \brief Get wireless slots open
 *
 * The dongle can simultaneously service up to sixteen individual data requests to wireless sensors. As sensors respond, requests are removed from the table. In the case that too many requests are sent to the dongle in too short a period, the dongle will begin tossing them out. This value will return the number of slots currently open. If this value is 0, no more wireless requests will be handled until some are internally processed.
 *
 * Input:
 * None
 *
 * Output:
 * Slots open (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessSlotsOpen(TSS_Device_Id device, unsigned char * slots_open, unsigned int * timestamp);
// 214(0xd6)
 /*****************AutoGenerated*************//**
 * \brief Get signal strength
 *
 * Returns a value indicating the reception strength of the most recently received packet. Higher values indicate a stronger link. 
 *
 * Input:
 * None
 *
 * Output:
 * Last packet signal strength (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getSignalStrength(TSS_Device_Id device, unsigned char * last_packet_signal_strength, unsigned int * timestamp);
// 219(0xdb)
 /*****************AutoGenerated*************//**
 * \brief Set wireless response header bitfield
 *
 * Configures the response header for data returned over a wireless connection. The only parameter is a four-byte bitfield that determines which data is prepended to all data responses. The following bits are used: 0x1: (1 byte) Success/Failure, with non-zero values representing failure.0x2: (4 bytes) Timestamp, in microseconds.0x4: (1 byte) Command echooutputs the called command. Returns 0xFF for streamed data. 0x8: (1 byte) Additive checksum over returned data, but not including response header.0x10: (1 byte) Logical ID0x20: (4 bytes) Serial number0x40: (1 byte) Data length, returns the length of the requested data, not including response header.This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * Response header bitfield (Unsigned int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessResponseHeaderBitfield(TSS_Device_Id device, unsigned int header_bitfield, unsigned int * timestamp);
// 220(0xdc)
 /*****************AutoGenerated*************//**
 * \brief Get wireless response header bitfield
 *
 * Return the current wireless response header bitfield.
 *
 * Input:
 * None
 *
 * Output:
 * Response header bitfield (Unsigned int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessResponseHeaderBitfield(TSS_Device_Id device, unsigned int * header_bitfield, unsigned int * timestamp);

//Wireless Sensor & Dongle Commands
// 192(0xc0)
 /*****************AutoGenerated*************//**
 * \brief Get wireless panID
 *
 * Return the current panID for this wireless sensor or dongle. For more information, refer to Section 2.9 Wireless Terminology. 
 *
 * Input:
 * None
 *
 * Output:
 * PanID (short)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessPanID(TSS_Device_Id device, unsigned short * pan_id, unsigned int * timestamp);
// 193(0xc1)
 /*****************AutoGenerated*************//**
 * \brief Set wireless panID
 *
 * Set the current panID for this wireless sensor or dongle. Note that the panID for a wireless sensor can only be set via the USB connection. For more information, refer to Section 2.9 Wireless Terminology. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * PanID (short)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessPanID(TSS_Device_Id device, unsigned short pan_id, unsigned int * timestamp);
// 194(0xc2)
 /*****************AutoGenerated*************//**
 * \brief Get wireless channel
 *
 * Read the current channel for this wireless sensor or dongle. For more information, refer to Section 2.9 Wireless Terminology.
 *
 * Input:
 * None
 *
 * Output:
 * Channel (Byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessChannel(TSS_Device_Id device, unsigned char * channel, unsigned int * timestamp);
// 195(0xc3)
 /*****************AutoGenerated*************//**
 * \brief Set wireless channel
 *
 * Set the current channel for this wireless sensor or dongle. For more information, refer to Section 2.9 Wireless Terminology. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * Channel (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessChannel(TSS_Device_Id device, unsigned char channel, unsigned int * timestamp);
// 197(0xc5)
 /*****************AutoGenerated*************//**
 * \brief Commit wireless settings
 *
 * Commits all current wireless settings to non-volatile flash memory, which will persist after the sensor is powered off. For more information on which parameters can be stored in this manner, refer to Section 3.4 Sensor Settings.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_commitWirelessSettings(TSS_Device_Id device, unsigned int * timestamp);
// 198(0xc6)
 /*****************AutoGenerated*************//**
 * \brief Get wireless address
 *
 * Read the wireless hardware address for this sensor or dongle.
 *
 * Input:
 * None
 *
 * Output:
 * Address (short)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessAddress(TSS_Device_Id device, unsigned short * address, unsigned int * timestamp);

//Battery Commands
// 201(0xc9)
 /*****************AutoGenerated*************//**
 * \brief Get battery voltage
 *
 * Read the current battery level in volts. Note that this value will read as slightly higher than it actually is if it is read via a USB connection.
 *
 * Input:
 * None
 *
 * Output:
 * Battery level in voltage (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getBatteryVoltage(TSS_Device_Id device, float * battery_voltage, unsigned int * timestamp);
// 202(0xca)
 /*****************AutoGenerated*************//**
 * \brief Get battery percent remaining
 *
 * Read the current battery lifetime as a percentage of the total. Note that this value will read as slightly higher than it actually is if it is read via a USB connection.
 *
 * Input:
 * None
 *
 * Output:
 * Battery level as percent (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getBatteryPercentRemaining(TSS_Device_Id device, unsigned char * battery_percent, unsigned int * timestamp);
// 203(0xcb)
 /*****************AutoGenerated*************//**
 * \brief Get battery status
 *
 * Returns a value indicating the current status of the battery, which can be a 3 to indicate that the battery is currently not charging, a 2 to indicate that the battery is charging and thus plugged in, or a 1 to indicate that the sensor is fully charged. 
 *
 * Input:
 * None
 *
 * Output:
 * Battery charge status (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getBatteryStatus(TSS_Device_Id device, unsigned char * battery_charge_status, unsigned int * timestamp);

//General Commands
// 196(0xc4)
 /*****************AutoGenerated*************//**
 * \brief Set LED Mode
 *
 * Allows finer-grained control over the sensor LED. Accepts a single parameter that can be 0 for standard, which displays all standard LED status indicators or 1 for static, which displays only the LED color as specified by command 238.
 *
 * Input:
 * LED mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setLEDMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 200(0xc8)
 /*****************AutoGenerated*************//**
 * \brief Get LED Mode
 *
 * Returns the current sensor LED mode, which can be 0 for standard or 1 for static.
 *
 * Input:
 * None
 *
 * Output:
 * LED mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getLEDMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 221(0xdd)
 /*****************AutoGenerated*************//**
 * \brief Set wired response header bitfield
 *
 * Configures the response header for data returned over a wired connection. The only parameter is a four-byte bitfield that determines which data is prepended to all data responses. The following bits are used: 0x1: (1 byte) Success/Failure, with non-zero values representing failure.0x2: (4 bytes) Timestamp, in microseconds.0x4: (1 byte) Command echooutputs the called command. Returns 0xFF for streamed data. 0x8: (1 byte) Additive checksum over returned data, but not including response header.0x10: (1 byte) Logical ID, returns 0xFE for wired sensors. Meant to be used with 3-Space Dongle response header (For more info, see command 0xDB). 0x20: (4 bytes) Serial number0x40: (1 byte) Data length, returns the length of the requested data, not including response header.This setting can be committed to non-volatile flash memory by calling the Commit Settings command. For more information on Response Headers, please refer to Section 4.4. 
 *
 * Input:
 * Response header configuration (Unsigned int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWiredResponseHeaderBitfield(TSS_Device_Id device, unsigned int header_bitfield, unsigned int * timestamp);
// 222(0xde)
 /*****************AutoGenerated*************//**
 * \brief Get wired response header bitfield
 *
 * Return the current wired response header bitfield. For more information, please refer to Section 4.4.
 *
 * Input:
 * None
 *
 * Output:
 * Response header configuration (Unsigned int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getWiredResponseHeaderBitfield(TSS_Device_Id device, unsigned int * header_bitfield, unsigned int * timestamp);
// 223(0xdf) note: adds null terminator
 /*****************AutoGenerated*************//**
 * \brief Get firmware version string
 *
 * Returns a string indicating the current firmware version.
 *
 * Input:
 * None
 *
 * Output:
 * Firmware version (string)
 ********************************************/
TSS_EXPORT TSS_Error tss_getFirmwareVersionString(TSS_Device_Id device, char * firmware_version13, unsigned int * timestamp);
// 224(0xe0)
 /*****************AutoGenerated*************//**
 * \brief Restore factory settings
 *
 * Return all non-volatile flash settings to their original, default settings.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_restoreFactorySettings(TSS_Device_Id device, unsigned int * timestamp);
// 225(0xe1)
 /*****************AutoGenerated*************//**
 * \brief Commit settings
 *
 * Commits all current sensor settings to non-volatile flash memory, which will persist after the sensor is powered off. For more information on which parameters can be stored in this manner, refer to Section 3.4 Sensor Settings.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_commitSettings(TSS_Device_Id device, unsigned int * timestamp);
// 226(0xe2)
 /*****************AutoGenerated*************//**
 * \brief Software reset
 *
 * Resets the sensor.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_softwareReset(TSS_Device_Id device, unsigned int * timestamp);
// 227(0xe3)
 /*****************AutoGenerated*************//**
 * \brief Set sleep mode
 *
 * Sets the current sleep mode of the sensor. Supported sleep modes are 0 for NONE and 1 for IDLE. IDLE mode merely skips all filtering steps. NONE is the default state. 
 *
 * Input:
 * Sleep mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setSleepMode(TSS_Device_Id device,unsigned char mode, unsigned int * timestamp);
// 228(0xe4)
 /*****************AutoGenerated*************//**
 * \brief Get sleep mode
 *
 * Reads the current sleep mode of the sensor, which can be 0 for NONE or 1 for IDLE. 
 *
 * Input:
 * None
 *
 * Output:
 * Sleep mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getSleepMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 229(0xe5)
 /*****************AutoGenerated*************//**
 * \brief Enter bootloader mode
 *
 * Places the sensor into a special mode that allows firmware upgrades. This will case normal operation until the firmware update mode is instructed to return the sensor to normal operation. For more information on upgrading firmware, refer to the 3-Space Sensor Suite Quick Start Guide.
 *
 * Input:
 * None
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_enterBootloaderMode(TSS_Device_Id device, unsigned int * timestamp);
// 230(0xe6) note: adds null terminator
 /*****************AutoGenerated*************//**
 * \brief Get hardware version string
 *
 * Returns a string indicating the current hardware version.
 *
 * Input:
 * None
 *
 * Output:
 * Hardware version (string)
 ********************************************/
TSS_EXPORT TSS_Error tss_getHardwareVersionString(TSS_Device_Id device, char * hardware_version33, unsigned int * timestamp);
// 231(0xe7)
 /*****************AutoGenerated*************//**
 * \brief Set UART baud rate
 *
 * Sets the baud rate of the physical UART. This setting does not need to be committed, but will not take effect until the sensor is reset. Valid baud rates are 1200, 2400, 4800, 9600, 19200, 28800, 38400, 57600, 115200 (default), 230400, 460800 and 921600. Note that this is only applicable for sensor types that have UART interfaces.
 *
 * Input:
 * Baud rate (int)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setUARTBaudRate(TSS_Device_Id device, unsigned int baud_rate, unsigned int * timestamp);
// 232(0xe8)
 /*****************AutoGenerated*************//**
 * \brief Get UART baud rate
 *
 * Returns the baud rate of the physical UART. Note that this is only applicable for sensor types that have UART interfaces.
 *
 * Input:
 * None
 *
 * Output:
 * Baud rate (int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUARTBaudRate(TSS_Device_Id device, unsigned int * baud_rate, unsigned int * timestamp);
// 233(0xe9)
 /*****************AutoGenerated*************//**
 * \brief Set USB Mode
 *
 * Sets the communication mode for USB. Accepts one value that can be 0 for CDC (default) or 1 for FTDI.
 *
 * Input:
 * USB communication mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setUSBMode(TSS_Device_Id device, unsigned int mode, unsigned int * timestamp);
// 234(0xea)
 /*****************AutoGenerated*************//**
 * \brief Get USB Mode
 *
 * Returns the current USB communication mode.
 *
 * Input:
 * None
 *
 * Output:
 * USB communication mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getUSBMode(TSS_Device_Id device, unsigned int * mode, unsigned int * timestamp);
// 237(0xed)
 /*****************AutoGenerated*************//**
 * \brief Get serial number
 *
 * Returns the serial number, which will match the value etched onto the physical sensor. 
 *
 * Input:
 * None
 *
 * Output:
 * Serial number (int)
 ********************************************/
TSS_EXPORT TSS_Error tss_getSerialNumber(TSS_Device_Id device, unsigned int * serial_number, unsigned int * timestamp);
// 238(0xee)
 /*****************AutoGenerated*************//**
 * \brief Set LED color
 *
 * Sets the color of the LED on the sensor to the specified RGB color. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * RGB Color (float x3)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setLEDColor(TSS_Device_Id device,const float * rgb_color3, unsigned int * timestamp);
// 239(0xef)
 /*****************AutoGenerated*************//**
 * \brief Get LED color 
 *
 * Returns the color of the LED on the sensor.
 *
 * Input:
 * None
 *
 * Output:
 * RGB Color (float x3)
 ********************************************/
TSS_EXPORT TSS_Error tss_getLEDColor(TSS_Device_Id device, float * rgb_color3, unsigned int * timestamp);

//Wireless HID Commands
// 215(0xd7)
 /*****************AutoGenerated*************//**
 * \brief Set wireless HID update rate
 *
 * Specify the interval at which HID information is requested by the dongle. The default and minimum value is 15ms in synchronous HID mode. In asynchronous HID mode, the minimum is 5ms. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command.
 *
 * Input:
 * HID update rate in milliseconds (byte)
 *
 * Output:
 * Last packet signal strength (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessHIDUpdateRate(TSS_Device_Id device, unsigned char HID_update_rate, unsigned int * timestamp);
// 216(0xd8)
 /*****************AutoGenerated*************//**
 * \brief Get wireless HID update rate
 *
 * Return the interval at which HID information is requested by the dongle. 
 *
 * Input:
 * None
 *
 * Output:
 * HID update rate in milliseconds
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessHIDUpdateRate(TSS_Device_Id device, unsigned char * HID_update_rate, unsigned int * timestamp);
// 217(0xd9)
 /*****************AutoGenerated*************//**
 * \brief Set wireless HID asynchronous mode
 *
 * Sets the current wireless HID communication mode. Supplying a 0 makes wireless HID communication synchronous, while a 1 makes wireless HID asynchronous. For more information, refer to Section 3.3.4 Wireless Joystick/Mouse. This setting can be committed to non-volatile flash memory by calling the Commit Wireless Settings command. 
 *
 * Input:
 * HID communication mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setWirelessHIDAsynchronousMode(TSS_Device_Id device, unsigned char HID_communication_mode , unsigned int * timestamp);
// 218(0xda)
 /*****************AutoGenerated*************//**
 * \brief Get wireless HID asynchronous mode
 *
 * Returns the current wireless HID communication mode, which can be a 0 for synchronous wireless HID or a 1 for asynchronous wireless HID.
 *
 * Input:
 * None
 *
 * Output:
 * HID communication mode
 ********************************************/
TSS_EXPORT TSS_Error tss_getWirelessHIDAsynchronousMode(TSS_Device_Id device, unsigned char * HID_communication_mode , unsigned int * timestamp);
// 240(0xf0)
 /*****************AutoGenerated*************//**
 * \brief Set joystick logical ID
 *
 * Causes the sensor at the specified logical ID to return joystick HID data. Passing a -1 will disable wireless joystick data. For more information, refer to Section 3.3.4 Wireless Joystick/Mouse. 
 *
 * Input:
 * Joystick logical ID (signed byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setJoystickLogicalID(TSS_Device_Id device, unsigned char logical_ID , unsigned int * timestamp);
// 241(0xf1)
 /*****************AutoGenerated*************//**
 * \brief Set mouse logical ID
 *
 * Causes the sensor at the specified logical ID to return mouse HID data. Passing a -1 will disable wireless mouse data. For more information, refer to Section 3.3.4 Wireless Joystick/Mouse.
 *
 * Input:
 * Mouse logical ID (signed byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setMouseLogicalID(TSS_Device_Id device, unsigned char * logical_ID , unsigned int * timestamp);
// 242(0xf2)
 /*****************AutoGenerated*************//**
 * \brief Get joystick logical ID
 *
 * Returns the current logical ID of the joystick-enabled sensor or -1 if none exists.
 *
 * Input:
 * None
 *
 * Output:
 * Joystick-enabled logical ID (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getJoystickLogicalID(TSS_Device_Id device, unsigned char * logical_ID , unsigned int * timestamp);
// 243(0xf3)
 /*****************AutoGenerated*************//**
 * \brief Get mouse logical ID
 *
 * Returns the current logical ID of the mouse-enabled sensor or -1 if none exists.
 *
 * Input:
 * None
 *
 * Output:
 * Mouse-enabled logical ID (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getMouseLogicalID(TSS_Device_Id device, unsigned char * logical_ID , unsigned int * timestamp);

//Wired HID Commands
// 240(0xf0)
 /*****************AutoGenerated*************//**
 * \brief Set joystick enabled
 *
 * Enable or disable streaming of joystick HID data for this sensor.
 *
 * Input:
 * Joystick enabled state (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setJoystickEnabled(TSS_Device_Id device, unsigned char enabled_state , unsigned int * timestamp);
// 241(0xf1)
 /*****************AutoGenerated*************//**
 * \brief Set mouse enabled
 *
 * Enable or disable streaming of mouse HID data for this sensor.
 *
 * Input:
 * Mouse enabled state (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setMouseEnabled(TSS_Device_Id device, unsigned char enabled_state , unsigned int * timestamp);
// 242(0xf2)
 /*****************AutoGenerated*************//**
 * \brief Get joystick enabled
 *
 * Read whether the sensor is currently streaming joystick HID data.
 *
 * Input:
 * None
 *
 * Output:
 * Joystick enabled state (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getJoystickEnabled(TSS_Device_Id device, unsigned char * enabled_state , unsigned int * timestamp);
// 243(0xf3)
 /*****************AutoGenerated*************//**
 * \brief Get mouse enabled
 *
 * Read whether the sensor is currently streaming mouse HID data.
 *
 * Input:
 * None
 *
 * Output:
 * Mouse enabled state (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getMouseEnabled(TSS_Device_Id device, unsigned char * enabled_state , unsigned int * timestamp);

//General HID Commands
// 244(0xf4)
 /*****************AutoGenerated*************//**
 * \brief Set control mode
 *
 * Sets the operation mode for one of the controls. The first parameter is the control class,which can be 0 for Joystick Axis, 1 for Joystick Button, 2 for Mouse Axis or 3 for Mouse Button. There are two axes and eight buttons on the joystick and mouse. The second parameter, the control index, selects which one of these axes or buttons you would like to modify. The third parameter, the handler index, specifies which handler you want to take care of this control. These can be the following:Turn off this control: 255Axes:Global Axis: 0Screen Point: 1Buttons:Hardware Button: 0Orientation Button: 1Shake Button: 2 
 *
 * Input:
 * Control class (byte), control index (byte), handler index (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setControlMode(TSS_Device_Id device, unsigned char control_class, unsigned char control_index, unsigned char handler_index, unsigned int * timestamp);
// 245(0xf5)
 /*****************AutoGenerated*************//**
 * \brief Set control data
 *
 * Sets parameters for the specified control's operation mode. The control classes and indices are the same as described in command 244. Each mode can have up to 10 data points associated with it. How many should be set and what they should be set to is entirely based on which mode is being used. 
 *
 * Input:
 * Control class (byte), control index (byte), data point index (byte), data point (float)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setControlData(TSS_Device_Id device, unsigned char control_class, unsigned char control_index, unsigned char data_point_index, float data_point, unsigned int * timestamp);
// 246(0xf6)
 /*****************AutoGenerated*************//**
 * \brief Get control mode
 *
 * Reads the handler index of this control's mode. The control classes and indices are the same as described in command 244.
 *
 * Input:
 * Control class (byte), control index (byte)
 *
 * Output:
 * Handler index (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getControlMode(TSS_Device_Id device, unsigned char control_class, unsigned char control_index, unsigned char * handler_index, unsigned int * timestamp);
// 247(0xf7)
 /*****************AutoGenerated*************//**
 * \brief Get control data
 *
 * Reads the value of a certain parameter of the specified control's operation mode. The control classes and indices are the same as described in command 244.
 *
 * Input:
 * Control class (byte), control index (byte), data point index (byte)
 *
 * Output:
 * Data point (float)
 ********************************************/
TSS_EXPORT TSS_Error tss_getControlData(TSS_Device_Id device, unsigned char control_class, unsigned char control_index, unsigned char data_point_index, float * data_point, unsigned int * timestamp);
// 248(0xf8)
 /*****************AutoGenerated*************//**
 * \brief Set button gyro disable length
 *
 * Determines how long, in frames, the gyros should be disabled after one of the physical buttons on the sensor is pressed. A setting of 0 means they won't be disabled at all. This setting helps to alleviate gyro disturbances cause by the buttons causing small shockwaves in the sensor. 
 *
 * Input:
 * Number of frames (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setButtonGyroDisableLength(TSS_Device_Id device, unsigned char number_of_frames, unsigned int * timestamp);
// 249(0xf9)
 /*****************AutoGenerated*************//**
 * \brief Get button gyro disable lentgh
 *
 * Returns the current button gyro disable length. 
 *
 * Input:
 * None
 *
 * Output:
 * Number of frames (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getButtonGyroDisableLength(TSS_Device_Id device, unsigned char * number_of_frames, unsigned int * timestamp);
// 250(0xfa)
 /*****************AutoGenerated*************//**
 * \brief Get button state
 *
 * Reads the current state of the sensor's physical buttons. This value returns a byte, where each bit represents the state of the sensor's physical buttons.
 *
 * Input:
 * None
 *
 * Output:
 * Button state (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getButtonState(TSS_Device_Id device, unsigned char * button_state, unsigned int * timestamp);
// 251(0xfb)
 /*****************AutoGenerated*************//**
 * \brief Set mouse absolute/relative mode
 *
 * Puts the mode in absolute or relative mode. This change will not take effect immediately and the sensor must be reset before the mouse will enter this mode. The only parameter can be 0 for absolute (default) or 1 for relative
 *
 * Input:
 * Absolute or relative mode (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setMouseAbsoluteRelativeMode(TSS_Device_Id device, unsigned char mode, unsigned int * timestamp);
// 252(0xfc)
 /*****************AutoGenerated*************//**
 * \brief Get mouse absolute/relative mode
 *
 * Return the current mouse absolute/relative mode. Note that if the sensor has not been reset since it has been put in this mode, the mouse will not reflect this change yet, even though the command will.
 *
 * Input:
 * None
 *
 * Output:
 * Absolute or relative mode (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getMouseAbsoluteRelativeMode(TSS_Device_Id device, unsigned char * mode, unsigned int * timestamp);
// 253(0xfd)
 /*****************AutoGenerated*************//**
 * \brief Set joystick and mouse present/removed
 *
 * Sets whether the joystick and mouse are present or removed. The first parameter is for the joystick, and can be 0 for removed or 1 for present. The second parameter is for the mouse. If removed, they will not show up as devices on the target system at all. For these changes to take effect, the sensor driver may need to be reinstalled. 
 *
 * Input:
 * Joystick present/removed (byte), Mouse present/removed (byte)
 *
 * Output:
 * None
 ********************************************/
TSS_EXPORT TSS_Error tss_setJoystickAndMousePresentRemoved(TSS_Device_Id device, unsigned char joystick, unsigned char mouse, unsigned int * timestamp);
// 254(0xfe)
 /*****************AutoGenerated*************//**
 * \brief Get joystick and mouse present/removed
 *
 * Returns whether the joystick and mouse are present or removed.
 *
 * Input:
 * None
 *
 * Output:
 * Joystick present/removed (byte), Mouse present/removed (byte)
 ********************************************/
TSS_EXPORT TSS_Error tss_getJoystickAndMousePresentRemoved(TSS_Device_Id device, unsigned char * joystick, unsigned char * mouse, unsigned int * timestamp);



#ifdef __cplusplus
    }
#endif

#endif // YEI_THREESPACE_API_H_INCLUDED
