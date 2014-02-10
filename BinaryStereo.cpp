/***************************************************************/
/* File: BinaryStereo.cpp                                      */
/* Usage: implement BinaryStereo Class                         */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#include "BinaryStereo.h"
#include "CommonFunc.h"

/***************************************************************/
/* Function: BinaryStereo                                      */
/* Usage: initial class member variables                       */
/* In:                                                         */
/*    const int pS - patch size                                */
/*    const int mD - disparity levels                          */
/*    const int dS - disparity scale                           */
/* Out:                                                        */
/***************************************************************/
BinaryStereo::BinaryStereo(const int pS, const int mD, const int dS)
: patchSz(pS), IStereoAlg(mD,dS)
{
	descLen = DESC_LEN;
	pairDistr = new int[4*DESC_LEN];
	gStd = 4.0f; 
	lCode = NULL;
	rCode = NULL;
	lUsed = NULL;
	rUsed = NULL;
	l_r = NULL;
	l_g = NULL;
	l_b = NULL;
	r_r = NULL;
	r_g = NULL;
	r_b = NULL;
	lL = NULL;
	lA = NULL;
	lB = NULL;
	rL = NULL;
	rA = NULL;
	rB = NULL;
}
/***************************************************************/
/* Function: ~BinaryStereo                                     */
/* Usage: free class member variables                          */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
BinaryStereo::~BinaryStereo(void)
{
	if( pairDistr ) {
		delete [] pairDistr;
	}
	if( lCode ) {
		delete [] lCode;
	}
	if( rCode ) {
		delete [] rCode;
	}
	if( lUsed ) {
		delete [] lUsed;
	}
	if( rUsed ) {
		delete [] rUsed;
	}
	if( l_r ) {
		delete [] l_r;
	}
	if( l_g ) {
		delete [] l_g;
	}
	if( l_b ) {
		delete [] l_b;
	}
	if( r_r ) {
		delete [] r_r;
	}
	if( r_g ) {
		delete [] r_g;
	}
	if( r_b ) {
		delete [] r_b;
	}
	if( lL ) {
		delete [] lL;
	}
	if( lA ) {
		delete [] lA;
	}
	if( lB ) {
		delete [] lB;
	}
	if( rL ) {
		delete [] rL;
	}
	if( rA ) {
		delete [] rA;
	}
	if( rB ) {
		delete [] rB;
	}
	if( lIsEdge ) {
		delete [] lIsEdge;
	}
	if( rIsEdge ) {
		delete [] rIsEdge;
	}
}
/***************************************************************/
/* Function: BinaryStereo                                      */
/* Usage: set image color data                                 */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::setClrData()
{

	l_b = new int[ imgSize ];
	l_g = new int[ imgSize ];
	l_r = new int[ imgSize ];
	r_b = new int[ imgSize ];
	r_g = new int[ imgSize ];
	r_r = new int[ imgSize ];
	l_gray = new int[ imgSize ];
	r_gray = new int[ imgSize ];
	lL = new double[ imgSize ];
	lA = new double[ imgSize ];
	lB = new double[ imgSize ];
	rL = new double[ imgSize ];
	rA = new double[ imgSize ];
	rB = new double[ imgSize ];


	double *plL, *plA, *plB, *prL, *prA, *prB;
	int *pl_b, *pl_g, *pl_r, *pr_b, *pr_g, *pr_r;
	int *pl_gray, *pr_gray;

	plL = lL;
	plA = lA;
	plB = lB;
	prL = rL;
	prA = rA;
	prB = rB;

	pl_b = l_b;
	pl_g = l_g;
	pl_r = l_r;
	pr_b = r_b;
	pr_g = r_g;
	pr_r = r_r;

	pl_gray = l_gray;
	pr_gray = r_gray;
	IplImage* lGray;
	IplImage* rGray;
	lGray = cvCreateImage( cvSize( width,height ), IPL_DEPTH_8U, 1 );
	cvCvtColor( lClr, lGray, CV_BGR2GRAY );
	rGray = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );
	cvCvtColor( rClr, rGray, CV_BGR2GRAY );
	for( int y = 0; y < height; y ++ ) {
		uchar* gray1 = ( uchar* ) ( lGray->imageData 
			+ y * lGray->widthStep );
		uchar* gray2 = ( uchar* ) ( rGray->imageData 
			+ y * rGray->widthStep );

		for( int x = 0; x < width; x ++ ) {
			uchar* bgr1 = ( uchar* ) lClr->imageData 
				+ y * lClr->widthStep + 3 * x;

			uchar* bgr2 = ( uchar* ) rClr->imageData
				+ y * rClr->widthStep + 3 * x;

			// BGR
			*pl_b = ( int ) ( bgr1[ 0 ] );
			*pl_g = ( int ) ( bgr1[ 1 ] );
			*pl_r = ( int ) ( bgr1[ 2 ] );

			*pr_b = ( int ) ( bgr2[ 0 ] );
			*pr_g = ( int ) ( bgr2[ 1 ] );
			*pr_r = ( int ) ( bgr2[ 2 ] );

			// GRAY
			*pl_gray = ( int ) ( gray1[ x ] );
			*pr_gray = ( int ) ( gray2[ x ] );

			// LAB
			// !!!!! computer use BGR  NOT RGB !!!!
			RGBtoLab( *pl_b, *pl_g, *pl_r,
				*plL, *plA, *plB );
			RGBtoLab( *pr_b, *pr_g, *pr_r,
				*prL, *prA, *prB );

			// BGR
			pl_b ++;
			pl_g ++;
			pl_r ++;
			pr_b ++;
			pr_g ++;
			pr_r ++;
			//GRAY
			pl_gray ++;
			pr_gray ++;
			//LAB
			plL ++;
			plA ++;
			plB ++;
			prL ++;
			prA ++;
			prB ++;

		}
	}
	cvReleaseImage(&lGray);
	cvReleaseImage(&rGray);
}
/***************************************************************/
/* Function: setPairDistr                                      */
/* Usage: set pixel pair distribution                          */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::setPairDistr()
{
	srand( ( unsigned int )time( NULL ) );

	int trial;
	int brd = patchSz /2;
	for (int i=0; i< 4 * descLen; i ++ ) {
		do {
			trial = int( RandNormal(gStd) );
		} while ( trial < ( -brd ) || trial > ( brd ) );
		pairDistr[i]=trial;
	}

}
/***************************************************************/
/* Function: setEdgeData                                       */
/* Usage: set edge labels                                      */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::setEdgeData()
{
	IplImage* lGray = cvCreateImage( cvSize( width,height ), IPL_DEPTH_8U, 1 );  
	cvCvtColor( lClr, lGray, CV_BGR2GRAY );
	IplImage* rGray = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );
	cvCvtColor( rClr, rGray, CV_BGR2GRAY );
	double low, high;
	IplImage* lEdge = cvCreateImage( cvGetSize( lGray ), IPL_DEPTH_8U, 1 );		// edge image
	IplImage* rEdge = cvCreateImage( cvGetSize( rGray ), IPL_DEPTH_8U, 1 );
	CvMat* dx = cvCreateMat( height, width, CV_16SC1 );								
	CvMat* dy = cvCreateMat( height, width, CV_16SC1 );
	cvSobel( lGray, dx, 1, 0 );
	cvSobel( lGray, dy, 0, 1 );
	AdaptiveFindThreshold( dx, dy, &low, &high);
	cvCanny( lGray, lEdge, low, high );					// canny edge detection
	cvSobel( rGray, dx, 1, 0 );
	cvSobel( rGray, dy, 0, 1 );
	AdaptiveFindThreshold( dx, dy, &low, &high);
	cvCanny( rGray, rEdge, low, high );
	cvReleaseMat( &dx );
	cvReleaseMat( &dy );

														// set edge data
	lIsEdge = new int[ imgSize ];
	rIsEdge = new int[ imgSize ];
	int ereaRadius = maxDis;
	// set left edge erea
	memset( lIsEdge, 0, imgSize * sizeof( int ) );
	for( int y = 0; y < height; y ++ ) {
		uchar* lEdgeData = ( uchar* ) ( lEdge->imageData + y * lEdge->widthStep );
		int* tempIsEdge = lIsEdge + y * width;
		for( int x = 0; x < width; x ++ ) {
			if( lEdgeData[ x ] == 255 ) {
				// just on the edge
				tempIsEdge[ x ] = 1;
				continue;
			} else {
				// if [ -e, e ] has edge define as edge erea
				for( int r = - ereaRadius; r <= ereaRadius; r++ ) {
					if( x + r < 0 || x + r >= width ) {
						// close to image edge
						tempIsEdge[ x ] = 1;
						break;
					}
					if( lEdgeData[ x + r ] == 255 ) {
						tempIsEdge[ x ] = 1;
						break;
					}
				}
			}
			if( tempIsEdge[ x ] == 1 ) { 
				lEdgeData[ x ] = 128;
			}
		}
	}

	// set right edge erea
	memset( rIsEdge, 0, imgSize * sizeof( int ) );
	for( int y = 0; y < height; y ++ ) {
		uchar* rEdgeData = ( uchar* ) ( rEdge->imageData + y * rEdge->widthStep );
		int* tempIsEdge = rIsEdge + y * width;
		for( int x = 0; x < width; x ++ ) {
			if( rEdgeData[ x ] == 255 ) {
				// just on the edge
				tempIsEdge[ x ] = 1;
				continue;
			} else {
				// if [ -e, e ] has edge define as edge erea
				for( int r = - ereaRadius; r <= ereaRadius; r++ ) {
					if( x + r < 0 || x + r >= width ) {
						// close to image edge
						tempIsEdge[ x ] = 1;
						break;
					}
					if( rEdgeData[ x + r ] == 255 ) {
						tempIsEdge[ x ] = 1;
						break;
					}
				}
			}
			if( tempIsEdge[ x ] == 1 ) {
				rEdgeData[ x ] = 128;
			}
		}
	}
	cvReleaseImage(&lGray);
	cvReleaseImage(&rGray);
	cvReleaseImage(&lEdge);
	cvReleaseImage(&rEdge);
}
/***************************************************************/
/* Function: prepareUsed                                       */
/* Usage: set binary mask                                      */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::prepareUsed()
{
#ifdef OUTPUT_USED_WGT
	FILE* fp = fopen( "wgt.txt", "w" );
	double sum = 0.0f;
#endif
	int mask = patchSz / 2;
	lUsed = new bitset<DESC_LEN>[ imgSize ];
	rUsed = new bitset<DESC_LEN>[ imgSize ];
	int* testsLWgt = new int[ DESC_LEN ];
	int* testsRWgt = new int[ DESC_LEN ];
	int topMax = WGT_CNT;

	int lWgtCount[ WGT_CNT ];
	int rWgtCount[ WGT_CNT ];

	bitset<DESC_LEN>* tempLUsed = lUsed;
	bitset<DESC_LEN>* tempRUsed = rUsed;
	for( int y = 0; y < height; y ++ ) {
		for( int x = 0; x < width; x ++  ) {
			int index = y * width + x;
			memset( testsLWgt, 0, sizeof( int ) * DESC_LEN );
			memset( testsRWgt, 0, sizeof( int ) * DESC_LEN );

			memset( lWgtCount, 0, sizeof( int ) * WGT_CNT );
			memset( rWgtCount, 0, sizeof( int ) * WGT_CNT );
			int* xd1 = pairDistr;
			int* yd1 = pairDistr + 1;
			int* xd2 = pairDistr + 2;
			int* yd2 = pairDistr + 3;

			for( int i = 0; i < DESC_LEN; i ++ ) {
				int x1 = x + (*xd1);
				int y1 = y + (*yd1);
				int x2 = x + (*xd2);
				int y2 = y + (*yd2);

				if( x1 < 0 || x1 >= width ) {
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( y1 < 0 || y1 >= height ) {
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( x2 < 0 || x2 >= width ) {
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( y2 < 0 || y2 >= height ) {
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}

				int tempL, tempR;
				int index1 = y1 * width + x1;
				int index2 = y2 * width + x2;
				tempL = MAX( abs( lL[ index1 ] - lL[ index ] ) + 
					abs( lA[ index1 ] - lA[ index ] ) +
					abs( lB[ index1 ] - lB[ index ] ), 

					abs( lL[ index2 ] - lL[ index ] ) + 
					abs( lA[ index2 ] - lA[ index ] ) +
					abs( lB[ index2 ] - lB[ index ] )
					);
				tempR = MAX( abs( rL[ index1 ] - rL[ index ] ) + 
					abs( rA[ index1 ] - rA[ index ] ) +
					abs( rB[ index1 ] - rB[ index ] ), 

					abs( rL[ index2 ] - rL[ index ] ) + 
					abs( rA[ index2 ] - rA[ index ] ) +
					abs( rB[ index2 ] - rB[ index ] )
					);

				testsLWgt[ i ] = tempL;
				testsRWgt[ i ] = tempR;
				lWgtCount[ tempL ] ++;
				rWgtCount[ tempR ] ++;

				xd1 += 4;
				yd1 += 4;
				xd2 += 4;
				yd2 += 4;
			}
			int lMid = MyGetCutVal( lWgtCount );
#ifdef OUTPUT_USED_WGT
			sum += lMid;
			fprintf( fp, "%d\n", lMid );
#endif
			int rMid = MyGetCutVal( rWgtCount );
			for( int i = 0; i < DESC_LEN; i ++ ) {
				if( testsLWgt[ i ] <= lMid ) {
					( *tempLUsed )[ i ] = 1;
				} else {
					( *tempLUsed )[ i ] = 0;
				}
				if( testsRWgt[ i ] <= rMid ) {
					( *tempRUsed )[ i ] = 1;
				} else {
					( *tempRUsed )[ i ] = 0;
				}

			}
			tempLUsed ++;
			tempRUsed ++;
		}
	}
	delete [] testsLWgt;
	delete [] testsRWgt;
#ifdef OUTPUT_USED_WGT
	fprintf( fp, "\n%.4f\n", sum / imgSize );
	fclose( fp );
#endif
}
/***************************************************************/
/* Function: prepareCode                                       */
/* Usage: set descriptor                                       */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::prepareCode()
{
	int mask = patchSz / 2;
	int x1, y1, x2, y2;
	int index1, index2;
	lCode = new bitset<DESC_LEN>[ imgSize ];
	rCode = new bitset<DESC_LEN>[ imgSize ];
	bitset<DESC_LEN>* tempLCode = lCode;
	bitset<DESC_LEN>* tempRCode = rCode;
	for( int y = 0; y < height; y ++ ) {
		for( int x = 0; x < width; x ++ ) {
			int* xd1 = pairDistr;
			int* yd1 = pairDistr + 1;
			int* xd2 = pairDistr + 2;
			int* yd2 = pairDistr + 3;
			for( int i = 0; i < DESC_LEN;  i ++ ) {
				x1 = x + (*xd1);
				y1 = y + (*yd1);
				x2 = x + (*xd2);
				y2 = y + (*yd2);

				// border check
				if( x1 < 0 || x1 >= width ) {
					( *tempLCode )[ i ] = 0;
					( *tempRCode )[ i ] = 1;
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( y1 < 0 || y1 >= height ) {
					( *tempLCode )[ i ] = 0;
					( *tempRCode )[ i ] = 1;
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( x2 < 0 || x2 >= width ) {
					( *tempLCode )[ i ] = 0;
					( *tempRCode )[ i ] = 1;
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				if( y2 < 0 || y2 >= height ) {
					( *tempLCode )[ i ] = 0;
					( *tempRCode )[ i ] = 1;
					xd1 += 4;
					yd1 += 4;
					xd2 += 4;
					yd2 += 4;
					continue;
				}
				index1 = y1 * width + x1;
				index2 = y2 * width + x2;
				(*tempLCode)[ i ] = ( l_gray[ index1 ]  > l_gray[ index2 ] );
				(*tempRCode)[ i ] = ( r_gray[ index1 ]  > r_gray[ index2 ] );
				xd1 += 4;
				yd1 += 4;
				xd2 += 4;
				yd2 += 4;
			}
			tempLCode ++;
			tempRCode ++;
		}
	}
}
/***************************************************************/
/* Function: PreProcess                                        */
/* Usage: prepare data for stereo matching                     */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::PreProcess()
{
	setClrData();
	setPairDistr();
	setEdgeData();
	prepareUsed();
	prepareCode();
}
/***************************************************************/
/* Function: Match                                             */
/* Usage: binary stereo matching                               */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::Match()
{
	bitset<DESC_LEN> lB;
	bitset<DESC_LEN> rB;
	bitset<DESC_LEN> lU;
	bitset<DESC_LEN> rU;
	bitset<DESC_LEN> allUsed;
	bitset<DESC_LEN>* tempLCode = lCode;
	bitset<DESC_LEN>* tempRCode = rCode;
	bitset<DESC_LEN>* tempLUsed = lUsed;
	bitset<DESC_LEN>* tempRUsed = rUsed;

	int isUseful = descLen / 16;

	int xST = 0;
	int xED = width;
	int yST = 0;
	int yED = height;
#ifdef MATCH_SMALL
	xST = X_START;
	xED = X_END;
	yST = Y_START;
	yED = Y_END;
#endif
	// calc left depth
	for( int y = yST; y  < yED; y ++ ) {
		uchar* dData = ( uchar* )lDis->imageData + y * lDis->widthStep;
		int index = y * width;
		for( int x = xST; x < xED; x ++ ) {
			int minDiff = descLen;
			int minDiffCount = -1;
			int matchDis = 0;
			int isEdge = lIsEdge[ index + x ];

			lU = ( *tempLUsed );
			lB = ( *tempLCode );

			for( int d = 0; d < maxDis; d ++ ) {
				if( x - d >= 0 ) {
					rB = rCode[ index + x - d ];
					rU = rUsed[ index + x - d ];
					int diff = 0;
					int diffCount = 0;

					if( isEdge ){
						// use binary mask only for edge
						diff      = ( lU & ( lB ^ rB ) ).count( );
						diffCount = lU.count();
					} else {
						diff      = ( lB ^ rB ).count();
						diffCount = DESC_LEN;
					}

					if( diffCount > isUseful ) {
						if(  diff  < minDiff ) {
							// record top min diff
							minDiff = diff;
							minDiffCount = diffCount;
							matchDis = d;
						}
					}
				}
			}
			dData[ x ] = matchDis * disScale;
			tempLCode ++;
			tempLUsed ++;
		}
	}
	// calc right depth
	for( int y = yST; y  < yED; y ++ ) {
		uchar* dData = ( uchar* )rDis->imageData + y * rDis->widthStep;
		int index = y * width;
		for( int x = xST; x < xED; x ++ ) {
			int minDiff = descLen;
			int minDiffCount = -1;
			int matchDis = 0;
			int isEdge = rIsEdge[ index + x ];

			rU = ( *tempRUsed );
			rB = ( *tempRCode );
			for( int d = 0; d < maxDis; d ++ ) {
				if( x + d < width ) {
					// match distribution
					lB = lCode[ index + x + d ];
					lU = lUsed[ index + x + d ];

					int diff = 0;
					int diffCount = 0;


					if( isEdge ){
						diff      = ( rU & ( lB ^ rB ) ).count( );
						diffCount = rU.count();
					} else {
						diff      = ( lB ^ rB ).count();
						diffCount = DESC_LEN;
					}

					if( diffCount > isUseful ) {
						if(  diff  < minDiff ) {
							// record top min diff
							minDiff = diff;
							minDiffCount = diffCount;
							matchDis = d;
						}
					}
				}
			}
			dData[ x ] = matchDis * disScale;
			tempRCode ++;
			tempRUsed ++;
		}
	}

}
/***************************************************************/
/* Function: PostProcess                                       */
/* Usage: currently implement by Weidong Hu                    */
/* In:                                                         */
/* Out:                                                        */
/***************************************************************/
void BinaryStereo::PostProcess()
{
	// using post processing program
}