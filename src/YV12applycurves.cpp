#include "stdafx.h"
#include "YV12applycurves.h"

void applyplane(PVideoFrame &frame, int plane, Curve curve)
{
	unsigned char *row = frame->GetWritePtr(plane);
	const int width = frame->GetRowSize(plane);
	const int height = frame->GetHeight(plane);
	const int pitch = frame->GetPitch(plane);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
			row[x] = curve( row[x] );
		row += pitch;
	}
}

void applypairplane(PVideoFrame &frame, int planeA, int planeB, PairedCurve curve)
{
	const int width = frame->GetRowSize(planeA);
	const int height = frame->GetHeight(planeA);
	const int pitchA = frame->GetPitch(planeA);
	const int pitchB = frame->GetPitch(planeB);

	unsigned char *rowA = frame->GetWritePtr(planeA);
	unsigned char *rowB = frame->GetWritePtr(planeB);
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			pairbyte value = curve( rowA[x], rowB[x] );
			rowA[x] = value.first;
			rowB[x] = value.second;
		}
		rowA += pitchA;
		rowB += pitchB;
	}
}//*/


YV12ApplyCurves::YV12ApplyCurves(PClip _child, YUVCurves _curves, AffectYUV _affect,  IScriptEnvironment *env)
: GenericVideoFilter(_child), curves(_curves), affect(_affect)
{
	if (!vi.IsYV12()) 
		env->ThrowError("ColourLike: YV12ApplyCurves requires YV12.");
}	

PVideoFrame __stdcall YV12ApplyCurves::GetFrame(int n, IScriptEnvironment *env) 
{
	PVideoFrame frame = child->GetFrame(n, env);	
	env->MakeWritable(&frame);
	if(affect.y)
		applyplane(frame, PLANAR_Y, curves.y);
	if(affect.u)
		applyplane(frame, PLANAR_U, curves.u);
	if(affect.v)
		applyplane(frame, PLANAR_V, curves.v);
	return frame;
}


YV12ApplyPolarCurves::YV12ApplyPolarCurves(PClip _child, PolarYUVCurves _curves, AffectPolarYUV _affect,  IScriptEnvironment *env)
: GenericVideoFilter(_child), curves(_curves), affect(_affect)
{
	if (!vi.IsYV12()) 
		env->ThrowError("ColourLike: YV12ApplyCurves requires YV12.");
}	

PVideoFrame __stdcall YV12ApplyPolarCurves::GetFrame(int n, IScriptEnvironment *env) 
{
	PVideoFrame frame = child->GetFrame(n, env);	
	env->MakeWritable(&frame);
	if(affect.y)
		applyplane(frame, PLANAR_Y, curves.y);
	if(affect.uv)
		applypairplane(frame, PLANAR_U, PLANAR_V, curves.uv);
	return frame;
}



