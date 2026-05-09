#ifndef KMC_H

#define KMC_H

#include "../utilities/point.h"
#include "../utilities/centroid.h"

#include <vector>

class KMeansClustering
{
private:
	double distance(const Point_T& point, const Centroid_T& centroid);

	void AssignPointsToClustersImpl(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids);
	void AssignClusterCentroidsImpl(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids);
public:
	KMeansClustering();
	void Simulate(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids);
};

#endif