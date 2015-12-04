#pragma once
#include <assert.h>
#include "api.h"

/* Collection of functions to unit test the API. 
 * All non-rendering functionality is unit-tested by these functions.
 * It is not worth it to try and unit test rendering.
 */

class UnitTest
{
private:
	static void mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w);
	void testInputMapperConstruct();
	void testInputMapperGetSet();
	void testRightTriangle();
	void testHelpers();
public:
	bool run();
};

