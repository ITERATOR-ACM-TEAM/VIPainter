/**
 * Copyright (C) 2017 VIPainter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef SWAPQUEUE_H
#define SWAPQUEUE_H

#include <algorithm>

template<class TN,int SWAPSIZE>
class SwapQueue
{
private:
    TN swp[SWAPSIZE];
    int left,right,now;
public:
    SwapQueue();
    void push(const TN &x);
    void push(TN &&x);
    TN redo();
    TN undo();
    bool atFirst();
    bool atLast();
    bool changed();
};

template<class TN,int SWAPSIZE>
SwapQueue<TN,SWAPSIZE>::SwapQueue():left(0),right(0),now(0)
{
}

template<class TN,int SWAPSIZE>
void SwapQueue<TN,SWAPSIZE>::push(const TN &x)
{
    left=std::max(left,now-SWAPSIZE+1);
    swp[(now++)%SWAPSIZE]=x;
    right=now;
}

template<class TN,int SWAPSIZE>
void SwapQueue<TN,SWAPSIZE>::push(TN &&x)
{
    left=std::max(left,now-SWAPSIZE+1);
    swp[(now++)%SWAPSIZE]=std::move(x);
    right=now;
}

template<class TN,int SWAPSIZE>
TN SwapQueue<TN,SWAPSIZE>::redo()
{
    if(!atLast())now++;
    return swp[(now-1)%SWAPSIZE];
}

template<class TN,int SWAPSIZE>
TN SwapQueue<TN,SWAPSIZE>::undo()
{
    if(!atFirst())now--;
    return swp[(now-1)%SWAPSIZE];
}

template<class TN,int SWAPSIZE>
bool SwapQueue<TN,SWAPSIZE>::atFirst()
{
    return now-left<=1;
}

template<class TN,int SWAPSIZE>
bool SwapQueue<TN,SWAPSIZE>::atLast()
{
    return now>=right;
}

template<class TN,int SWAPSIZE>
bool SwapQueue<TN,SWAPSIZE>::changed()
{
    return now>1;
}


#endif // SWAPQUEUE_H
