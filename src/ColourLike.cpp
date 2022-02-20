/*
	ColourLike plugin for AviSynth

	Copyright (C) 2006 Mohan
  
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation.
	 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
		
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	The author can be contacted at the doom9.org forum under mg262.
*/

#include "stdafx.h"
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

#include "curve.h"
#include "histogram.h"
#include "planarbytecount.h"
#include "pairedplanarbytecount.h"
#include "YV12applycurves.h"
#include "RGBbytecount.h"
#include "RGBapplycurves.h"
#include "equalise.h"

PairedCurve saturationgammacurves(double gamma, double fixed)
{
	vector< vector<pairbyte> > result(256);//vector<unsigned char>
	for(int i = 0; i < 256; ++i)
		result[i].resize(256);
 	for(int i = -128; i < 128; ++i)
		for(int j = -128; j < 128; ++j)
 	{
		double length = sqrt(static_cast<double>(i*i+j*j))/fixed;

		double newlength = pow(length, gamma);
		double scale = newlength/length;
		//scale = 2.0;
		int newi = static_cast<int>(i * scale + 0.5);
		int newj = static_cast<int>(j * scale + 0.5);
		if(newi < -128) newi = -128; 
		if(newj < -128) newj = -128; 
		if(newi > 127) newi = 127; 
		if(newj > 127) newj = 127; 
		unsigned char newfirst		= static_cast<unsigned char>( newi+128 );
		unsigned char newsecond	= static_cast<unsigned char>( newj+128 );
		result[i+128][j+128] = make_pair(newfirst, newsecond);
	}
	return PairedCurve(result);
}

AVSValue __cdecl WriteHistogram(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	ofstream output(args[1].AsString());
	int every(args[2].AsInt(1));
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		PlanarByteCount ycount;
		PlanarByteCount ucount;
		PlanarByteCount vcount;
		for(int i = 0; i < videoinfo.num_frames; i += every)
		{
			PVideoFrame frame = clip->GetFrame(i, env);
			ycount.add( frame, PLANAR_Y );
			ucount.add( frame, PLANAR_U );
			vcount.add( frame, PLANAR_V );
		}
		YUVHistogram histogram( ycount.histogram(), ucount.histogram(), vcount.histogram() );
		output << histogram;
	}	
	else if(videoinfo.IsRGB32())
	{
		RGBByteCount count;
		for(int i = 0; i < videoinfo.num_frames; i += every)
		{
			PVideoFrame frame = clip->GetFrame(i, env);
			count.add( frame );
		}
		RGBHistogram histogram = count.histogram();
		output << histogram;
	}
	else
		env->ThrowError("WriteHistogram requires YV12 or RGB32.");

	return clip;  
}


AVSValue __cdecl WritePolarHistogram(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	ofstream output(args[1].AsString());
	int every(args[2].AsInt(1));
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		PlanarByteCount ycount;
		PairedPlanarByteCount uvcount;
		PlanarByteCount vcount;
		for(int i = 0; i < videoinfo.num_frames; i += every)
		{
			PVideoFrame frame = clip->GetFrame(i, env);
			ycount.add( frame, PLANAR_Y );
			uvcount.add( frame, PLANAR_U, PLANAR_V );
		}
		PolarYUVHistogram histogram( ycount.histogram(), uvcount.histogram() );
		output << histogram;
	}	
	else
		env->ThrowError("ColourLike requires RGB32.");

	return clip;  
}
//
//
//AVSValue __cdecl CopyHistogram(AVSValue args, void *user_data, IScriptEnvironment *env) 
//{
//	ifstream input(args[0].AsString());++i
//	ofstream output(args[1].AsString());
//	YUVHistogram histogram;
//	input >> histogram;		
//	output << histogram;
//	return 0;
//}

AVSValue __cdecl CopyPolarHistogram(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	ifstream input(args[0].AsString());
	ofstream output(args[1].AsString());
	PolarYUVHistogram histogram;
	input >> histogram;		
	output << histogram;
	return 0;
}

AVSValue __cdecl WriteColourLikeCurve(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	ofstream output(args[0].AsString());
	ifstream fromfile(args[1].AsString());
	ifstream tofile(args[2].AsString());
	YUVHistogram from;
	YUVHistogram to;
	fromfile >> from;		
	tofile >> to;
	output << YUVequalisingcurves(from, to);
	return 0;
}

AVSValue __cdecl ColourLike(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	ifstream fromfile(args[1].AsString());
	ifstream tofile(args[2].AsString());
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		YUVHistogram from;
		YUVHistogram to;
		fromfile >> from;		
		tofile >> to;
		YUVCurves curves(YUVequalisingcurves(from, to));
		AffectYUV affect(args[3].AsBool(true), args[4].AsBool(true), args[5].AsBool(true));
		return new YV12ApplyCurves(clip, curves, affect, env);
	}
	if(videoinfo.IsRGB32())
	{
		RGBHistogram from;
		RGBHistogram to;
		fromfile >> from;		
		tofile >> to;
		RGBCurves curves(RGBequalisingcurves(from, to));
		AffectRGB affect(args[3].AsBool(true), args[4].AsBool(true), args[5].AsBool(true));
		return new RGBApplyCurves(clip, curves, affect, env);
	}
	//unsupported colour space...
	env->ThrowError("ColourLike requires YV12 or RGB32.");
	throw;//prevents "not all control paths return a value"
}

AVSValue __cdecl PolarColourLike(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	ifstream fromfile(args[1].AsString());
	ifstream tofile(args[2].AsString());
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		PolarYUVHistogram from;
		PolarYUVHistogram to;
		fromfile >> from;		
		tofile >> to;
		PolarYUVCurves curves(YUVpolarequalisingcurves(from, to));
		AffectPolarYUV affect(args[3].AsBool(true), args[4].AsBool(true));
		return new YV12ApplyPolarCurves(clip, curves, affect, env);
	}
	//unsupported colour space...
	env->ThrowError("PolarColourLike requires YV12 or RGB32.");
	throw;//prevents "not all control paths return a value"
}

AVSValue __cdecl ApplyAMP(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	ifstream file(args[1].AsString(), ios::binary);
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		vector<unsigned char> curvedata(0);
		curvedata.reserve(256);
		for(int n = 0; n < 256; ++n)
			curvedata.push_back(file.get());
		Curve curve(curvedata);
		AffectYUV affect(true, false, false);
		return new YV12ApplyCurves(clip, YUVCurves(curve, curve, curve), affect, env);
	}
	if(videoinfo.IsRGB32())
	{
		vector<unsigned char> curvedata(0);
		curvedata.reserve(256);
		for(int n = 0; n < 256; ++n)
			curvedata.push_back(file.get());
		Curve curve(curvedata);
		AffectRGB affect(true, true, true);
		return new RGBApplyCurves(clip, RGBCurves(curve, curve, curve), affect, env);
	}
	//unsupported colour space...
	env->ThrowError("ApplyAMP requires YV12 or RGB32.");
	throw;//prevents "not all control paths return a value"
}


AVSValue __cdecl SaturationGamma(AVSValue args, void *user_data, IScriptEnvironment *env) 
{
	PClip clip = args[0].AsClip();
	double gamma = args[1].AsFloat();
	double fixed = args[2].AsFloat(127.0);
	VideoInfo videoinfo = clip->GetVideoInfo();
	if(videoinfo.IsYV12())
	{
		PolarYUVCurves curves(identity(), saturationgammacurves(gamma, fixed));
		AffectPolarYUV affect(false, true);//gamma only
		return new YV12ApplyPolarCurves(clip, curves, affect, env);
	}
	//unsupported colour space...
	env->ThrowError("SaturationGamma requires YV12.");
	throw;//prevents "not all control paths return a value"
}

//Add dll to AviSynth Scripting Environment
const AVS_Linkage* AVS_linkage;

extern "C" __declspec(dllexport)
const char* __stdcall AvisynthPluginInit3(IScriptEnvironment * env, const AVS_Linkage* const vectors)
{
    AVS_linkage = vectors;
	env->AddFunction("WriteHistogram", "cs[every]i", WriteHistogram, 0);
	env->AddFunction("WritePolarHistogram", "cs[every]i", WritePolarHistogram, 0);
	//env->AddFunction("CopyHistogram", "ss", CopyHistogram, 0);
	 env->AddFunction("CopyPolarHistogram", "ss", CopyPolarHistogram, 0);
	env->AddFunction("WriteColourLikeCurve", "sss", WriteColourLikeCurve, 0);
	env->AddFunction("ColourLike", "css[affectfirst]b[affectsecond]b[affectthird]b", ColourLike, 0);
	env->AddFunction("PolarColourLike", "css[affectfirst]b[affectsecond]b[affectthird]b", PolarColourLike, 0);
	env->AddFunction("ApplyAMP", "cs", ApplyAMP, 0);
	env->AddFunction("SaturationGamma", "cf[fixed]f", SaturationGamma, 0);
	return "ColourLike plugin";
}
