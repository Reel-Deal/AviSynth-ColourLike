#ifndef _PAIREDPLANARBYTECOUNT_H_
#define _PAIREDPLANARBYTECOUNT_H_

#include <vector>
//#include <fstream>
using namespace std;
#include "histogram.h"

class PairedPlanarByteCount
{
	vector<vector<__int64/*double*/> > paircount;

	unsigned int paircountbuffer[256*256]; //temporary buffer
	int pairscounted;
	void emptybuffer();
public:
	PairedPlanarByteCount();
	void add(PVideoFrame, int planeA, int planeB);//requires mod 4 width
	PairedHistogram histogram() /*const*/;
};

#endif