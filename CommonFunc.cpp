/***************************************************************/
/* File: CommonFunc.cpp                                        */
/* Usage: implement common functions                           */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#include <math.h>
#include <stdlib.h>
#include "BinaryStereo.h"
#include "CommonFunc.h"
#include<opencv/cv.h>

double F(double input)
{
	if(input>0.008856)
		return (pow(input, 0.333333333));
	else
		return (7.787*input+0.137931034);
}


// RGB -> XYZ
void RGBtoXYZ(double R, double G, double B, double &X, double &Y, double &Z)
{
	X=0.412453*R+0.357580*G+0.189423*B;
	Y=0.212671*R+0.715160*G+0.072169*B;
	Z=0.019334*R+0.119193*G+0.950227*B;
}


// XYZ -> CIELab
void XYZtoLab(double X, double Y, double Z, double &L, double &a, double &b)
{
	const double Xo=244.66128;
	const double Yo=255.0;
	const double Zo=277.63227;
	L=116*F(Y/Yo)-16;
	a=500*(F(X/Xo)-F(Y/Yo));
	b=200*(F(Y/Yo)-F(Z/Zo));
}

// RGB -> CIELab
void RGBtoLab(double R, double G, double B, double &L, double &a, double &b)
{
	double X, Y, Z;
	RGBtoXYZ(R, G, B, X, Y, Z);
	XYZtoLab(X, Y, Z, L, a, b);
}

// generate gaussian distribution
CvRNG rngSeed = cvRNG( 23 ); // 23
double RandNormal( const double std, const double mu )
{
	double x1, x2, w, y1;
	static double y2;
	static bool use_last = false;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = false;
	}
	else
	{
		do {
			//double t1 = ( double )rand( ) / RAND_MAX;
			//double t2 = ( double )rand( ) / RAND_MAX;
			x1 = 2.0 * cvRandReal( &rngSeed ) - 1.0;
			x2 = 2.0 * cvRandReal( &rngSeed ) - 1.0;
			//x1 = 2.0 * t1 - 1.0;
			//x2 = 2.0 * t2 - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = true;
	}

	return mu + y1 * std;
}


// get 1 / 4 value
int MyGetCutVal(int* wgtCnt)
{
	int perLen = DESC_LEN / 4;
	int count = 0;
	for( int i = 0; i < WGT_CNT; i ++ ) {
		count += wgtCnt[ i ];
		if( count > perLen ) {
			//if( i > CLR_WGT_THRES ) {
			//	return i;
			//} else {
			//	return CLR_WGT_THRES;
			//}
			return i;
		}
	}
	// return CLR_WGT_THRES;
}
// adaptive threshold for canny edge detecotr
void AdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high)
{
	CvSize size;
	IplImage *imge=0;
	int i,j;
	CvHistogram *hist;
	int hist_size = 255;
	float range_0[]={0,256};
	float* ranges[] = { range_0 };
	double  PercentOfPixelsNotEdges = 0.7;
	size = cvGetSize(dx);
	imge = cvCreateImage(size, IPL_DEPTH_32F, 1);
	float maxv = 0;
	for(i = 0; i < size.height; i++ )
	{
		const short* _dx = (short*)(dx->data.ptr + dx->step*i);
		const short* _dy = (short*)(dy->data.ptr + dy->step*i);
		float* _image = (float *)(imge->imageData + imge->widthStep*i);
		for(j = 0; j < size.width; j++)
		{
			_image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));
			maxv = maxv < _image[j] ? _image[j]: maxv;
		}
	}

	range_0[1] = maxv;
	hist_size = (int)(hist_size > maxv ? maxv:hist_size);
	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist( &imge, hist, 0, NULL );
	int total = (int)(size.height * size.width * PercentOfPixelsNotEdges);
	float sum=0;
	int icount = hist->mat.dim[0].size;

	float *h = (float*)cvPtr1D( hist->bins, 0 );
	for(i = 0; i < icount; i++)
	{
		sum += h[i];
		if( sum > total )
			break; 
	}

	*high = (i+1) * maxv / hist_size ;
	*low = *high * 0.4;
	cvReleaseImage( &imge );
	cvReleaseHist(&hist);
}