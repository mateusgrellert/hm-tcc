/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.  
 *
 * Copyright (c) 2010-2012, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     TEncCfg.h
    \brief    encoder configuration class (header)
*/

#ifndef __TENCCFG__
#define __TENCCFG__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TLibCommon/CommonDef.h"
#include <assert.h>

#if G1002_RPS
struct GOPEntry
{
  Int m_iPOC;
  Int m_iQPOffset;
  Double m_iQPFactor;
  Int m_iTemporalId;
  Bool m_bRefPic;
  Int m_iRefBufSize;
  Char m_iSliceType;
  Int m_iNumRefPics;
  Int m_aiReferencePics[MAX_NUM_REF_PICS];
  Int m_aiUsedByCurrPic[MAX_NUM_REF_PICS];
#if INTER_RPS_PREDICTION
  Bool m_bInterRPSPrediction;
  Int m_iDeltaRIdxMinus1;
  Int m_iDeltaRPS;
  Int m_iNumRefIdc;
  Int m_aiRefIdc[MAX_NUM_REF_PICS+1];
#endif
  GOPEntry()
  : m_iPOC(-1)
  , m_iQPOffset()
  , m_iQPFactor()
  , m_iTemporalId()
  , m_bRefPic()
  , m_iRefBufSize()
  , m_iSliceType()
  , m_iNumRefPics()
#if INTER_RPS_PREDICTION
  , m_bInterRPSPrediction()
  , m_iDeltaRIdxMinus1()
  , m_iDeltaRPS()
  , m_iNumRefIdc()
#endif
  {
    ::memset( m_aiReferencePics, 0, sizeof(m_aiReferencePics) );
    ::memset( m_aiUsedByCurrPic, 0, sizeof(m_aiUsedByCurrPic) );
#if INTER_RPS_PREDICTION
    ::memset( m_aiRefIdc,        0, sizeof(m_aiRefIdc) );
#endif
  }
};

std::istringstream &operator>>(std::istringstream &in, GOPEntry &entry);     //input
#endif
//! \ingroup TLibEncoder
//! \{

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// encoder configuration class
class TEncCfg
{
protected:
  //==== File I/O ========
  Int       m_iFrameRate;
  Int       m_FrameSkip;
  Int       m_iSourceWidth;
  Int       m_iSourceHeight;
  Int       m_iFrameToBeEncoded;
#if G678_LAMBDA_ADJUSTMENT  
  Double    m_adLambdaModifier[ MAX_TLAYER ];
#endif

  //====== Coding Structure ========
  UInt      m_uiIntraPeriod;
  UInt      m_uiDecodingRefreshType;            ///< the type of decoding refresh employed for the random access.
  Int       m_iGOPSize;
#if G1002_RPS
  GOPEntry  m_pcGOPList[MAX_GOP];
  Int       m_iExtraRPSs;
  UInt      m_uiMaxNumberOfReferencePictures;
  Int       m_numReorderFrames;
#else
  Int       m_iRateGOPSize;
  Int       m_iNumOfReference;
  Int       m_iNumOfReferenceB_L0;
  Int       m_iNumOfReferenceB_L1;
#endif
  
  Int       m_iQP;                              //  if (AdaptiveQP == OFF)
  
  Int       m_aiTLayerQPOffset[MAX_TLAYER];
  Int       m_aiPad[2];
  

  Int       m_iMaxRefPicNum;                     ///< this is used to mimic the sliding mechanism used by the decoder
                                                 // TODO: We need to have a common sliding mechanism used by both the encoder and decoder

  Bool      m_bTLayering;                        ///< indicates whether temporal IDs are set based on the hierarchical coding structure
  Bool      m_abTLayerSwitchingFlag[MAX_TLAYER]; ///< temporal layer switching flags corresponding to temporal layer
#if DISABLE_4x4_INTER
  Bool      m_bDisInter4x4;
#endif
#if AMP
  Bool m_useAMP;
#endif
  //======= Transform =============
  UInt      m_uiQuadtreeTULog2MaxSize;
  UInt      m_uiQuadtreeTULog2MinSize;
  UInt      m_uiQuadtreeTUMaxDepthInter;
  UInt      m_uiQuadtreeTUMaxDepthIntra;
  
#if NSQT
  Bool      m_useNSQT;
#endif
  
  //====== B Slice ========
#if !G1002_RPS
  Bool      m_bHierarchicalCoding;              //  hierarchical-B coding
#endif
  
#if !DISABLE_CAVLC
  //====== Entropy Coding ========
  Int       m_iSymbolMode;                      //  (CAVLC, CABAC)
#endif
  
  //====== Loop/Deblock Filter ========
  Bool      m_bLoopFilterDisable;
#if G174_DF_OFFSET
  Bool      m_loopFilterOffsetInAPS;
  Int       m_loopFilterBetaOffsetDiv2;
  Int       m_loopFilterTcOffsetDiv2;
#else
  Int       m_iLoopFilterAlphaC0Offset;
  Int       m_iLoopFilterBetaOffset;
#endif

#if SAO
  Bool      m_bUseSAO;
#endif

  //====== Motion search ========
  Int       m_iFastSearch;                      //  0:Full search  1:Diamond  2:PMVFAST
  Int       m_iSearchRange;                     //  0:Full frame
  Int       m_bipredSearchRange;

  //====== Quality control ========
  Int       m_iMaxDeltaQP;                      //  Max. absolute delta QP (1:default)
  Int       m_iMaxCuDQPDepth;                   //  Max. depth for a minimum CuDQP (0:default)

#if G509_CHROMA_QP_OFFSET
  Int       m_iChromaQpOffset  ;                //  ChromaQpOffset    (0:default)
  Int       m_iChromaQpOffset2nd;               //  ChromaQpOffset2nd (0:default)
#endif

#if QP_ADAPTATION
  Bool      m_bUseAdaptiveQP;
  Int       m_iQPAdaptationRange;
#endif
  
  //====== Tool list ========
  Bool      m_bUseSBACRD;
  Bool      m_bUseALF;
  Int       m_iALFEncodePassReduction;

#if G215_ALF_NUM_FILTER
  Int       m_iALFMaxNumberFilters;
#endif

  Bool      m_bUseASR;
  Bool      m_bUseHADME;
#if !G1002_RPS
  Bool      m_bUseGPB;
#endif
  Bool      m_bUseLComb;
  Bool      m_bLCMod;
  Bool      m_bUseRDOQ;
#if !G1002_RPS
  Bool      m_bUseLDC;
#endif
  Bool      m_bUsePAD;
#if !G1002_RPS
  Bool      m_bUseNRF;
  Bool      m_bUseBQP;
#endif
  Bool      m_bUseFastEnc;
  //TCC: bool variables that enable/disable Fast modes
  UInt      m_uiFastPU;
  Bool      m_bFastTU;
  Bool      m_bFastCU;

#if EARLY_CU_DETERMINATION
  Bool      m_bUseEarlyCU;
#endif
#if CBF_FAST_MODE
  Bool      m_bUseCbfFastMode;
#endif
  Bool      m_bUseMRG; // SOPH:
  Bool      m_bUseLMChroma; 

  Int*      m_aidQP;
  UInt      m_uiDeltaQpRD;
  
  Bool      m_bUseConstrainedIntraPred;
#if MAX_PCM_SIZE
  Bool      m_usePCM;
  UInt      m_pcmLog2MaxSize;
#endif
  UInt      m_uiPCMLog2MinSize;
  //====== Slice ========
  Int       m_iSliceMode;
  Int       m_iSliceArgument; 
  //====== Entropy Slice ========
  Int       m_iEntropySliceMode;
  Int       m_iEntropySliceArgument;
#if FINE_GRANULARITY_SLICES
  Int       m_iSliceGranularity;
#endif
  Bool      m_bLFCrossSliceBoundaryFlag;

#if E192_SPS_PCM_BIT_DEPTH_SYNTAX
  Bool      m_bPCMInputBitDepthFlag;
  UInt      m_uiPCMBitDepthLuma;
  UInt      m_uiPCMBitDepthChroma;
#endif
#if E192_SPS_PCM_FILTER_DISABLE_SYNTAX 
  Bool      m_bPCMFilterDisableFlag;
#endif
#if TILES
#if NONCROSS_TILE_IN_LOOP_FILTERING
  Int       m_iTileBehaviorControlPresentFlag;
  Bool      m_bLFCrossTileBoundaryFlag;
#endif
  Int       m_iColumnRowInfoPresent;
  Int       m_iUniformSpacingIdr;
  Int       m_iTileBoundaryIndependenceIdr;
  Int       m_iNumColumnsMinus1;
  UInt*     m_puiColumnWidth;
  Int       m_iNumRowsMinus1;
  UInt*     m_puiRowHeight;
#if TILES_DECODER
  Int       m_iTileLocationInSliceHeaderFlag; //< enable(1)/disable(0) transmitssion of tile location in slice header

  Int       m_iTileMarkerFlag;              //< enable(1)/disable(0) transmitssion of light weight tile marker
  Int       m_iMaxTileMarkerEntryPoints;    //< maximum number of tile markers allowed in a slice (controls degree of parallelism)
  Double    m_dMaxTileMarkerOffset;         //< Calculated offset. Light weight tile markers will be transmitted for TileIdx= Offset, 2*Offset, 3*Offset ... 
#endif
#endif

#if OL_USE_WPP
  Int       m_iWaveFrontSynchro;
  Int       m_iWaveFrontFlush;
  Int       m_iWaveFrontSubstreams;
#endif

  bool m_pictureDigestEnabled; ///< enable(1)/disable(0) md5 computation and SEI signalling

#if !G1002_RPS
#if REF_SETTING_FOR_LD
  Bool      m_bUseNewRefSetting;
#endif
#endif

#if WEIGHT_PRED
  //====== Weighted Prediction ========
  Bool      m_bUseWeightPred;       //< Use of Weighting Prediction (P_SLICE)
  UInt      m_uiBiPredIdc;          //< Use of Bi-Directional Weighting Prediction (B_SLICE)
#endif
#if SCALING_LIST
  Int       m_useScalingListId;            ///< Using quantization matrix i.e. 0=off, 1=default, 2=file.
  char*     m_scalingListFile;          ///< quantization matrix file name
#endif

#if NO_TMVP_MARKING
  Bool      m_bEnableTMVP;
#endif

public:
  TEncCfg()          {}
  virtual ~TEncCfg()
  {
#if TILES
    if( m_iUniformSpacingIdr == 0 )
    {
      if( m_iNumColumnsMinus1 )
      { 
        delete[] m_puiColumnWidth; 
        m_puiColumnWidth = NULL;
      }
      if( m_iNumRowsMinus1 )
      {
        delete[] m_puiRowHeight;
        m_puiRowHeight = NULL;
      }
    }
#if TILES_DECODER
    m_iTileLocationInSliceHeaderFlag = 0;
    m_iTileMarkerFlag              = 0;
#endif
#endif
  }
  
  Void      setFrameRate                    ( Int   i )      { m_iFrameRate = i; }
  Void      setFrameSkip                    ( unsigned int i ) { m_FrameSkip = i; }
  Void      setSourceWidth                  ( Int   i )      { m_iSourceWidth = i; }
  Void      setSourceHeight                 ( Int   i )      { m_iSourceHeight = i; }
  Void      setFrameToBeEncoded             ( Int   i )      { m_iFrameToBeEncoded = i; }
  
  //====== Coding Structure ========
  Void      setIntraPeriod                  ( Int   i )      { m_uiIntraPeriod = (UInt)i; }
  Void      setDecodingRefreshType          ( Int   i )      { m_uiDecodingRefreshType = (UInt)i; }
  Void      setGOPSize                      ( Int   i )      { m_iGOPSize = i; }
#if G1002_RPS
  Void      setGopList                      ( GOPEntry*  piGOPList )      {  for ( Int i = 0; i < MAX_GOP; i++ ) m_pcGOPList[i] = piGOPList[i]; }
  Void      setExtraRPSs                    ( Int   i )      { m_iExtraRPSs = i; }
  GOPEntry  getGOPEntry                     ( Int   i )      { return m_pcGOPList[i]; }
  Void      setMaxNumberOfReferencePictures ( UInt u )       { m_uiMaxNumberOfReferencePictures = u;    }
  Void      setNumReorderFrames             ( Int  i )       { m_numReorderFrames = i;    }
#else
  Void      setRateGOPSize                  ( Int   i )      { m_iRateGOPSize = i; }
  Void      setNumOfReference               ( Int   i )      { m_iNumOfReference = i; }
  Void      setNumOfReferenceB_L0           ( Int   i )      { m_iNumOfReferenceB_L0 = i; }
  Void      setNumOfReferenceB_L1           ( Int   i )      { m_iNumOfReferenceB_L1 = i; }
#endif
  
  Void      setQP                           ( Int   i )      { m_iQP = i; }
  
  Void      setTemporalLayerQPOffset        ( Int*  piTemporalLayerQPOffset )      { for ( Int i = 0; i < MAX_TLAYER; i++ ) m_aiTLayerQPOffset[i] = piTemporalLayerQPOffset[i]; }
  Void      setPad                          ( Int*  iPad                   )      { for ( Int i = 0; i < 2; i++ ) m_aiPad[i] = iPad[i]; }
  
  Int       getMaxRefPicNum                 ()                              { return m_iMaxRefPicNum;           }
  Void      setMaxRefPicNum                 ( Int iMaxRefPicNum )           { m_iMaxRefPicNum = iMaxRefPicNum;  }

  Bool      getTLayering                    ()                              { return m_bTLayering;              } 
  Void      setTLayering                    ( Bool bTLayering )             { m_bTLayering = bTLayering;        }
  Bool      getTLayerSwitchingFlag          ( UInt uiTLayer )               { assert (uiTLayer < MAX_TLAYER ); return  m_abTLayerSwitchingFlag[uiTLayer];                   }
  Void      setTLayerSwitchingFlag          ( Bool* pbTLayerSwitchingFlag ) { for ( Int i = 0; i < MAX_TLAYER; i++ ) m_abTLayerSwitchingFlag[i] = pbTLayerSwitchingFlag[i]; }

#if DISABLE_4x4_INTER
  Bool      getDisInter4x4                  ()              { return m_bDisInter4x4;        }
  Void      setDisInter4x4                  ( Bool b )      { m_bDisInter4x4  = b;          }
#endif
  //======== Transform =============
  Void      setQuadtreeTULog2MaxSize        ( UInt  u )      { m_uiQuadtreeTULog2MaxSize = u; }
  Void      setQuadtreeTULog2MinSize        ( UInt  u )      { m_uiQuadtreeTULog2MinSize = u; }
  Void      setQuadtreeTUMaxDepthInter      ( UInt  u )      { m_uiQuadtreeTUMaxDepthInter = u; }
  Void      setQuadtreeTUMaxDepthIntra      ( UInt  u )      { m_uiQuadtreeTUMaxDepthIntra = u; }
  
#if NSQT
  Void setUseNSQT( Bool b ) { m_useNSQT = b; }
#endif
#if AMP
  Void setUseAMP( Bool b ) { m_useAMP = b; }
#endif
  
  //====== b; Slice ========
#if !G1002_RPS
  Void      setHierarchicalCoding           ( Bool  b )      { m_bHierarchicalCoding = b; }
#endif
  
#if !DISABLE_CAVLC
  //====== Entropy Coding ========
  Void      setSymbolMode                   ( Int   i )      { m_iSymbolMode = i; }
#endif
  
  //====== Loop/Deblock Filter ========
  Void      setLoopFilterDisable            ( Bool  b )      { m_bLoopFilterDisable       = b; }
#if G174_DF_OFFSET
  Void      setLoopFilterOffsetInAPS        ( Bool  b )      { m_loopFilterOffsetInAPS      = b; }
  Void      setLoopFilterBetaOffset         ( Int   i )      { m_loopFilterBetaOffsetDiv2  = i; }
  Void      setLoopFilterTcOffset           ( Int   i )      { m_loopFilterTcOffsetDiv2    = i; }
#else
  Void      setLoopFilterAlphaC0Offset      ( Int   i )      { m_iLoopFilterAlphaC0Offset = i; }
  Void      setLoopFilterBetaOffset         ( Int   i )      { m_iLoopFilterBetaOffset    = i; }
#endif

  //====== Motion search ========
  Void      setFastSearch                   ( Int   i )      { m_iFastSearch = i; }
  Void      setSearchRange                  ( Int   i )      { m_iSearchRange = i; }
  Void      setBipredSearchRange            ( Int   i )      { m_bipredSearchRange = i; }

  //====== Quality control ========
  Void      setMaxDeltaQP                   ( Int   i )      { m_iMaxDeltaQP = i; }
  Void      setMaxCuDQPDepth                ( Int   i )      { m_iMaxCuDQPDepth = i; }

#if G509_CHROMA_QP_OFFSET
  Void      setChromaQpOffset               ( Int   i ) { m_iChromaQpOffset    = i; }
  Void      setChromaQpOffset2nd            ( Int   i ) { m_iChromaQpOffset2nd = i; }
#endif

#if QP_ADAPTATION
  Void      setUseAdaptiveQP                ( Bool  b )      { m_bUseAdaptiveQP = b; }
  Void      setQPAdaptationRange            ( Int   i )      { m_iQPAdaptationRange = i; }
#endif
  
  //====== Sequence ========
  Int       getFrameRate                    ()      { return  m_iFrameRate; }
  unsigned int getFrameSkip                 ()      { return  m_FrameSkip; }
  Int       getSourceWidth                  ()      { return  m_iSourceWidth; }
  Int       getSourceHeight                 ()      { return  m_iSourceHeight; }
  Int       getFrameToBeEncoded             ()      { return  m_iFrameToBeEncoded; }
#if G678_LAMBDA_ADJUSTMENT  
  void setLambdaModifier                    ( UInt uiIndex, Double dValue ) { m_adLambdaModifier[ uiIndex ] = dValue; }
  Double getLambdaModifier                  ( UInt uiIndex ) const { return m_adLambdaModifier[ uiIndex ]; }
#endif

  //==== Coding Structure ========
  UInt      getIntraPeriod                  ()      { return  m_uiIntraPeriod; }
  UInt      getDecodingRefreshType          ()      { return  m_uiDecodingRefreshType; }
  Int       getGOPSize                      ()      { return  m_iGOPSize; }
#if !G1002_RPS
  Int       getRateGOPSize                  ()      { return  m_iRateGOPSize; }
  Int       getNumOfReference               ()      { return  m_iNumOfReference; }
  Int       getNumOfReferenceB_L0           ()      { return  m_iNumOfReferenceB_L0; }
  Int       getNumOfReferenceB_L1           ()      { return  m_iNumOfReferenceB_L1; }
  
#else
  UInt      getMaxNumberOfReferencePictures ()      { return m_uiMaxNumberOfReferencePictures; }
  Int       geNumReorderFrames              ()      { return m_numReorderFrames; }
#endif
  Int       getQP                           ()      { return  m_iQP; }
  
  Int       getTemporalLayerQPOffset        ( Int i )      { assert (i < MAX_TLAYER ); return  m_aiTLayerQPOffset[i]; }
  Int       getPad                          ( Int i )      { assert (i < 2 );                      return  m_aiPad[i]; }
  
  //======== Transform =============
  UInt      getQuadtreeTULog2MaxSize        ()      const { return m_uiQuadtreeTULog2MaxSize; }
  UInt      getQuadtreeTULog2MinSize        ()      const { return m_uiQuadtreeTULog2MinSize; }
  UInt      getQuadtreeTUMaxDepthInter      ()      const { return m_uiQuadtreeTUMaxDepthInter; }
  UInt      getQuadtreeTUMaxDepthIntra      ()      const { return m_uiQuadtreeTUMaxDepthIntra; }
  
  //==== b; Slice ========
#if !G1002_RPS
  Bool      getHierarchicalCoding           ()      { return  m_bHierarchicalCoding; }
#endif
  
#if !DISABLE_CAVLC
  //==== Entropy Coding ========
  Int       getSymbolMode                   ()      { return  m_iSymbolMode; }
#endif
  
  //==== Loop/Deblock Filter ========
  Bool      getLoopFilterDisable            ()      { return  m_bLoopFilterDisable;       }
#if G174_DF_OFFSET
  Bool      getLoopFilterOffsetInAPS        ()      { return m_loopFilterOffsetInAPS; }
  Int       getLoopFilterBetaOffset         ()      { return m_loopFilterBetaOffsetDiv2; }
  Int       getLoopFilterTcOffset           ()      { return m_loopFilterTcOffsetDiv2; }
#else
  Int       getLoopFilterAlphaC0Offget      ()      { return  m_iLoopFilterAlphaC0Offset; }
  Int       getLoopFilterBetaOffget         ()      { return  m_iLoopFilterBetaOffset;    }
#endif

  //==== Motion search ========
  Int       getFastSearch                   ()      { return  m_iFastSearch; }
  Int       getSearchRange                  ()      { return  m_iSearchRange; }

  //==== Quality control ========
  Int       getMaxDeltaQP                   ()      { return  m_iMaxDeltaQP; }
  Int       getMaxCuDQPDepth                ()      { return  m_iMaxCuDQPDepth; }
#if QP_ADAPTATION
  Bool      getUseAdaptiveQP                ()      { return  m_bUseAdaptiveQP; }
  Int       getQPAdaptationRange            ()      { return  m_iQPAdaptationRange; }
#endif
  
  //==== Tool list ========
  Void      setUseSBACRD                    ( Bool  b )     { m_bUseSBACRD  = b; }
  Void      setUseASR                       ( Bool  b )     { m_bUseASR     = b; }
  Void      setUseHADME                     ( Bool  b )     { m_bUseHADME   = b; }
  Void      setUseALF                       ( Bool  b )     { m_bUseALF   = b; }
#if !G1002_RPS
  Void      setUseGPB                       ( Bool  b )     { m_bUseGPB     = b; }
#endif
  Void      setUseLComb                     ( Bool  b )     { m_bUseLComb   = b; }
  Void      setLCMod                        ( Bool  b )     { m_bLCMod   = b;    }
  Void      setUseRDOQ                      ( Bool  b )     { m_bUseRDOQ    = b; }
#if !G1002_RPS
  Void      setUseLDC                       ( Bool  b )     { m_bUseLDC     = b; }
#endif
  Void      setUsePAD                       ( Bool  b )     { m_bUsePAD     = b; }
#if !G1002_RPS
  Void      setUseNRF                       ( Bool  b )     { m_bUseNRF     = b; }
  Void      setUseBQP                       ( Bool  b )     { m_bUseBQP     = b; }
#endif
  Void      setUseFastEnc                   ( Bool  b )     { m_bUseFastEnc = b; }
  //TCC: fast mode setters/getters
  Void      setFastPU                       ( UInt  ui )     { m_uiFastPU = ui; }
  Void      setFastCU                       ( Bool  b )     { m_bFastCU = b; }
  Void      setFastTU                       ( Bool  b )     { m_bFastTU = b; }
  
  UInt      getFastPU                       ( )     { return m_uiFastPU; }
  bool      getFastCU                       ( )     { return m_bFastCU; }
  bool      getFastTU                       ( )     { return m_bFastTU; }

#if EARLY_CU_DETERMINATION
  Void      setUseEarlyCU                   ( Bool  b )     { m_bUseEarlyCU = b; }
#endif
#if CBF_FAST_MODE
  Void      setUseCbfFastMode            ( Bool  b )     { m_bUseCbfFastMode = b; }
#endif
  Void      setUseMRG                       ( Bool  b )     { m_bUseMRG     = b; } // SOPH:
  Void      setUseConstrainedIntraPred      ( Bool  b )     { m_bUseConstrainedIntraPred = b; }
#if E192_SPS_PCM_BIT_DEPTH_SYNTAX
  Void      setPCMInputBitDepthFlag         ( Bool  b )     { m_bPCMInputBitDepthFlag = b; }
#endif
#if E192_SPS_PCM_FILTER_DISABLE_SYNTAX
  Void      setPCMFilterDisableFlag         ( Bool  b )     {  m_bPCMFilterDisableFlag = b; }
#endif
#if MAX_PCM_SIZE
  Void      setUsePCM                       ( Bool  b )     {  m_usePCM = b;               }
  Void      setPCMLog2MaxSize               ( UInt u )      { m_pcmLog2MaxSize = u;      }
#endif
  Void      setPCMLog2MinSize               ( UInt u )     { m_uiPCMLog2MinSize = u;      }
  Void      setdQPs                         ( Int*  p )     { m_aidQP       = p; }
  Void      setDeltaQpRD                    ( UInt  u )     {m_uiDeltaQpRD  = u; }
  Bool      getUseSBACRD                    ()      { return m_bUseSBACRD;  }
  Bool      getUseASR                       ()      { return m_bUseASR;     }
  Bool      getUseHADME                     ()      { return m_bUseHADME;   }
  Bool      getUseALF                       ()      { return m_bUseALF;     }
  Void      setALFEncodePassReduction       (Int i)  { m_iALFEncodePassReduction = i; }
  Int       getALFEncodePassReduction       ()       { return m_iALFEncodePassReduction; }

#if G215_ALF_NUM_FILTER
  Void      setALFMaxNumberFilters          (Int i)  { m_iALFMaxNumberFilters = i; } 
  Int       getALFMaxNumberFilters          ()       { return m_iALFMaxNumberFilters; } 
#endif

#if !G1002_RPS
  Bool      getUseGPB                       ()      { return m_bUseGPB;     }
#endif
  Bool      getUseLComb                     ()      { return m_bUseLComb;   }
  Bool      getLCMod                        ()      { return m_bLCMod; }
  Bool      getUseRDOQ                      ()      { return m_bUseRDOQ;    }
#if !G1002_RPS
  Bool      getUseLDC                       ()      { return m_bUseLDC;     }
#endif
  Bool      getUsePAD                       ()      { return m_bUsePAD;     }
#if !G1002_RPS
  Bool      getUseNRF                       ()      { return m_bUseNRF;     }
  Bool      getUseBQP                       ()      { return m_bUseBQP;     }
#endif
  Bool      getUseFastEnc                   ()      { return m_bUseFastEnc; }
#if EARLY_CU_DETERMINATION
  Bool      getUseEarlyCU                   ()      { return m_bUseEarlyCU; }
#endif
#if CBF_FAST_MODE
  Bool      getUseCbfFastMode           ()      { return m_bUseCbfFastMode; }
#endif
  Bool      getUseMRG                       ()      { return m_bUseMRG;     } // SOPH:
  Bool      getUseConstrainedIntraPred      ()      { return m_bUseConstrainedIntraPred; }
#if NS_HAD
  Bool      getUseNSQT                      ()      { return m_useNSQT; }
#endif
#if E192_SPS_PCM_BIT_DEPTH_SYNTAX
  Bool      getPCMInputBitDepthFlag         ()      { return m_bPCMInputBitDepthFlag;   } 
#endif
#if E192_SPS_PCM_FILTER_DISABLE_SYNTAX
  Bool      getPCMFilterDisableFlag         ()      { return m_bPCMFilterDisableFlag;   } 
#endif
#if MAX_PCM_SIZE
  Bool      getUsePCM                       ()      { return m_usePCM;                 }
  UInt      getPCMLog2MaxSize               ()      { return m_pcmLog2MaxSize;  }
#endif
  UInt      getPCMLog2MinSize               ()      { return  m_uiPCMLog2MinSize;  }

  Bool getUseLMChroma                       ()      { return m_bUseLMChroma;        }
  Void setUseLMChroma                       ( Bool b ) { m_bUseLMChroma  = b;       }

  Int*      getdQPs                         ()      { return m_aidQP;       }
  UInt      getDeltaQpRD                    ()      { return m_uiDeltaQpRD; }

  //====== Slice ========
  Void  setSliceMode                   ( Int  i )       { m_iSliceMode = i;              }
  Void  setSliceArgument               ( Int  i )       { m_iSliceArgument = i;          }
  Int   getSliceMode                   ()              { return m_iSliceMode;           }
  Int   getSliceArgument               ()              { return m_iSliceArgument;       }
  //====== Entropy Slice ========
  Void  setEntropySliceMode            ( Int  i )      { m_iEntropySliceMode = i;       }
  Void  setEntropySliceArgument        ( Int  i )      { m_iEntropySliceArgument = i;   }
  Int   getEntropySliceMode            ()              { return m_iEntropySliceMode;    }
  Int   getEntropySliceArgument        ()              { return m_iEntropySliceArgument;}
#if FINE_GRANULARITY_SLICES
  Void  setSliceGranularity            ( Int  i )      { m_iSliceGranularity = i;       }
  Int   getSliceGranularity            ()              { return m_iSliceGranularity;    }
#endif
  Void      setLFCrossSliceBoundaryFlag     ( Bool   bValue  )    { m_bLFCrossSliceBoundaryFlag = bValue; }
  Bool      getLFCrossSliceBoundaryFlag     ()                    { return m_bLFCrossSliceBoundaryFlag;   }

#if SAO
  Void      setUseSAO                  (Bool bVal)     {m_bUseSAO = bVal;}
  Bool      getUseSAO                  ()              {return m_bUseSAO;}
#endif
#if TILES
#if NONCROSS_TILE_IN_LOOP_FILTERING
  Void  setTileBehaviorControlPresentFlag        ( Int i )             { m_iTileBehaviorControlPresentFlag = i;    }
  Int   getTileBehaviorControlPresentFlag        ()                    { return m_iTileBehaviorControlPresentFlag; }
  Void  setLFCrossTileBoundaryFlag               ( Bool   bValue  )    { m_bLFCrossTileBoundaryFlag = bValue; }
  Bool  getLFCrossTileBoundaryFlag               ()                    { return m_bLFCrossTileBoundaryFlag;   }
#endif
  Void  setColumnRowInfoPresent        ( Int i )           { m_iColumnRowInfoPresent = i; }
  Int   getColumnRowInfoPresent        ()                  { return m_iColumnRowInfoPresent; }
  Void  setUniformSpacingIdr           ( Int i )           { m_iUniformSpacingIdr = i; }
  Int   getUniformSpacingIdr           ()                  { return m_iUniformSpacingIdr; }
  Void  setTileBoundaryIndependenceIdr ( Int i )           { m_iTileBoundaryIndependenceIdr = i; }
  Int   getTileBoundaryIndependenceIdr ()                  { return m_iTileBoundaryIndependenceIdr; }
  Void  setNumColumnsMinus1            ( Int i )           { m_iNumColumnsMinus1 = i; }
  Int   getNumColumnsMinus1            ()                  { return m_iNumColumnsMinus1; }
  Void  setColumnWidth ( char* str )
  {
    char *columnWidth;
    int  i=0;
    Int  m_iWidthInCU = ( m_iSourceWidth%g_uiMaxCUWidth ) ? m_iSourceWidth/g_uiMaxCUWidth + 1 : m_iSourceWidth/g_uiMaxCUWidth;

    if( m_iUniformSpacingIdr == 0 && m_iNumColumnsMinus1 > 0 )
    {
      m_puiColumnWidth = new UInt[m_iNumColumnsMinus1];

      columnWidth = strtok(str, " ,-");
      while(columnWidth!=NULL)
      {
        if( i>=m_iNumColumnsMinus1 )
        {
          printf( "The number of columns whose width are defined is larger than the allowed number of columns.\n" );
          exit( EXIT_FAILURE );
        }
        *( m_puiColumnWidth + i ) = atoi( columnWidth );
        printf("col: m_iWidthInCU= %4d i=%4d width= %4d\n",m_iWidthInCU,i,m_puiColumnWidth[i]); //AFU
        columnWidth = strtok(NULL, " ,-");
        i++;
      }
      if( i<m_iNumColumnsMinus1 )
      {
        printf( "The width of some columns is not defined.\n" );
        exit( EXIT_FAILURE );
      }
    }
  }
  UInt  getColumnWidth                 ( UInt columnidx )  { return *( m_puiColumnWidth + columnidx ); }
  Void  setNumRowsMinus1               ( Int i )           { m_iNumRowsMinus1 = i; }
  Int   getNumRowsMinus1               ()                  { return m_iNumRowsMinus1; }
  Void  setRowHeight (char* str)
  {
    char *rowHeight;
    int  i=0;
    Int  m_iHeightInCU = ( m_iSourceHeight%g_uiMaxCUHeight ) ? m_iSourceHeight/g_uiMaxCUHeight + 1 : m_iSourceHeight/g_uiMaxCUHeight;

    if( m_iUniformSpacingIdr == 0 && m_iNumRowsMinus1 > 0 )
    {
      m_puiRowHeight = new UInt[m_iNumRowsMinus1];

      rowHeight = strtok(str, " ,-");
      while(rowHeight!=NULL)
      {
        if( i>=m_iNumRowsMinus1 )
        {
          printf( "The number of rows whose height are defined is larger than the allowed number of rows.\n" );
          exit( EXIT_FAILURE );
        }
        *( m_puiRowHeight + i ) = atoi( rowHeight );
        printf("row: m_iHeightInCU=%4d i=%4d height=%4d\n",m_iHeightInCU,i,m_puiRowHeight[i]); //AFU
        rowHeight = strtok(NULL, " ,-");
        i++;
      }
      if( i<m_iNumRowsMinus1 )
      {
        printf( "The height of some rows is not defined.\n" );
        exit( EXIT_FAILURE );
     }
    }
  }
  UInt  getRowHeight                   ( UInt rowIdx )     { return *( m_puiRowHeight + rowIdx ); }
  Void  xCheckGSParameters();
#if TILES_DECODER
  Int  getTileLocationInSliceHeaderFlag ()                 { return m_iTileLocationInSliceHeaderFlag; }
  Void setTileLocationInSliceHeaderFlag ( Int iFlag )      { m_iTileLocationInSliceHeaderFlag = iFlag;}
  Int  getTileMarkerFlag              ()                 { return m_iTileMarkerFlag;              }
  Void setTileMarkerFlag              ( Int iFlag )      { m_iTileMarkerFlag = iFlag;             }
  Int  getMaxTileMarkerEntryPoints    ()                 { return m_iMaxTileMarkerEntryPoints;    }
  Void setMaxTileMarkerEntryPoints    ( Int iCount )     { m_iMaxTileMarkerEntryPoints = iCount;  }
  Double getMaxTileMarkerOffset       ()                 { return m_dMaxTileMarkerOffset;         }
  Void setMaxTileMarkerOffset         ( Double dCount )  { m_dMaxTileMarkerOffset = dCount;       }
#endif
#endif
#if OL_USE_WPP
  Void  setWaveFrontSynchro(Int iWaveFrontSynchro)       { m_iWaveFrontSynchro = iWaveFrontSynchro; }
  Int   getWaveFrontsynchro()                            { return m_iWaveFrontSynchro; }
  Void  setWaveFrontFlush(Int iWaveFrontFlush)           { m_iWaveFrontFlush = iWaveFrontFlush; }
  Int   getWaveFrontFlush()                              { return m_iWaveFrontFlush; }
  Void  setWaveFrontSubstreams(Int iWaveFrontSubstreams) { m_iWaveFrontSubstreams = iWaveFrontSubstreams; }
  Int   getWaveFrontSubstreams()                         { return m_iWaveFrontSubstreams; }
#endif
  void setPictureDigestEnabled(bool b) { m_pictureDigestEnabled = b; }
  bool getPictureDigestEnabled() { return m_pictureDigestEnabled; }

#if !G1002_RPS
#if REF_SETTING_FOR_LD
  Void      setUseNewRefSetting    ( Bool b ) { m_bUseNewRefSetting = b;    }
  Bool      getUseNewRefSetting    ()         { return m_bUseNewRefSetting; }
#endif
#endif

#if WEIGHT_PRED
  Void      setUseWP               ( Bool  b )   { m_bUseWeightPred    = b;    }
  Void      setWPBiPredIdc         ( UInt u )    { m_uiBiPredIdc       = u;    }
  Bool      getUseWP               ()            { return m_bUseWeightPred;    }
  UInt      getWPBiPredIdc         ()            { return m_uiBiPredIdc;       }
#endif
#if SCALING_LIST
  Void      setUseScalingListId    ( Int  u )    { m_useScalingListId       = u;   }
  Int       getUseScalingListId    ()            { return m_useScalingListId;      }
  Void      setScalingListFile     ( char*  pch ){ m_scalingListFile     = pch; }
  char*     getScalingListFile     ()            { return m_scalingListFile;    }
#endif

#if NO_TMVP_MARKING
  Void      setEnableTMVP ( Bool b ) { m_bEnableTMVP = b;    }
  Bool      getEnableTMVP ()         { return m_bEnableTMVP; }
#endif
};

//! \}

#endif // !defined(AFX_TENCCFG_H__6B99B797_F4DA_4E46_8E78_7656339A6C41__INCLUDED_)
