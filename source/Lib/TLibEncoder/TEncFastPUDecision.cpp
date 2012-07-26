/* 
 * File:   TEncFastPUDecision.cpp
 * Author: felsamps
 * 
 * Created on September 6, 2011, 3:24 PM
 */

#include "TEncFastPUDecision.h"

TComMv TEncFastPUDecision::bestMv[4];
TComMv TEncFastPUDecision::prefMv[4]; //Se vamos usar vector, temos que resolver BAD_ACCESS da indexacao direta
UInt TEncFastPUDecision::bestDist[4];
UInt TEncFastPUDecision::prefDist[4];
Bool TEncFastPUDecision::borderA;
Bool TEncFastPUDecision::borderB;
Bool TEncFastPUDecision::borderC;
Bool TEncFastPUDecision::borderD;
PartSize TEncFastPUDecision::testSMP;
PartSize TEncFastPUDecision::testAMP;
UInt TEncFastPUDecision::currPartIdx;
UInt TEncFastPUDecision::currDepth;
UInt TEncFastPUDecision::fastPU;
const PredMode TEncFastPUDecision::predMode;
TComDataCU* TEncFastPUDecision::cu;
std::vector<UInt**> TEncFastPUDecision::distMap;
Int TEncFastPUDecision::refFrameIdx;
Bool TEncFastPUDecision::fastMode;
UInt TEncFastPUDecision::bs[4];
UInt TEncFastPUDecision::cuSize, TEncFastPUDecision::cuStride;
Pel* TEncFastPUDecision::cuOrg;
Int TEncFastPUDecision::bestRefIdx;

void TEncFastPUDecision::init() {
    borderA = false;
    borderB = false;
    borderC = false;
    borderD = false;
    currPartIdx = 0;
    cu = NULL;
    fastMode = true;
    for (int i = 0; i < 4; i++) {
        testSMP[i] = false;
        testAMP[i+4] = false;
        bestDist[i] = MAX_UINT;                  
    }

}

void TEncFastPUDecision::motionVectorMerge() {
	bool a = borderA;
	bool b = borderB;
	bool c = borderC;
	bool d = borderD;

	if( (a&&b)||(b&&d)||(a&&c)||(c&&d) ) 
		testSMP[SIZE_2Nx2N] = true;
        else if (a || d)
		testSMP[SIZE_2NxN] = true;
        else if (b || c)
		testSMP[SIZE_Nx2N] = true;
        else testSMP[SIZE_NxN] = true;
	 
}

void TEncFastPUDecision::decideMVSimilarity() {
	if(bestMv[0] == bestMv[1]) setBorderA(true);
	if(bestMv[0] == bestMv[2]) setBorderB(true);
	if(bestMv[1] == bestMv[3]) setBorderC(true);
	if(bestMv[2] == bestMv[3]) setBorderD(true);

	motionVectorMerge();

}

UInt TEncFastPUDecision::xTestHorBorders(Pel* pel) {
	Int acum = 0;
	Pel* p = pel + ((cuSize/8)*cuStride);
	UInt puSize = cuSize/2;
	for(int x = 0; x < cuSize; x++) {
		for (int y = 0; y < puSize/4; y++) {
			acum += abs(p[y*cuStride] - p[((puSize/2)-1-y)*cuStride]);
		}
		p += 1;
	}
	return acum;
}

UInt TEncFastPUDecision::xTestVerBorders(Pel* pel) {
	Int acum = 0;
	Pel* p = pel + (cuSize/8);
	UInt puSize = cuSize/2;
	for (int y = 0; y < cuSize; y++) {
		for (int x = 0; x < puSize/4; x++) {
			acum += abs(p[x] - p[(puSize/2)-1-x]);
		}
		p += cuStride;
	}
	return acum;

}

void TEncFastPUDecision::performBordersTests() {
	//test upper asymmetric border
	bs[0] = xTestHorBorders(cuOrg);
	//test lower asymmetric border
	bs[1] = xTestHorBorders(cuOrg + (cuSize/2)*cuStride);
	//test left asymmetric border
	bs[2] = xTestVerBorders(cuOrg);
	//test right asymmetric border
	bs[3] = xTestVerBorders(cuOrg + (cuSize/2));
}

void TEncFastPUDecision::borderStrengthDecision() {

	performBordersTests();

	UInt bsBest = -MAX_INT -1;
	UInt idBest = -1;

	Int diffBs[4];

	diffBs[0] = bs[0] - (bs[1] + bs[2] + bs[3])/3;
	diffBs[1] = bs[1] - (bs[0] + bs[2] + bs[3])/3;
	diffBs[2] = bs[2] - (bs[0] + bs[1] + bs[3])/3;
	diffBs[3] = bs[3] - (bs[0] + bs[1] + bs[2])/3;

	for (int i = 0; i < 4; i++) {
		if(bsBest < diffBs[i]) {
			idBest = i;
			bsBest = diffBs[i];
		}
	}
	
        if((currDepth == 1 || currDepth == 2) && fastPU == 3){
            for (int i = 0; i < 4; i++){
                testAMP[i] = true;
            }
            return;
        }
        
        testAMP[idBest+4] == true;
	
}

void TEncFastPUDecision::processMVs() {
	UInt bestAcumDist = 0;
	UInt prefAcumDist = 0;

	for (int i = 0; i < 4; i++) {
		prefAcumDist += prefDist[i];
		bestAcumDist += bestDist[i];
	}

	if(prefAcumDist < bestAcumDist) {
		bestRefIdx = refFrameIdx;
		for (int i = 0; i < 4; i++) {
			bestDist[i] = prefDist[i];
		}
	}
}