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
#include "PchGUI.h"
#include "GAComboBox.h"
#include "StrUtils.h"
#include "NWTypeUtils.h"

//********************************************************************
//
//********************************************************************
GAComboBoxInt::~GAComboBoxInt()
{
    mDataList.removeNotificationCallback(this);
    mDataList.shutdown();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAComboBoxInt::initAdapter()
{
    std::string dataObjName = getDataObjName();
    dataObjName = dataObjName + "List";

    REG_START(mDataProvider, getDataObjContext());
        REG_CLI_OBJ(dataObjName.c_str(), mDataList);
    REG_END();
    mDataList.addNotificationCallback(this);
    mControl->setComboBoxEventHandler(this);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxInt::comboBoxSelectionChanged(::ComboBox* _comboBox)
{ 
    ASSERT(mControl == _comboBox);

    int index = mControl->getSelectedItem();
    if ( index != -1 )
    {
        std::string text;
        void* userData = 0;
        mControl->getItemData(index,text,userData);
        int val = NWTypeUtils::voidPtrToInt(userData);
        mData.set(val);
    }
    else
        updateSelection();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxInt::update(const char * _contextName, const char * _objName)
{
    // Data List (data contents changed)
    if ( strcmp(_objName,mDataList.getObjName()) == 0 )
    {
        NWItemsList<int> itemsList = mDataList.get();
        
        mControl->clearItemsList();
        int items = itemsList.getNumItems();
        for ( int i = 0 ; i < items ; ++i )
        {
            int val = itemsList.getItem(i);
            mControl->addItem(StrUtils::iToA(val).c_str(), NWTypeUtils::intToVoidPtr(val));
        }

        updateSelection();
    }
    // Data (item selected changed)
    else
    {
        ASSERT(strcmp(_objName,getDataObjName()) == 0);
        updateSelection();
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxInt::updateSelection()
{
    int val = mData.get();
    int index = mControl->getItemIndexFromUserData(NWTypeUtils::intToVoidPtr(val));
    mControl->selectItem(index);
}



//********************************************************************
//
//********************************************************************
GAComboBoxString::~GAComboBoxString()
{
    mDataList.removeNotificationCallback(this);
    mDataList.shutdown();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAComboBoxString::initAdapter()
{
    std::string dataObjName = getDataObjName();
    dataObjName = dataObjName + "List";

    REG_START(mDataProvider, getDataObjContext());
        REG_CLI_OBJ(dataObjName.c_str(), mDataList);
    REG_END();
    mDataList.addNotificationCallback(this);
    mControl->setComboBoxEventHandler(this);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxString::comboBoxSelectionChanged(::ComboBox* _comboBox)
{ 
    ASSERT(mControl == _comboBox);

    int index = mControl->getSelectedItem();
    if ( index != -1 )
    {
        std::string text;
        void* userData = 0;
        mControl->getItemData(index,text,userData);
        int val = NWTypeUtils::voidPtrToInt(userData);

        NWItemsList<std::string> itemsList = mDataList.get();
        if ( val >= 0 && val < itemsList.getNumItems() )
            mData.set(itemsList.getItem(val));
        else
            mData.set("");  // Unknown value
    }
    else
        updateSelection();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxString::update(const char * _contextName, const char * _objName)
{
    // Data List (data contents changed)
    if ( strcmp(_objName,mDataList.getObjName()) == 0 )
    {
        NWItemsList<std::string> itemsList = mDataList.get();
        
        mControl->clearItemsList();
        int items = itemsList.getNumItems();
        for ( int i = 0 ; i < items ; ++i )
        {
            std::string str = itemsList.getItem(i);
            mControl->addItem(str.c_str(), NWTypeUtils::intToVoidPtr(i));
        }

        updateSelection();
    }
    // Data (item selected changed)
    else
    {
        ASSERT(strcmp(_objName,getDataObjName()) == 0);
        updateSelection();
    }
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAComboBoxString::updateSelection()
{
    std::string val = mData.get();
    NWItemsList<std::string> itemsList = mDataList.get();
    int index = -1;
    for ( int i = 0 ; index == -1 && i < itemsList.getNumItems() ; ++i )
    {
        if ( strcmp(itemsList.getItem(i).c_str(),val.c_str()) == 0 )
            index = i;
    }
    mControl->selectItem(index);
}
