#include "unittestapi.h"

/* Tests the InputMapper Constructor */
void UnitTest::testInputMapperConstruct()
{
	TAM::InputMapper In(&UnitTest::mapper, 0.0, 0.0, 0.0, 0.0);
	assert(In.getWCoordinate() == 0.0);
	assert(In.getXCoordinate() == 0.0);
	assert(In.getYCoordinate() == 0.0);
	assert(In.getZCoordinate() == 0.0);
}

/* Tests if get and set work correctly */
void UnitTest::testInputMapperGetSet()
{
	TAM::InputMapper In;
	In.setXCoordinate(0.0);
	In.setYCoordinate(0.0);
	In.setZCoordinate(0.0);
	In.setWCoordinate(0.0);
	assert(In.getWCoordinate() == 0.0);
	assert(In.getXCoordinate() == 0.0);
	assert(In.getYCoordinate() == 0.0);
	assert(In.getZCoordinate() == 0.0);

}

/* Tests right triangle drawing and compounding */
void UnitTest::testRightTriangle()
{
	TAM::RightTriangle newTriangle(0, 0, 0, 0, 0.20f);
	TAM::InputMapper newInput;
	TAM::InputMapper tmpInput;
	newInput.setXCoordinate(0.0);
	newInput.setYCoordinate(0.0);

	newTriangle.draw(newInput);

	tmpInput = newTriangle.compound(newInput, TAM::TOP, false, false);
	assert(tmpInput.getYCoordinate() == newInput.getYCoordinate() + 0.20f);

	tmpInput = newTriangle.compound(newInput, TAM::BOT, false, false);
	std::cout << tmpInput.getYCoordinate() << std::endl ;
	assert(tmpInput.getYCoordinate() == newInput.getYCoordinate() - 0.20f);

	tmpInput = newTriangle.compound(newInput, TAM::LEFT, false, false);
	assert(tmpInput.getXCoordinate() == newInput.getXCoordinate() - 0.10f);

	tmpInput = newTriangle.compound(newInput, TAM::RIGHT, false, false);
	assert(tmpInput.getXCoordinate() == newInput.getXCoordinate() + 0.10f);

}

/* Tests the helper class functions */
void UnitTest::testHelpers()
{
	TAM::Helpers newHelper(500, 500);
	float x, y, z, w;
	newHelper.pixelsToGL2D(250, 250, 0, 0, &x, &y, &z, &w);

	assert(x == 0);
	assert(y == 0);
	assert(z == 0);
	assert(w == 0);

	/* Now test setScreenSize */
	newHelper.setScreenSize(800, 800);
	newHelper.pixelsToGL2D(400, 400, 0, 0, &x, &y, &z, &w);

	assert(x == 0);
	assert(y == 0);
	assert(z == 0);
	assert(w == 0);

	/* Test the loading of shaders */

	assert(newHelper.LoadShaders("C:/TransformVertexShader.vertexshader", "C:/ColorFragmentShader.fragmentshader") != 0);

	/* Test random color generation */
	float r = 0.0f, g = 0.0f, b = 0.0f;
	newHelper.generateColor(1, r, g, b);
	assert(r >= 0.0f && r <= 1.0f);
	assert(g >= 0.0f && g <= 1.0f);
	assert(b >= 0.0f && b <= 1.0f);
}

/* Run the Unit Test */
bool UnitTest::run()
{
	TAM::VisualizerHandler<TAM::Shape> newVis;
	testInputMapperConstruct();
	testInputMapperGetSet();
	testRightTriangle();
	testHelpers();
	return true;
}

void UnitTest::mapper(float x, float y, float z, float w, float *new_x, float *new_y, float *new_z, float *new_w) {
	*new_x = x;
	*new_y = y;
	*new_z = z;
	*new_w = w;
}