#include "UnitTest.h"

const float eps = 1e-3;

bool UnitTest::Test_gyroRotations(QMatrix4x4 result, QMatrix4x4 matrix, QString testno)
{
	bool passed = true;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float res = result(i, j);
			float ans = matrix(i, j);

			//Q_ASSERT(qAbs(res - ans) < eps);
			if (qAbs(res - ans) > eps)
				passed = false;
		}
	}

	if (passed)
		qDebug() << "Test " << qPrintable(testno) << " [PASSED]";
	else
		qDebug() << "Test " << qPrintable(testno) << " [FAILED]";

	return passed;
}

void UnitTest::run()
{
	bool allPassed = true;

	QElapsedTimer timer;
	timer.start();

	// Test OpenGL Gyroscope Rotation code
	{
		/*
		* Initial position for all tests begin with the gyroscope facing the dongle
		* and then rotating according to the orientation on the test.
		*/

		qDebug() << "Running Gyroscope Rotation Tests";

		// Test 1 - Gyroscope Laying flat without motion
		QMatrix4x4 result_1 = GLWindow::quatToMat(0.998835, 0.000543857, 0.0252765, 0.0410958);
		QMatrix4x4 matrix_1
			(
				-0.995344, -0.0820683, 0.0505387, 0,
				-0.0821233, 0.996622, 0.00099123, 0,
				-0.0504493, -0.00316379, -0.998721, 0,
				0, 0, 0, 1
			);

		allPassed = allPassed && UnitTest::Test_gyroRotations(result_1, matrix_1, "1");

		// Test 2 - Gyroscope pointing upward 
		QMatrix4x4 result_2 = GLWindow::quatToMat(0.687756, -0.714691, 0.0955641, 0.0841234);
		QMatrix4x4 matrix_2
			(
				-0.967582, -0.25231, 0.0112051, 0,
				0.0208848, -0.0357194, 0.999144, 0,
				-0.251694, 0.966987, 0.0398307, 0,
				0, 0, 0, 1
			);

		allPassed = allPassed && UnitTest::Test_gyroRotations(result_2, matrix_2, "2");

		// Test 3 - Gyroscope Pointing downward 
		QMatrix4x4 result_3 = GLWindow::quatToMat(0.675038, 0.727124, -0.099769, 0.0752347);
		QMatrix4x4 matrix_3
			(
				-0.968772, -0.246662, -0.0252858, 0,
				0.0435163, -0.0687395, -0.996685, 0,
				0.244106, -0.966661, 0.0773266, 0,
				0, 0, 0, 1
			);

		allPassed = allPassed && UnitTest::Test_gyroRotations(result_3, matrix_3, "3");

		// Test 4 - Gyroscope pointing right
		QMatrix4x4 result_4 = GLWindow::quatToMat(0.68999, -0.0143567, 0.723573, -0.0122754);
		QMatrix4x4 matrix_4
			(
				0.0474161, -0.00383639, 0.998868, 0,
				0.037716, 0.999286, 0.00204763, 0, 
				-0.998163, 0.0375762, 0.047527, 0,
				0, 0, 0, 1
			);
		
		allPassed = allPassed && UnitTest::Test_gyroRotations(result_4, matrix_4, "4");

		// Test 5 - Gyroscope pointing left
		QMatrix4x4 result_5 = GLWindow::quatToMat(0.677845, -0.0011734, -0.735002, 0.0172121);
		QMatrix4x4 matrix_5
			(
				0.0810494, -0.0216093, -0.996476, 0, 
				-0.0250592, 0.999405, -0.023711, 0, 
				0.996395, 0.0268927, 0.0804596, 0,
				0, 0, 0, 1
			);

		allPassed = allPassed && UnitTest::Test_gyroRotations(result_5, matrix_5, "5");
	}

	if (allPassed)
		qDebug() << "ALL TESTS PASSED! " << timer.nsecsElapsed() * 1e-6 << " Milliseconds";
	else
		qDebug() << "FAILURE! " << timer.nsecsElapsed() * 1e-6 << " Milliseconds";
}