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

