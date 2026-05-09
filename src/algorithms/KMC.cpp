#include "../../include/algorithms/KMC.h"

#include <iostream>

KMeansClustering::KMeansClustering() {}

double KMeansClustering::distance(const Point_T& point, const Centroid_T& centroid)
{
	return sqrt(pow(point.Position.X - centroid.Position.X, 2) + pow(point.Position.Y - centroid.Position.Y, 2));
}

void KMeansClustering::AssignPointsToClustersImpl(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids)
{
	if (centroids.size() > 0)
	{
		for (size_t i = 0; i < points.size(); ++i)
		{
			int cluster = -1;
			double clusterdst = 0.0;

			for (size_t j = 0; j < centroids.size(); ++j)
			{
				double dst = this->distance(points[i], centroids[j]);

				if (j == 0)
				{
					cluster = j;
					clusterdst = dst;
				}
				else
				{
					if (dst < clusterdst)
					{
						cluster = j;
						clusterdst = dst;
					}
				}
			}

			points[i].Cluster = cluster;
			points[i].Color = { centroids[cluster].Color.Red, centroids[cluster].Color.Green, centroids[cluster].Color.Blue, centroids[cluster].Color.Alpha };
		}
	}
}
void KMeansClustering::AssignClusterCentroidsImpl(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids)
{
	for (size_t i = 0; i < centroids.size(); ++i)
	{
		double totalInertia = 0.0;
		double totalX = 0.0;
		double totalY = 0.0;
		int numOfPoints = 0;

		for (size_t j = 0; j < points.size(); ++j)
		{
			if (points[j].Cluster == i)
			{
				totalInertia += pow(this->distance(points[j], centroids[i]), 2);
				totalX += points[j].Position.X;
				totalY += points[j].Position.Y;
				numOfPoints++;
			}
		}

		if (numOfPoints > 0)
		{
			centroids[i].Inertia = totalInertia;
			centroids[i].Position.X = totalX / numOfPoints;
			centroids[i].Position.Y = totalY / numOfPoints;
			centroids[i].rerect();
		}
	}
}

void KMeansClustering::Simulate(std::vector<Point_T>& points, std::vector<Centroid_T>& centroids)
{
	this->AssignPointsToClustersImpl(points, centroids);
	this->AssignClusterCentroidsImpl(points, centroids);
}