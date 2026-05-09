#include "../../include/algorithms/KNN.h"

#include <iostream>
#include <algorithm>
#include <map>

KNearestNeighbor::KNearestNeighbor() {}

double KNearestNeighbor::distance(const Point_T& fpoint, const Point_T& spoint)
{
	return sqrt(((fpoint.Position.X - spoint.Position.X) * (fpoint.Position.X - spoint.Position.X)) + ((fpoint.Position.Y - spoint.Position.Y) * (fpoint.Position.Y - spoint.Position.Y)));
}

void KNearestNeighbor::Simulate(std::vector<Point_T>& points, std::vector<Point_T>& targetPoints, const int K)
{
	if (K > 0 && K < points.size())
	{
		for (Point_T& point : targetPoints)
		{
			std::sort(points.begin(), points.end(), [this, point](const Point_T& fpoint, const Point_T& spoint) {
				return this->distance(point, fpoint) < this->distance(point, spoint);
				});

			std::vector<Point_T> closest_points;

			for (size_t i = 0; i < K; ++i)
			{
				closest_points.push_back(points[i]);
			}

			std::map<int, int> classes;

			for (size_t i = 0; i < closest_points.size(); ++i)
			{
				classes[closest_points[i].Cluster]++;
			}

			std::vector<std::pair<int, int>> vect(classes.begin(), classes.end());

			std::sort(vect.begin(), vect.end(), [this](const std::pair<int, int>& first, const std::pair<int, int>& second) {
				return first.second > second.second;
				});

			if (!vect.empty())
			{
				point.Cluster = vect[0].first;
			}
		}
	}
}