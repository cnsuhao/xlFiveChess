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

// ���Զ��壬Խ�������Խ����
enum PolicyName
{
    Policy_SimpleAI,
    Policy_Random,
    Policy_Center,

    Policy_Count,
};

namespace Policy
{
    Point FindNextMove(PolicyName policy, const ChessData &data, ChessmanColor currentTurn);
};

#endif // #ifndef __POLICY_H_E4AC519F_30F6_447A_B737_DADAA6CCD9BE_INCLUDED__
