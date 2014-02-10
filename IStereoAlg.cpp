/***************************************************************/
/* File: IStereoAlg.cpp                                        */
/* Usage: basic methods for stereo matching                    */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#include "IStereoAlg.h"

/***************************************************************/
/* Function: IStereoAlg                                        */
/* Usage: initial class member variables                       */
/* In:                                                         */
/*    const int mD - disparity levels                          */
/*    const int dS - disparity scale                           */
/* Out:                                                        */
/***************************************************************/
IStereoAlg::IStereoAlg(const int mD, const int dS)
{
	width = 0;        
	height = 0;
	imgSize = 0;
	lClr = NULL;		
	rClr = NULL;
	lDis = NULL;		
	rDis = NULL;
	maxDis = mD;
	disScale = dS;  
}
/***************************************************************/
/* Function: ~IStereoAlg                                       */
/* Usage: free class member variables                          */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
IStereoAlg::~IStereoAlg(void)
{
	if( lClr ) {
		cvReleaseImage(&lClr);
	}
	if( rClr ) {
		cvReleaseImage(&rClr);
	}
	if( lDis ) {
		cvReleaseImage(&lDis);
	}
	if( rDis ) {
		cvReleaseImage(&rDis);
	}
}

/***************************************************************/
/* Function: LoadImage                                         */
/* Usage: loading left&right images                            */
/* In:                                                         */
/*    const char *lFn - left image file name                   */
/*    const char *rFn - right image file name                  */
/* Out:                                                        */
/***************************************************************/
void IStereoAlg::LoadImg(const char *lFn, const char *rFn)
{
	lClr = cvLoadImage( lFn, CV_LOAD_IMAGE_COLOR );
	rClr = cvLoadImage( rFn, CV_LOAD_IMAGE_COLOR );
	
	width = lClr->width;	
	height = lClr->height;
	imgSize = width * height;
	
	// create depth map
	lDis = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );
	cvZero( lDis );
	rDis = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );
	cvZero( rDis );
}
/***************************************************************/
/* Function: SaveDep                                           */
/* Usage: save left&right depth maps                           */
/* In:                                                         */
/*    const char *lFn - left depth file name                   */
/*    const char *rFn - right depth file name                  */
/* Out:                                                        */
/***************************************************************/
void IStereoAlg::SaveDep(const char* lFn, const char* rFn)
{
	cvSaveImage(lFn,lDis);
	cvSaveImage(rFn,rDis);
}