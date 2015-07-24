#ifndef __Unit_Test_H__
#define __Unit_Test_H__

// Comment out to disable unit testing
//#define TESTING_ENABLED

#include <QtOpenGL>
#include <QtGlobal>

#include "GyroThread.h"
#include "glwindow.h"

class UnitTest : public QObject
{
	Q_OBJECT

	public:
		static void run();

		static bool Test_gyroRotations(QMatrix4x4 result, QMatrix4x4 matrix, QString testno);
};

#endif