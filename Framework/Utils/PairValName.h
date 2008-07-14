/*
 *       This file is part of NWFramework.
 *       Copyright (c) InCrew Software and Others.
 *       (See the AUTHORS file in the root of this distribution.)
 *
 *       NWFramework is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *
 *       NWFramework is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 * 
 *       You should have received a copy of the GNU General Public License
 *       along with NWFramework; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
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
