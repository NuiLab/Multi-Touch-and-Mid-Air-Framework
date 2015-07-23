#ifndef UNITTEST_H
#define UNITTEST_H

#include "shapedata.h"
#include "globaldata.h"
#include "processorthread.h"

class UnitTest {
public:
	static bool run();
private:
	static float INF, NEG_INF, NaN;
};

#endif