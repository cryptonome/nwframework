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
#include "PchGUI.h"

#include "ComboBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ComboBox::ComboBox() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool ComboBox::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mComboBoxEventHandler = 0;

        bOK = Inherited::init(_gui, _name, _parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void ComboBox::done()
{
    if (isOk())
    {
        for ( std::vector<Item*>::iterator it = mItems.begin() ; it != mItems.end() ; ++it )
        {
            Item* item = *it;
            DISPOSE(item);
        }
        mItems.clear();

        Inherited::done();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId ComboBox::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId ComboBox::staticGetControlType() 
{ 
    return StrId("ComboBox");
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int ComboBox::addItem(const char* _string, void* _userData)
{
    int index = (int)mItems.size();

    Item* item = NEW Item;
    item->mUserData = _userData;
    item->mString = _string;
    mItems.push_back(item);

    internalAddItem(_string);

    return index;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int ComboBox::getNumItems() const
{
    return (int)mItems.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void ComboBox::clearItemsList()
{
    for ( std::vector<Item*>::iterator it = mItems.begin() ; it != mItems.end() ; ++it )
    {
        Item* item = *it;
        DISPOSE(item);
    }
    mItems.clear();

    internalClearItemsList();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void ComboBox::getItemData(int _index, std::string& string_, void*& userData_)
{
    ASSERT(_index >= 0 && _index < getNumItems());
    Item* item = mItems[_index];

    string_ = item->mString;
    userData_ = item->mUserData;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int ComboBox::getItemIndexFromUserData(void* _userData)
{
    int index = -1;

    int items = getNumItems();
    for ( int i = 0 ; index == -1 && i < items ; ++i )
    {
        Item* item = mItems[i];
        if ( item->mUserData == _userData )
            index = i;
    }

    return index;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void ComboBox::sendEventComboBoxSelectionChanged()
{
    if ( mComboBoxEventHandler )
        mComboBoxEventHandler->comboBoxSelectionChanged(this);
}

