/***************************************************************/
/* File: main.cpp                                              */
/* Usage: mian entrance for stereo matching benchmark          */
/* Author: Zhang Kang                                          */
/* Date:                                                       */
/***************************************************************/
#include <Windows.h>
#include <opencv/highgui.h>
#include "IStereoAlg.h"
#include "BinaryStereo.h"
#include<map>
#include<string>
using namespace std;

enum StereoAlg{
	BSM,
	AdaptWgt
};

map<string,StereoAlg> algMap;		// algorithm name mapping



void InitAlgMap()
{
	algMap[string("BSM")]=BSM;
	algMap[string("AdaptWgt")]=AdaptWgt;
}

// Usage: [lImg] [rImg] [lDis] [rDis] [method] [maxDis] [disScale]
int main( int argc, char** argv )
{
	if( argc != 8 ) {
		printf( "Usage: [lImg] [rImg] [lDis] [rDis] [method] [maxDis] [disScale]\n");
		return -1;
	}

	string lImg(argv[1]), rImg(argv[2]);		
	string lDis(argv[3]), rDis(argv[4]);    
	string method(argv[5]);
	int maxDis   = atoi(argv[6]);
	int disScale = atoi(argv[7]);
	
	InitAlgMap();
	// decide algorithm
	map<string,StereoAlg>::iterator p = algMap.find(method);
	if( p != algMap.end() ) {
		IStereoAlg* matcher;
		switch(p->second) {
			case BSM :
				matcher = new BinaryStereo(PATCH_SZ,maxDis,disScale);
				break;
			case AdaptWgt :
				break;
		}
	
		DWORD st,ed;
		st=GetTickCount();
		// print info
		printf("\n--------------------------------------\n");
		printf( "Files:\n%s\n%s\n", lImg.c_str(),rImg.c_str());
		printf( "Method: %s\n", method.c_str());

		matcher->LoadImg(lImg.c_str(),rImg.c_str()); 
		printf("Load-->");

		matcher->PreProcess();                       
		printf("PreProcss-->");

		matcher->Match();                         
		printf("Match-->");

		matcher->SaveDep(lDis.c_str(),rDis.c_str()); 
		printf("Save\n");
		ed=GetTickCount();
		printf("Total Time : %ld ms\n", ed - st);
		printf("--------------------------------------\n");
	} else {
		printf( "Algorithm not implented!\n" );
		return -1;
	}
	return 0;
}