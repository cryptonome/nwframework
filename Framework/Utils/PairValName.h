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
#ifndef PAIRVALNAME_H_
#define PAIRVALNAME_H_

#include <string>
#include <vector>
#include "NWItemsList.h"

//********************************************************************
//
//********************************************************************
template <typename T>
class PairValName
{
public:
    PairValName  ();
    ~PairValName ();

    void addPair(const T val, const char* _name);
    void clearPairs();
    
    bool getValFromName(const char* _name, T& val_) const;
    const char* getNameFromVal(const T& _val, bool _mustExist = true) const;

    int getNumPairs() const { return (int)mPairs.size(); }
    T getPairVal(int _index) const;
    const char* getPairName(int _index) const;

    void fillItemsList(NWItemsList<std::string>& il_)
    {
        il_.clearItems();
        for ( int i = 0 ; i < getNumPairs() ; ++i )
        {
            il_.addItem(getPairName(i));
        }
    }

    NWItemsList<std::string> getItemsList()
    {
        NWItemsList<std::string> il;
        fillItemsList(il);
        return il;
    }

private:
    struct Pair
    {
        T val;
        std::string name;
    };
    std::vector<Pair*> mPairs;
};


#include "PairValName.hpp"

#endif
