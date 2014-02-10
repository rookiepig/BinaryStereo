/***************************************************************/
/* File: CommonFunc.h                                          */
/* Usage: header file for common functions                     */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#pragma once

// RGB->LAB
inline double F(double input);
void RGBtoXYZ(double R, double G, double B, double &X, double &Y, double &Z);
void XYZtoLab(double X, double Y, double Z, double &L, double &a, double &b);
void RGBtoLab(double R, double G, double B, double &L, double &a, double &b);

// generate gaussian distribution
double RandNormal( const double std, const double mu = 0.0f );
// get 1 / 4 value
#define WGT_CNT 768
#define CLR_WGT_THRES 768
int MyGetCutVal( int* wgtCnt );
// adaptive threshold for canny edge detecotr
void AdaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high);