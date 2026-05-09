#ifndef KMCAPP_H

#define KMCAPP_H

#include "../algorithms/KMC.h"
#include "../utilities/selecttype.h"

class KMCApp
{
private:
	KMeansClustering m_KMC;
public:
	KMCApp();
	void run();
};

#endif