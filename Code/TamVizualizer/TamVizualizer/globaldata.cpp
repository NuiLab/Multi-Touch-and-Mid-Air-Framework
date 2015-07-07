#include "globaldata.h"
#include <math.h>

/*Calculates circumcircle given 3 touch data points*/
bool Algorithm::getCircumcenter(touch_data data1, touch_data data2, touch_data data3, int accuracy, float &centerX, float &centerY, float &radius){
	/* Efficient equation found here: https://en.wikipedia.org/wiki/Circumscribed_circle#Cartesian_coordinates */

	/* To be looked at further: "Least Square Circle Fitting"
	http://www.dtcenter.org/met/users/docs/write_ups/circle_fit.pdf */
	centerX = centerY = radius = -1;
	long long temp1, temp2, temp3;
	float denom;
	temp1 = data2.y - data3.y;
	temp2 = data3.y - data1.y;
	temp3 = data1.y - data2.y;

	denom = 2 * (data1.x*temp1 + data2.x*temp2 + data3.x*temp3);

	if (denom == 0) return false;

	long long sqlen1, sqlen2, sqlen3;
	sqlen1 = data1.x*data1.x + data1.y*data1.y;
	sqlen2 = data2.x*data2.x + data2.y*data2.y;
	sqlen3 = data3.x*data3.x + data3.y*data3.y;

	centerX = (sqlen1*temp1 + sqlen2*temp2 + sqlen3*temp3) / denom;
	centerY = (sqlen1*(data3.x - data2.x) + sqlen2*(data1.x - data3.x) + sqlen3*(data2.x - data1.x)) / denom;

	temp1 = centerX - data1.x;
	temp2 = centerY - data1.y;
	radius = temp1*temp1 + temp2*temp2;
	if (radius > accuracy*accuracy) {
		centerX = centerY = radius = -1;
		return false;
	}
	radius = sqrt(radius);
	return true;
}


QList<int> Algorithm::getShortestHamiltonianPath(QList< QList<int> > dist, int &res){
	// Algorithm retrieved from Google Source Code and converted to C++:
	// https://sites.google.com/site/indy256/algo/shortest_hamiltonian_path

	const int n = dist.size();
	const int MAX = 1 << 30;
	const int MAX_N = (1 << n);

	//int dp[1 << n][n];
	QList< QList<int> > dp;
	QList<int> order;

	for (int i = 0; i < MAX_N; i++){
		QList<int> add;
		dp.append(add);
		for (int j = 0; j < n; j++){
			dp[i].append(MAX);
		}
	}

	for (int i = 0; i < n; i++){
		order.append(-1);
		dp[(1 << i)][i] = 0;
	}

	int temp;
	for (int mask = 0; mask < 1 << n; mask++) {
		for (int i = 0; i < n; i++) {
			if ((mask & (1 << i)) != 0) {
				for (int j = 0; j < n; j++) {
					temp = dp[mask ^ (1 << i)][j] + dist[j][i];
					if ((mask & (1 << j)) != 0 && temp < dp[mask][i]) {
						dp[mask][i] = temp;//Math.min(dp[mask][i], dp[mask ^ (1 << i)][j] + dist[j][i]);
					}
				}
			}
		}
	}

	res = MAX;
	for (int i = 0; i < n; i++) {
		temp = dp[(1 << n) - 1][i];
		if (temp < res){
			res = temp;
		}
		//res = Math.min(res, dp[(1 << n) - 1][i]);
	}

	// reconstruct path
	int cur = (1 << n) - 1;
	//int order = new int[n];
	int last = -1;
	for (int i = n - 1; i >= 0; i--) {
		int bj = -1;
		for (int j = 0; j < n; j++) {
			if ((cur & 1 << j) != 0
				&& (bj == -1
				|| dp[cur][bj] + (last == -1 ? 0 : dist[bj][last]) > dp[cur][j] + (last == -1 ? 0 : dist[j][last]))) {
				bj = j;
			}
		}
		order[i] = bj;
		cur ^= (1 << bj);
		last = bj;
	}
	return order;
}