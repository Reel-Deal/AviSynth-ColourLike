#include "stdafx.h"
#include "RGBapplycurves.h"

RGBApplyCurves::RGBApplyCurves(PClip _child, RGBCurves _curves, AffectRGB _affect, IScriptEnvironment *env)
: GenericVideoFilter(_child), curves(_curves), affect(_affect)
{
	if (!vi.IsRGB()) 
		env->ThrowError("ColourLike: RGBApplyCurves requires RGB32.");
}	

PVideoFrame __stdcall RGBApplyCurves::GetFrame(int n, IScriptEnvironment *env) 
{
	PVideoFrame frame = child->GetFrame(n, env);	
	env->MakeWritable(&frame);

	unsigned char *row = frame->GetWritePtr();
	const int width = frame->GetRowSize();
	const int height = frame->GetHeight();
	const int pitch = frame->GetPitch();
	if( affect.r && affect.g && affect.b )
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; x+=4)
			{
				row[x] = curves.r( row[x] );
				row[x+1] = curves.g( row[x+1] );
				row[x+2] = curves.b( row[x+2] );
			}
			row += pitch;
		}
	}
	else
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; x+=4)
			{
				if (affect.r)
					row[x] = curves.r( row[x] );
				if (affect.g)
					row[x+1] = curves.g( row[x+1] );
				if (affect.b)
					row[x+2] = curves.b( row[x+2] );
			}
			row += pitch;
		}
	}


	return frame;
}