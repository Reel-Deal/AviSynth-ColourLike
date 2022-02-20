#ifndef _RGB_APPLY_CURVES_H_
#define _RGB_APPLY_CURVES_H_

#include "curve.h"
#include "curve.h"

struct AffectRGB
{
	bool r;
	bool g;
	bool b;
	//AffectRGB() {}
	AffectRGB(bool _r, bool _g, bool _b)
		: r(_r), g(_g), b(_b) {}
};

class RGBApplyCurves : public GenericVideoFilter 
{   
	RGBCurves curves;
	AffectRGB affect;
public:
	RGBApplyCurves(PClip _child, RGBCurves _curves, AffectRGB, IScriptEnvironment *env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment *env);
};

#endif