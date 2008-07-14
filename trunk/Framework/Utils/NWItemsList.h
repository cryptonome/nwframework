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
#ifndef NWITEMSLIST_H_
#define NWITEMSLIST_H_

#include <vector>

//********************************************************************
//
//********************************************************************
template <typename T>
class NWItemsList
{
public:
    NWItemsList  ();
    NWItemsList  (const NWItemsList<T>& il);
    ~NWItemsList ();

    NWItemsList<T>& operator=(const NWItemsList<T>& il);

    void addItem(const T& _val);
    const T& getItem(int _index) const;
    int getNumItems() const;
    void clearItems();

private:

    void copy(const NWItemsList<T>& il);

    std::vector<T> mItems;
};

#include "NWItemsList.hpp"

#endif
