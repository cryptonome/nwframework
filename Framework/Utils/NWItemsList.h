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
