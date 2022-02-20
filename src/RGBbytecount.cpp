#include "stdafx.h"
#include "RGBbytecount.h"


RGBByteCount::RGBByteCount()
	: Rcount(256, 0), Gcount(256, 0), Bcount(256, 0), pixelscounted(0)
{
	RGwordcount = new unsigned int [256*256]; 
	BAwordcount = new unsigned int [256*256];
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			RGwordcount[i + j*256] = BAwordcount[i + j*256] = 0;
}

void RGBByteCount::emptywords()
{
	if( pixelscounted == 0 )
		return;
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
		{
			//working in a little-endian architecture, so R is a lower byte, G a higher byte
			Rcount[i] += RGwordcount[i +  j*256];//
			Gcount[j] += RGwordcount[i +  j*256];
			Bcount[i] += BAwordcount[i +  j*256];
			RGwordcount[i + j*256] = 0; 
			BAwordcount[i + j*256] = 0;
		}
	pixelscounted = 0;
}

void RGBByteCount::add(PVideoFrame frame)
{	
	const int width = frame->GetRowSize();//in bytes
	const int height = frame->GetHeight();
	const int pitch = frame->GetPitch();

	int widthinwords = width/2;
	int widthinpixels = width/4;
	if(pixelscounted + widthinpixels*height > 1000000000)
		emptywords();

	const unsigned char *row = frame->GetReadPtr();
	for(int y = 0; y < height; ++y)
	{
		const unsigned short *wordrow = reinterpret_cast<const unsigned short *>(row);
		for(int x = 0; x < widthinwords; x+=2)
		{
			++RGwordcount[  wordrow[x]  ];
			++BAwordcount[  wordrow[x+1]  ];
		}
		row += pitch;
	}
	pixelscounted += widthinpixels*height;
}

RGBHistogram RGBByteCount::histogram() /*const*/
{	
	vector<double> Rcountdouble(256);
	vector<double> Gcountdouble(256);
	vector<double> Bcountdouble(256);
	emptywords();
	for(int i = 0; i < 256; ++i)
	{
		Rcountdouble[i] = static_cast<double>(Rcount[i]);
		Gcountdouble[i] = static_cast<double>(Gcount[i]);
		Bcountdouble[i] = static_cast<double>(Bcount[i]);
	}
	return RGBHistogram(  Histogram(Rcountdouble), Histogram(Gcountdouble), Histogram(Bcountdouble)  );
}
