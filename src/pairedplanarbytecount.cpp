#include "stdafx.h"
#include "pairedplanarbytecount.h"


PairedPlanarByteCount::PairedPlanarByteCount()
	: paircount(256), pairscounted(0)
{
	for(int i = 0; i < 256; ++i)
		paircount[i].resize(256);
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			paircountbuffer[i + j*256] = 0;
}

void PairedPlanarByteCount::emptybuffer()
{
	if( pairscounted == 0 )
		return;
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
		{
			paircount[i][j] += paircountbuffer[i +  j*256];
			paircountbuffer[i + j*256] = 0;
		}
	pairscounted = 0;
}

void PairedPlanarByteCount::add(PVideoFrame frame, int planeA, int planeB)
{	
	const int width = frame->GetRowSize(planeA);
	const int height = frame->GetHeight(planeA);
	const int pitchA = frame->GetPitch(planeA);
	const int pitchB = frame->GetPitch(planeB);

	if(pairscounted + width*height > 1000000000)
		emptybuffer();

	const unsigned char *rowA = frame->GetReadPtr(planeA);
	const unsigned char *rowB = frame->GetReadPtr(planeB);
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
			++paircountbuffer[  rowA[x] +  rowB[x]*256 ];
		rowA += pitchA;
		rowB += pitchB;
	}
	pairscounted += width*height;
}

PairedHistogram PairedPlanarByteCount::histogram() /*const*/
{	
	vector< vector<double> > paircountdouble(256);
	for(int i = 0; i < 256; ++i)
		paircountdouble[i].resize(256);
	emptybuffer();
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
		{
				__int64/*double*/ value = paircount[i][j];
			paircountdouble[i][j] = static_cast<double>(value);
		}
	return PairedHistogram(paircountdouble);
}
