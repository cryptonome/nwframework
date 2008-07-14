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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
