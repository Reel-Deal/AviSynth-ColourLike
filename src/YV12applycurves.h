#ifndef _YV12_APPLY_CURVES_H_
#define _YV12_APPLY_CURVES_H_

#include "curve.h"

struct AffectYUV
{
	bool y;
	bool u;
	bool v;
	//AffectYUV() {}
	AffectYUV(bool _y, bool _u, bool _v)
		: y(_y), u(_u), v(_v) {}
};

//void applyplane(PVideoFrame &frame, int plane, Curve curve);

class YV12ApplyCurves : public GenericVideoFilter 
{   
	YUVCurves curves;
	AffectYUV affect;
public:
	YV12ApplyCurves(PClip _child, YUVCurves _curves, AffectYUV, IScriptEnvironment *env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment *env);
};

struct AffectPolarYUV
{
	bool y;
	bool uv;
	//AffectYUV() {}
	AffectPolarYUV(bool _y, bool _uv)
		: y(_y), uv(_uv) {}
};

class YV12ApplyPolarCurves : public GenericVideoFilter 
{   
	PolarYUVCurves curves;
	AffectPolarYUV affect;
public:
	YV12ApplyPolarCurves(PClip _child, PolarYUVCurves _curves, AffectPolarYUV, IScriptEnvironment *env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment *env);
};

#endif