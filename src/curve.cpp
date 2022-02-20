#include "stdafx.h"
#include "curve.h"

ostream &operator<<(ostream &o, const Curve &c)
{
	for(int y = 0; y < 256; ++y) 
		o << y << ": "<< int(c(y)) <<  "\n";
	o <<  "\n";
	return o;
}

ostream &operator <<(ostream &output, YUVCurves curves)
{
	output << "Y\t" << curves.y;
	output << "U\t" << curves.u;
	output << "V\t" << curves.v;
	return output;
}

ostream &operator <<(ostream &output, RGBCurves curves)
{
	output << "R\t" << curves.r;
	output << "G\t" << curves.g;
	output << "B\t" << curves.b;
	return output;
}


