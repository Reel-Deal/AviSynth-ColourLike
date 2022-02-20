#ifndef _EQUALISE_H_
#define _EQUALISE_H_

#include "curve.h"
#include "histogram.h"

Curve identity();

Curve equalisingcurve(Histogram from, Histogram to);
PairedCurve equalisingpairedcurve(PairedHistogram from, PairedHistogram to);

YUVCurves YUVequalisingcurves(YUVHistogram from, YUVHistogram to);
RGBCurves RGBequalisingcurves(RGBHistogram from, RGBHistogram to);
PolarYUVCurves YUVpolarequalisingcurves(PolarYUVHistogram from, PolarYUVHistogram to);

#endif