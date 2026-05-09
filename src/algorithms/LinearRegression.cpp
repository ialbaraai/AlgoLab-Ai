#include "../../include/algorithms/LinearRegression.h"

LinearRegression_T::LinearRegression_T()
{
	this->m_MBPair = { 0.0, 0.0 };
	this->m_Epochs = 5000;
	this->m_Alpha = 0.01;
}
LinearRegression_T::LinearRegression_T(double alpha = 0.01, int epochs = 5000)
{
	this->m_MBPair = { 0.0, 0.0 };
	this->m_Epochs = epochs;
	this->m_Alpha = alpha;
}

void LinearRegression_T::Simulate(const std::vector<Point_T>& points)
{
	int pointsNum = points.size();

	for (int epoch = 0; epoch < this->m_Epochs; epoch++)
	{
		double m_gradient = 0.0;
		double b_gradient = 0.0;

		for (const Point_T& point : points)
		{
			double predictedY = this->predict(point.Position.X);
			double predictionError = predictedY - point.Position.Y;

			m_gradient += (2.0 / pointsNum) * point.Position.X * predictionError;
			b_gradient += (2.0 / pointsNum) * predictionError;
		}

		this->m_MBPair.first -= this->m_Alpha * m_gradient;
		this->m_MBPair.second -= this->m_Alpha * b_gradient;
	}
}

double LinearRegression_T::predict(const double X) const
{
	return this->m_MBPair.first * X + this->m_MBPair.second;
}