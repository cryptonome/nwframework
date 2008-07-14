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
//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
NWItemsList<T>::NWItemsList()
{
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
NWItemsList<T>::NWItemsList(const NWItemsList<T>& il)
{
    copy(il);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
NWItemsList<T>& NWItemsList<T>::operator=(const NWItemsList<T>& il)
{
    copy(il);
    return *this;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
NWItemsList<T>::~NWItemsList()
{
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void NWItemsList<T>::addItem(const T& _val)
{
    mItems.push_back(_val);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
const T& NWItemsList<T>::getItem(int _index) const
{
    ASSERT(_index >= 0 && _index < getNumItems());
    return mItems[_index];
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
int NWItemsList<T>::getNumItems() const
{
    return (int)mItems.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void NWItemsList<T>::clearItems()
{
    mItems.clear();
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template <typename T>
void NWItemsList<T>::copy(const NWItemsList<T>& il)
{
    clearItems();
    int items = il.getNumItems();
    for ( int i = 0 ; i < items ; ++i )
    {
        T val = il.getItem(i);
        addItem(val);
    }
}

