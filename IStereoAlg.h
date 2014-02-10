/***************************************************************/
/* File: IStereoAlg.h                                          */
/* Usage: header file for stereo matching interface            */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#pragma once
#include<opencv/cv.h>
#include<opencv/highgui.h>

class IStereoAlg
{
public:

	int width;             // image size
	int height;
	int imgSize;

	IplImage* lClr;		   // left&right images
	IplImage* rClr;

	IplImage* lDis;		   // output depth maps
	IplImage* rDis;

	// 公共参数
	int      maxDis;    //  dataset    levels  scale
	int      disScale;  //  Tsukuba 	0..15 	16 	
						//  Venus 	    0..19 	8 	
						//  Teddy 	    0..59 	4 	
						//  Cones 	    0..59 	4
public:
	IStereoAlg(const int mD, const int dS);
	~IStereoAlg(void);
public:
	void LoadImg( const char* lFn, const char* rFn );			
	void SaveDep( const char* lFn, const char* rFn );	

	virtual void PreProcess()=0;		
	virtual void Match()=0;		
	virtual void PostProcess()=0;
};
