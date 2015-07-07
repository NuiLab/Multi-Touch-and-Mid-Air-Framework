#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <cstdlib>
#include <qobject.h>

using namespace std;

# define PI_L          3.141592653589793238462643383279502884L /* pi */

/*The structure to hold the data of a touch input*/
struct touch_data
{
	long long x;
	long long y;
	int id;
	long long time;
};

enum DisplaySetting { NONE, AVG, MST, CIRCLE, SHP, CUBE };
static const DisplaySetting toDS[6] = { NONE, AVG, MST, CIRCLE, SHP, CUBE };

class Algorithm {
public:

	static bool getCircumcenter(touch_data data1, touch_data data2, touch_data data3, int accuracy,
						float &centerX, float &centerY, float &radius);

	static QList<int> getShortestHamiltonianPath(QList< QList<int> > dist, int &res);
};
#endif