#include "GyroThread.h"
#include "yei_threespace_api.h"
#include "qmath.h"

void GyroThread::run()
{
	TSS_Device_Id	device;
	TSS_Device_Id	d_device;
	TSS_Error		tss_error;
	TSS_ComPort		comport;
	unsigned int	timestamp;

	float	quat[4];
	float	angle[3];
	float	gaccel[3];
	float	pitch, roll, heading, fAccel, pAccel = 0.0;

	QString errorMsg = "Gyroscope Connection Error!\nPlease restart the application.";

	if (tss_getComPorts(&comport, 1, 0, TSS_FIND_DNG))
	{
		d_device = tss_createTSDeviceStr(comport.com_port, TSS_TIMESTAMP_SENSOR);
		if (d_device == TSS_NO_DEVICE_ID)
		{
			// Failed to create a sensor on com_port
			emit sendError(errorMsg);
			return;
		}

		tss_getSensorFromDongle(d_device, 0, &device);

		if (device == TSS_NO_DEVICE_ID)
		{
			// Failed to create a wireless sensor
			emit sendError(errorMsg);
			return;
		}

	}
	else
	{
		// No sensors found
		emit sendError(errorMsg);
		return;
	}

	while (true)
	{
		tss_error = tss_getUntaredOrientationAsEulerAngles(device, angle, &timestamp);
		if (tss_error)
		{
			emit sendError(errorMsg);
			return;
		}

		tss_error = tss_getCorrectedLinearAccelerationInGlobalSpace(device, gaccel, &timestamp);
		if (tss_error)
		{
			emit sendError(errorMsg);
			return;
		}

		tss_error = tss_getTaredOrientationAsQuaternion(device, quat, &timestamp);
		if (tss_error)
		{
			emit sendError(errorMsg);
			return;
		}

		fAccel = sqrt(gaccel[0] * gaccel[0] + gaccel[1] * gaccel[1] + gaccel[2] * gaccel[2]);

		if (fAccel - pAccel < 0)
			fAccel *= -1;

		pAccel = fAccel;

		heading = qRadiansToDegrees(angle[1]);
		pitch = qRadiansToDegrees(angle[0]);
		roll = -qRadiansToDegrees(angle[2]);

		if (heading < 0)
			heading += 360.0;

		emit gyroData(pitch, roll, heading, fAccel, gaccel[0], gaccel[1], gaccel[2]);
		emit GLData(quat[0], quat[1], quat[2], quat[3]);
	}
}