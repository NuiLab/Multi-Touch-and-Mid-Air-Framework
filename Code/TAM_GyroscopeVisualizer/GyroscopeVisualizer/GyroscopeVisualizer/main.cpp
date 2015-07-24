#include "gyroscopevisualizer.h"
#include "UnitTest.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GyroscopeVisualizer w;
	w.show();

#ifdef TESTING_ENABLED
	UnitTest::run();
#endif

	return a.exec();
}
