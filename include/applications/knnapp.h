#ifndef KNNAPP_H

#define KNNAPP_H

#include "../algorithms/KNN.h"
#include "../utilities/selecttype.h"

class KNNApp
{
private:
	KNearestNeighbor m_KNN;
public:
	KNNApp();
	void run();
};

#endif