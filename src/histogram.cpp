#include "stdafx.h"
#include "histogram.h"
#include <cmath>
using namespace std;


Histogram::Histogram()	
	: normalised(256, 0.0)
{
}

Histogram::Histogram(vector<double> v)	
	: normalised(256)
{
	double total = 0.0;
	for(int i = 0; i < 256; ++i)
		total += v[i];
	for(int i = 0; i < 256; ++i)
		normalised[i] = v[i]/total;
}

ostream &operator <<(ostream &output, Histogram histogram)
{
	for(int i = 0; i < 256; ++i)
		output << histogram[i] << "\t";
	output << "\n";
	return output;
}

istream &operator >>(istream &input, Histogram &histogram)
{
	vector<double> v(256);	
	input >> ws;
	for(int i = 0; i < 256; ++i)
	{
		if(input.eof())
			throw HistogramInputError();
		input >> v[i] >> ws;
	}
	histogram = Histogram(v);
	return input;
}

ostream &operator <<(ostream &output, YUVHistogram histogram)
{
	//output <<histogram.y;
	output << "Y\t" << histogram.y;
	output << "U\t" << histogram.u;
	output << "V\t" << histogram.v;
	return output;
}

istream &operator >>(istream &input, YUVHistogram &histogram)
{
	char c = '\0';
	input >> ws >> c >> ws  >> histogram.y;
	if(input.eof() || c != 'Y')
		throw HistogramInputError();
	input >> ws >> c >> ws  >> histogram.u;
	if(input.eof() || c != 'U')
		throw HistogramInputError();
	input >> ws >> c >> ws  >> histogram.v;
	if(/*input.eof() ||*/ c != 'V')
		throw HistogramInputError();
	return input;
}

ostream &operator <<(ostream &output, RGBHistogram histogram)
{
	//output <<histogram.r;
	output << "R\t" << histogram.r;
	output << "G\t" << histogram.g;
	output << "B\t" << histogram.b;
	return output;
}

istream &operator >>(istream &input, RGBHistogram &histogram)
{
	char c = '\0';
	input >> ws >> c >> ws  >> histogram.r;
	if(input.eof() || c != 'R')
		throw HistogramInputError();
	input >> ws >> c >> ws  >> histogram.g;
	if(input.eof() || c != 'G')
		throw HistogramInputError();
	input >> ws >> c >> ws  >> histogram.b;
	if(/*input.eof() ||*/ c != 'B')
		throw HistogramInputError();
	return input;
}



PairedHistogram::PairedHistogram()	
	: normalised(256)
{
	for(int i = 0; i < 256; ++i)
		normalised[i].resize(256, 0.0);
}

PairedHistogram::PairedHistogram(vector<vector<double> > v)
	: normalised(256)
{
	for(int i = 0; i < 256; ++i)
		normalised[i].resize(256);
	double total = 0.0;
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			total += v[i][j];
	for(int i = 0; i < 256; ++i)
		for(int j = 0; j < 256; ++j)
			normalised[i][j] = v[i][j]/total;
}

map<double, double> PairedHistogram::polarlengths() const
{
	map<double, double> result;
 	for(int i = -128; i < 128; ++i)
		for(int j = -128; j < 128; ++j)
			result[ sqrt(static_cast<double>(i*i+j*j)) ] += normalised[i+128][j+128];
	return result;
}

ostream &operator <<(ostream &output, PairedHistogram histogram)
{
	for(int i = 0; i < 256; ++i)
	{
		for(int j = 0; j < 256; ++j)
			output << histogram(i, j) << "\t";
		output << "\n";
	}
	return output;
}

istream &operator >>(istream &input, PairedHistogram &histogram)
{
	vector< vector<double> > v(256);
	for(int i = 0; i < 256; ++i)
		v[i].resize(256);

	input >> ws;
	for(int i = 0; i < 256; ++i)
	{
		for(int j = 0; j < 256; ++j)
		{
			if(  input.eof()  )
				throw HistogramInputError();
			input >> v[i][j] >> ws;
		}
		input >> ws;
	}
	histogram = PairedHistogram(v);
	return input;
}


ostream &operator <<(ostream &output, PolarYUVHistogram histogram)
{
	//output <<histogram.y;
	output << "Y\t" << histogram.y;
	output << "UV\n" << histogram.uv;
	return output;
}

istream &operator >>(istream &input, PolarYUVHistogram &histogram)
{
	char c = '\0';
	char c1 = '\0';
	input >> ws >> c >> ws  >> histogram.y;
	if(input.eof() || c != 'Y')
		throw HistogramInputError();
	input >> ws >> c >> c1 >> ws  >> histogram.uv;
	if(c != 'U' || c1 != 'V' )
		throw HistogramInputError();
	return input;
}