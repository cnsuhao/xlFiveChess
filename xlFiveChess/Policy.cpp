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
#include <xl/Common/Algorithm/xlSort.h>
#include "Valuation.h"


Point Policy::FindNextMove(LineInfoCollection ours, LineInfoCollection theirs)
{
    if (!ours.Empty())
    {
        xl::Sort::QuickSort(&ours[0], ours.Size(), xl::Function<bool(const LineInfo &, const LineInfo &)>(Valuation::LineComparor));
    }
    if (!theirs.Empty())
    {
        xl::Sort::QuickSort(&theirs[0], theirs.Size(), xl::Function<bool(const LineInfo &, const LineInfo &)>(Valuation::LineComparor));
    }

    LineInfoCollection::Iterator itOurs = ours.Begin();
    LineInfoCollection::Iterator itTheirs = theirs.Begin();

    if ((itOurs != ours.End() && itOurs->Count >= 5) || (itTheirs != theirs.End() && itTheirs->Count >= 5))
    {
        return INVALID_POSITION;
    }

    for (int i = 4; i > 0; --i)
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
        if (!it->Blocked.HeadBlocked || !it->Blocked.TailBlocked)
        {
            LOG_LINE(*it);
            return it->Blocked.HeadBlocked ? it->Position + DirectionDef[it->Direction] * it->Count : it->Position - DirectionDef[it->Direction];
        }
        ++it;
    }

    return INVALID_POSITION;
}
