#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <cstdlib>
#include <qobject.h>

# define PI_L	3.141592653589793238462643383279502884L /* Pi */

using namespace std;

/* The GlobalData Files is used to store variables, methods, structures, and classes that may
   be used throughout the project. This provides a way to modify global values that will
   reach all parts of the project in one easily accessible place. */

/*The structure to hold touch-screen input data*/
struct touch_data
{
	long long x;
	long long y;
	int id;
	long long time;	
};

/* Enumerations of the different display settings available currently*/
enum DisplaySetting { NONE, AVG, MST, CIRCLE, SHP, CUBE, PYRA, TRIG, COMP, STRUCT, SQUARE };
/* Maps a number to the respective enum*/
static const DisplaySetting toDS[11] = { NONE, AVG, MST, CIRCLE, SHP, CUBE, PYRA, TRIG, COMP, STRUCT, SQUARE };

/* Algorithm class to store all computationally-expensive calculations 
   It's purpose is to have easy access for quickly updating the method
   if a more-improved algorithm gets written up.*/
class Algorithm {
public:
	static bool getCircumcenter(touch_data data1, touch_data data2, touch_data data3, int accuracy,
		float &centerX, float &centerY, float &radius);

	static QList<int> getShortestHamiltonianPath(QList< QList<int> > dist, int &res);
};
#endif