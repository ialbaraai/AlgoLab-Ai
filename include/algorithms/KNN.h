#ifndef KNN_H

#define KNN_H

#include "../utilities/point.h"

#include <vector>

class KNearestNeighbor
{
private:
	double distance(const Point_T& fpoint, const Point_T& spoint);
public:
	KNearestNeighbor();

	void Simulate(std::vector<Point_T>& points, std::vector<Point_T>& targetPoints, const int K);
};

#endif