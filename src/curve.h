#ifndef _CURVE_H_
#define _CURVE_H_

#include <vector>
#include <fstream>
using namespace std;


class Curve
{
	vector<unsigned char> data;
public:
	Curve(vector<unsigned char> _data) : data(_data) {if(data.size() != 256) throw;}
	unsigned char operator() (int n) const {return data[n];}
	void rawwrite(ostream &); //for use in creating .AMP files 
};
ostream &operator<<(ostream &o, const Curve &c);



struct YUVCurves
{
	Curve y;
	Curve u;
	Curve v;
	//YUVCurves() {}
	YUVCurves(Curve _y, Curve _u, Curve _v)
		: y(_y), u(_u), v(_v) {}
};
ostream &operator <<(ostream &output, YUVCurves curves);

struct RGBCurves
{
	Curve r;
	Curve g;
	Curve b;
	//RGBCurves() {}
	RGBCurves(Curve _r, Curve _g, Curve _b)
		: r(_r), g(_g), b(_b) {}
};
ostream &operator <<(ostream &output, RGBCurves curves);

typedef pair< unsigned char, unsigned char> pairbyte;
class PairedCurve
{
	vector< vector<pairbyte> > data;
public:
	PairedCurve(vector< vector<pairbyte> > _data) : data(_data) {if(data.size() != 256) throw;}
	pairbyte operator() (int n, int m) const {return data[n][m];}
	void rawwrite(ostream &); //for use in creating .AMP files 
};
ostream &operator<<(ostream &o, const PairedCurve &c);

struct PolarYUVCurves
{
	Curve y;
	PairedCurve uv;
	//PolarYUVCurves() {}
	PolarYUVCurves(Curve _y, PairedCurve _uv)
		: y(_y), uv(_uv) {}
};
ostream &operator <<(ostream &output, PolarYUVCurves curves);

#endif