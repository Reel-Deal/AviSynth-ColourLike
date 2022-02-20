#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include <vector>
#include <map>
#include <fstream>
using namespace std;

struct HistogramInputError
{
};

class Histogram
{
	vector<double> normalised;
public:
	Histogram();
	Histogram(vector<double>);
	double operator[](int n) const { return normalised[n]; }
};
ostream &operator <<(ostream &output, Histogram histogram);
istream &operator >>(istream &input, Histogram &histogram);

struct YUVHistogram
{
	Histogram y;
	Histogram u;
	Histogram v;
	YUVHistogram() {}
	YUVHistogram(Histogram _y, Histogram _u, Histogram _v)
		: y(_y), u(_u), v(_v) {}
};
ostream &operator <<(ostream &output, YUVHistogram histogram);
istream &operator >>(istream &input, YUVHistogram &histogram);

struct RGBHistogram
{
	Histogram r;
	Histogram g;
	Histogram b;
	RGBHistogram() {}
	RGBHistogram(Histogram _r, Histogram _g, Histogram _b)
		: r(_r), g(_g), b(_b) {}
};
ostream &operator <<(ostream &output, RGBHistogram histogram);
istream &operator >>(istream &input, RGBHistogram &histogram);

//________________________________

class PairedHistogram
{
	vector<vector<double> > normalised;
public:
	PairedHistogram();
	PairedHistogram(vector<vector<double> >);
	double operator()(int n, int m) const { return normalised[n][m]; }
	map<double, double> polarlengths() const;
};
ostream &operator <<(ostream &output, PairedHistogram histogram);
istream &operator >>(istream &input, PairedHistogram &histogram);

struct PolarYUVHistogram
{
	Histogram y;
	PairedHistogram uv;
	PolarYUVHistogram() {}
	PolarYUVHistogram(Histogram _y, PairedHistogram _uv)
		: y(_y), uv(_uv) {}
};
ostream &operator <<(ostream &output, PolarYUVHistogram histogram);
istream &operator >>(istream &input, PolarYUVHistogram &histogram);
#endif
