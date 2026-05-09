#ifndef LINEAR_REGRESSION_H

#define LINEAR_REGRESSION_H

#include <vector>
#include "../utilities/point.h"

class LinearRegression_T
{
private:
	std::pair<double, double> m_MBPair;
	double m_Alpha;
public:
	int m_Epochs;
	LinearRegression_T();
	LinearRegression_T(double alpha, int epochs);
	void Simulate(const std::vector<Point_T>& points);
	double predict(const double X) const;
	const std::pair<double, double> mbpair() const { return this->m_MBPair; }
};

#endif