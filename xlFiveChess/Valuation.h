//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Valuation.h
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __VALUATION_H_DDAACB0E_04CD_4F20_8D01_ADC295196245_INCLUDED__
#define __VALUATION_H_DDAACB0E_04CD_4F20_8D01_ADC295196245_INCLUDED__


#include "FiveModel.h"

namespace Valuation
{
    double EvalLine(const LineInfo &li);
    double EvalChessboard(const ChessData &data, ChessmanColor colorToEval);
    ChessmanColor FindLine(const ChessData &data, int nCount, ChessmanColor colorToFind = ChessmanColor_None, bool bFindAll = false, bool bAllowHole = false, LineInfoCollection *pResult = nullptr);
}

#endif // #ifndef __VALUATION_H_DDAACB0E_04CD_4F20_8D01_ADC295196245_INCLUDED__
