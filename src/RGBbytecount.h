#ifndef _RGBBYTECOUNT_H_
#define _RGBBYTECOUNT_H_

#include <vector>
//#include <fstream>
using namespace std;
#include "histogram.h"

class RGBByteCount
{
	vector<__int64/*double*/> Rcount;
	vector<__int64/*double*/> Gcount;
	vector<__int64/*double*/> Bcount;

//temporary buffer
	unsigned int *RGwordcount;//[256*256]; 
	unsigned int *BAwordcount;//[256*256];//currently not deleting these...
	int pixelscounted;
	void emptywords();
public:
	RGBByteCount();
	void add(PVideoFrame);
	RGBHistogram histogram() /*const*/;
};

#endif