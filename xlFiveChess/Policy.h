//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Policy.h
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __POLICY_H_E4AC519F_30F6_447A_B737_DADAA6CCD9BE_INCLUDED__
#define __POLICY_H_E4AC519F_30F6_447A_B737_DADAA6CCD9BE_INCLUDED__


#include "FiveModel.h"

class Policy
{
public:
    static Point FindNextMove(const ChessData &data, ChessmanColor currentTurn);

private:
    static Point FindNextMove(LineInfoCollection::Iterator &it, LineInfoCollection::Iterator &itEnd, int nMinCount);
};

#endif // #ifndef __POLICY_H_E4AC519F_30F6_447A_B737_DADAA6CCD9BE_INCLUDED__
