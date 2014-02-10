/***************************************************************/
/* File: BinaryStereo.h                                        */
/* Usage: header file for BinaryStereo Class                   */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#pragma once
#include "IStereoAlg.h"
#include<bitset>
using namespace std;

#define  DESC_LEN 512		// descriptor length
#define  PATCH_SZ 26		// patch size for descriptor

class BinaryStereo : public IStereoAlg
{
public:
	int descLen;                // descriptor length
	int patchSz;                // patch size for descriptor
	int* pairDistr;				// pixel pairs [x1,y1,x2,y2,...xn,yn,xn+1,yn+1]
	double gStd;                // gaussian standard for sampling pixel pairs

	bitset<DESC_LEN>* lCode;	// descriptor for left&right images
	bitset<DESC_LEN>* rCode;
	bitset<DESC_LEN>* lUsed;    // binary mask for left&right images
	bitset<DESC_LEN>* rUsed;
	int* l_r;					// image data
	int* l_g;
	int* l_b;
	int* r_r;
	int* r_g;
	int* r_b;
	int* l_gray;				
	int* r_gray;
	double* lL;					
	double* lA;
	double* lB;
	double* rL;
	double* rA;
	double* rB;

	int*   lIsEdge;			    // edge detecting label
	int*   rIsEdge;
public:
	BinaryStereo(const int pS, const int mD, const int dS);
	~BinaryStereo(void);
public:
	// initial member variables
	void setClrData();				
	void setPairDistr();            
	void setEdgeData();             
	void prepareUsed();             
	void prepareCode();            
public :
	// implement IStereoAlg interface
	virtual void PreProcess();		
	virtual void Match();			
	virtual void PostProcess();		
};
