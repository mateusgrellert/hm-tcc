#ifndef _TENCFASTPUDECISION_H
#define	_TENCFASTPUDECISION_H

#include <vector>
#include <string>
#include "../TLibCommon/TComMv.h"
#include "../TLibCommon/TComDataCU.h"

class TEncFastPUDecision {

public:
    static TComMv bestMv[4], prefMv[4]; //Se vamos usar vector, temos que resolver BAD_ACCESS da indexacao direta
    static UInt bestDist[4], prefDist[4];
    static Bool borderA, borderB, borderC, borderD;
    static PartSize partSize, partSizeAMP;
    static UInt currPartIdx;
    static Int refFrameIdx;
    static const PredMode predMode = MODE_INTER;
    static TComDataCU *cu;
    static std::vector<UInt**> distMap;
    static bool fastMode;
    static UInt bs[4];
    static UInt cuSize, cuStride;
    static Pel* cuOrg;
    static Int bestRefIdx;


    TEncFastPUDecision();
    
    /* Fast PU Decision Methods */
    static void init();
    static void decideMVSimilarity();
    static PartSize approach01();
    static PartSize approach02();
    static void processMVs();

    static UInt xTestHorBorders(Pel* p);
    static UInt xTestVerBorders(Pel* p);
    static void performBordersTests();
    static void decideAMPBorders();

    static void xSetCUParameters();
    static std::string report();
        
    static void        setCU               (TComDataCU *pCu)           { cu = pCu; }
    static TComDataCU* getCU               ()                          { return cu; }

    static void        setCurrPartIdx      (UInt idx)                  { currPartIdx = idx; }
    static UInt        getCurrPartIdx      ()                          { return currPartIdx; }


    /* Best Matches' Information Setters and Getters */
    static void        setBestMv           (UInt block, TComMv mv)    { bestMv[block] = mv; }
    static void        setBestMv           (TComMv mv)                 { bestMv[currPartIdx] = mv; }
    static TComMv      getBestMv           (UInt block)                { return bestMv[block]; }

    static void        setBestDist         (UInt block, UInt dist)     { bestDist[block] = dist; }
    static void        setBestDist         (UInt dist)                 { bestDist[currPartIdx] = dist; }
    static UInt        getBestDist         (UInt block)                { return bestDist[block]; }

    /* Prefferred Matches' Information Setters and Getters */
    static void        setPrefMv           (UInt block, TComMv mv)     { prefMv[block] = mv; }
    static void        setPrefMv           (TComMv mv)                 { prefMv[currPartIdx] = mv; }
    static TComMv      getPrefMv           (UInt block)                { return prefMv[block]; }

    static void        setPrefDist         (UInt block, UInt dist)     { prefDist[block] = dist; }
    static void        setPrefDist         (UInt dist)                 { prefDist[currPartIdx] = dist; }
    static UInt        getPrefDist         (UInt block)                { return prefDist[block]; }

    /* Border flags setters and getters */
    static void        setBorderA          (Bool b)                    { borderA = b; }
    static void        setBorderB          (Bool b)                    { borderB = b; }
    static void        setBorderC          (Bool b)                    { borderC = b; }
    static void        setBorderD          (Bool b)                    { borderD = b; }

    static Bool        isBorderA           ()                          { return borderA; }
    static Bool        isBorderB           ()                          { return borderB; }
    static Bool        isBorderC           ()                          { return borderC; }
    static Bool        isBorderD           ()                          { return borderD; }

    static void        setRefIdx           (Int refIdx)                { refFrameIdx = refIdx; }
    static Int         getRefIdx           ()                          { return refFrameIdx; }

    static void        setCUSize           (UInt size)                 { cuSize = size; }
    static UInt        getCUSIze           ()                          { return cuSize; }

    static void        setCUStride         (UInt stride)               { cuStride = stride; }
    static UInt        getCUStride         ()                          { return cuStride; }

    static void        setPel              (Pel *p)                    { cuOrg = p; }
    static Pel*        getPel              ()                          { return cuOrg; }
};

#endif	/* _TENCFASTPUDECISION_H */

