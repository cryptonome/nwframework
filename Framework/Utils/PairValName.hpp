/* 
 * Copyright 2007-2008 InCrew Software, All Rights Reserved
 *
 * This file may be used or modified without the need for a license.
 *
 * Redistribution of this file in either its original form, or in an
 * updated form may be done under the terms of the GNU LIBRARY GENERAL
 * PUBLIC LICENSE.  If this license is unacceptable to you then you
 * may not redistribute this work.
 * 
 * See the file COPYING.GPL for details.
 */
//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
PairValName<T>::PairValName()
{
    mPairs.clear();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
PairValName<T>::~PairValName()
{
    clearPairs();
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void PairValName<T>::addPair(const T val, const char* _name)
{
    ASSERT(getNameFromVal(val,false) == 0); T check;
    ASSERT(!getValFromName(_name,check)); check;

    Pair* pair = NEW Pair;
    pair->name = _name;
    pair->val = val;
    mPairs.push_back(pair);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void PairValName<T>::clearPairs()
{
    for ( std::vector<Pair*>::iterator it = mPairs.begin() ; it != mPairs.end() ; ++it )
    {
        Pair* pair = *it;
        DISPOSE(pair);
    }
    mPairs.clear();
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
bool PairValName<T>::getValFromName(const char* _name, T& val_) const
{
    bool found = false;

    for ( std::vector<Pair*>::const_iterator it = mPairs.begin() ; !found && it != mPairs.end() ; ++it )
    {
        const Pair* pair = *it;
        if ( strcmp(_name,pair->name.c_str()) == 0 )
        {
            found = true;
            val_ = pair->val;
        }
    }

    return found;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* PairValName<T>::getNameFromVal(const T& _val, bool _mustExist) const
{
    const char* str = 0;

    for ( std::vector<Pair*>::const_iterator it = mPairs.begin() ; str == 0 && it != mPairs.end() ; ++it )
    {
        const Pair* pair = *it;
        if ( _val == pair->val )
            str = pair->name.c_str();
    }

    ASSERT(str != 0 || !_mustExist);

    return str;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
T PairValName<T>::getPairVal(int _index) const
{
    ASSERT(_index >= 0 && _index < getNumPairs());
    return mPairs[_index]->val;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const char* PairValName<T>::getPairName(int _index) const
{
    ASSERT(_index >= 0 && _index < getNumPairs());
    return mPairs[_index]->name.c_str();
}

