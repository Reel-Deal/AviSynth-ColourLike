#include "stdafx.h"
#include "equalise.h"

#include <vector>
#include <map>
#include <cmath>
using namespace std;



Curve identity()
{
	vector<unsigned char> result(256);
	for(int n = 0; n < 256; ++n)
		result[n] = n;
	return Curve(result);
}

vector<double> cumulative(Histogram histogram)//result[i] is the number of values STRICTLY less than i
{
	vector<double> result(257);
	result[0] = 0.0;
	for(int i = 1; i <= 256; ++i)
		result[i] = result[i-1] + histogram[i-1];
	return result;
}

Curve equalisingcurve(Histogram from, Histogram to)
{
	vector<double> cumulativefrom = cumulative(from);
	vector<double> cumulativeto = cumulative(to);
	vector<unsigned char> result(256);
	for(int n = 0; n < 256; ++n)
	{
		double fraction = (cumulativefrom[n] + cumulativefrom[n+1])/2.0;
		unsigned char f = 0;
		while(f < 255 && cumulativeto[f] < fraction)
			++f;
		result[n] = f;
	}
	return Curve(result);
}

map<double, double> cumulative(map<double, double> m)//result[f] is the number of values STRICTLY less than f
{
	map<double, double> result;
	double sum = 0.0;

	for(map<double, double>::const_iterator i = m.begin(); i != m.end(); ++i)
	{
		result[i->first] = sum;
		sum += i->second;
	}
	result[100.0] = 1.0;//the cumulative 1.0 has to appear after all the normal entries as a sentinel
	return result;
}

map<double, double> inverse(map<double, double> m)//result[f] is the first value STRICTLY greater than f
{
	map<double, double> result;

	for(map<double, double>::const_iterator i = m.begin(); i != m.end(); ++i)
		result[i->second] = i->first;

	return result;
}

PairedCurve equalisingpairedcurve(PairedHistogram from, PairedHistogram to)
{
	map<double, double> cumulativefrom = cumulative(from.polarlengths());
	map<double, double> cumulativeto = cumulative(to.polarlengths());
	map<double, double> inversecumulativeto = inverse(cumulativeto);

	ofstream output("cumulative.txt");
	//for(map<double, double>::const_iterator i = inversecumulativeto.begin(); i != inversecumulativeto.end(); ++i)
	//	output  <<  i->first << "\t" << i->second << "\n";




	vector< vector<pairbyte> > result(256);//vector<unsigned char>
	for(int i = 0; i < 256; ++i)
		result[i].resize(256);
  	for(int i = -128; i < 128; ++i)
		for(int j = -128; j < 128; ++j)
	{
	//	result[i][j]= make_pair(i, j);
		double length = sqrt(static_cast<double>(i*i+j*j));
		map<double, double>::iterator end = cumulativefrom.upper_bound(length);
	/*	map<double, double>::iterator start = end-1;
		double fraction = (start->second + end->second)/2.0;
		if(fraction==1.0)
			throw;//it's the midpoint of a range,*/
		double fraction = end->second;
		double newlength = inversecumulativeto.upper_bound(fraction)->second;
		double scale = newlength/length;
		int newi = static_cast<int>(i * scale + 0.5);
		int newj = static_cast<int>(j * scale + 0.5);
		if(newi < -128) newi = -128; 
		if(newj < -128) newj = -128; 
		if(newi > 127) newi = 127; 
		if(newj > 127) newj = 127; 
		unsigned char newfirst		= static_cast<unsigned char>( newi+128 );
		unsigned char newsecond	= static_cast<unsigned char>( newj+128 );
		result[i+128][j+128] = make_pair(newfirst, newsecond);
	/*	output  <<  i << "\t" << j << "\t";
		output  <<  newfirst << "\t" << newsecond << "\n";*/
		

	//	double fraction = (cumulativefrom[n] + cumulativefrom[n+1])/2.0;
	//	unsigned char f = 0;
	//	while(f < 255 && cumulativeto[f] < fraction)
	//		++f;
	//	result[n] = f;
	//}
	}
	return PairedCurve(result);
}//*/;/*

YUVCurves YUVequalisingcurves(YUVHistogram from, YUVHistogram to)
{
	return YUVCurves(
		equalisingcurve(from.y, to.y),
		equalisingcurve(from.u, to.u),
		equalisingcurve(from.v, to.v));
}

RGBCurves RGBequalisingcurves(RGBHistogram from, RGBHistogram to)
{
	return RGBCurves(
		equalisingcurve(from.r, to.r),
		equalisingcurve(from.g, to.g),
		equalisingcurve(from.b, to.b));
}

PolarYUVCurves YUVpolarequalisingcurves(PolarYUVHistogram from, PolarYUVHistogram to)
{
	return PolarYUVCurves(
		equalisingcurve(from.y, to.y),
		equalisingpairedcurve(from.uv, to.uv));
}
