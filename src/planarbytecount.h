#ifndef _PLANARBYTECOUNT_H_
#define _PLANARBYTECOUNT_H_

#include <vector>
//#include <fstream>
using namespace std;
#include "histogram.h"

class PlanarByteCount
{
	vector<__int64/*double*/> count;

	unsigned int wordcount[256*256]; //temporary buffer
	int wordscounted;
	void emptywords();
public:
	PlanarByteCount();
	void add(PVideoFrame, int plane);//requires mod 4 width
	Histogram histogram() /*const*/;
};

#endif