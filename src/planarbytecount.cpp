#include "stdafx.h"
#include "planarbytecount.h"


PlanarByteCount::PlanarByteCount()
	: count(256, 0), wordscounted(0)
{
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			wordcount[i + j*256] = 0;
}

void PlanarByteCount::emptywords()
{
	if( wordscounted == 0 )
		return;
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
		{
			count[i] += wordcount[i +  j*256];
			count[j] += wordcount[i +  j*256];
			wordcount[i + j*256] = 0;
		}
	wordscounted = 0;
}

void PlanarByteCount::add(PVideoFrame frame, int plane)
{	
	const int width = frame->GetRowSize(plane);
	const int height = frame->GetHeight(plane);
	const int pitch = frame->GetPitch(plane);

	if(width % 2 != 0) 
		throw;
	int widthinwords = width/2;
	if(wordscounted + widthinwords*height > 1000000000)
		emptywords();

	const unsigned char *row = frame->GetReadPtr(plane);
	for(int y = 0; y < height; ++y)
	{
		const unsigned short *wordrow = reinterpret_cast<const unsigned short *>(row);
		for(int x = 0; x < widthinwords; ++x)
			++wordcount[  wordrow[x]  ];
		row += pitch;
	}
	wordscounted += widthinwords*height;
}

Histogram PlanarByteCount::histogram() /*const*/
{	
	vector<double> countdouble(256);
	emptywords();
	for(int i = 0; i < 256; ++i)
	{
		__int64/*double*/ value = count[i];
		countdouble[i] = static_cast<double>(value);
	}
	return Histogram(countdouble);
}
