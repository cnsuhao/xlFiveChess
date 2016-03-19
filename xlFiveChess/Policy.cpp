//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Policy.cpp
//    Author:      Streamlet
//    Create Time: 2016-03-17
//    Description: 
//
//------------------------------------------------------------------------------


#include "Policy.h"
#include "Valuation.h"


Point Policy::FindNextMove(const ChessData &data, ChessmanColor currentTurn)
{
    XL_LOG_INFO_FUNCTION();

    LineInfoCollection ours, theirs;
    Valuation::FindLine(data, 1, currentTurn, true, true, &ours);
    Valuation::FindLine(data, 1, !currentTurn, true, true, &theirs);

    LineInfoCollection::Iterator itOurs = ours.Begin();
    LineInfoCollection::Iterator itTheirs = theirs.Begin();

    for (int i = CHESS_LENGTH - 1; i > 0; --i)
    {
        XL_LOG_INFO(L"Find our %d-line", i);
        Point pt = FindNextMove(itOurs, ours.End(), i);

        if (pt != INVALID_POSITION)
        {
            return pt;
        }

        XL_LOG_INFO(L"Find their %d-line", i);
        pt = FindNextMove(itTheirs, theirs.End(), i);

        if (pt != INVALID_POSITION)
        {
            return pt;
        }
    }

    return INVALID_POSITION;
}

Point Policy::FindNextMove(LineInfoCollection::Iterator &it, LineInfoCollection::Iterator &itEnd, int nMinCount)
{
    while (it != itEnd)
    {
        if (it->Count < nMinCount)
        {
            break;
        }
        if (it->Blank.HolePos > 0)
        {
            LOG_LINE(*it);
            return it->Position + DirectionDef[it->Direction] * it->Blank.HolePos;
        }
        if (it->Blank.HeadRemain > 0 && it->Blank.HeadRemain >= it->Blank.TailRemain)
        {
            LOG_LINE(*it);
            return it->Position - DirectionDef[it->Direction];
        }
        if (it->Blank.TailRemain > 0 && it->Blank.TailRemain > it->Blank.HeadRemain)
        {
            LOG_LINE(*it);
            return it->Position + DirectionDef[it->Direction] * (it->Count + (it->Blank.HolePos > 0 ? 1 : 0));
        }
        if (it->Blank.HeadRemain > 0 && it->Blank.TailRemain)
        {
            LOG_LINE(*it);
            return it->Position - DirectionDef[it->Direction];
        }

        ++it;
    }

    return INVALID_POSITION;
}

