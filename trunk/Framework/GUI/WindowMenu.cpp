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

#include "WindowMenu.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu::WindowMenu() :
    mInit(false),
    mNumId(-1)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool WindowMenu::init()
{
    return init("main menu","");
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool WindowMenu::init(const char* _name, const char* _id)
{
    bool bOK = true;

    if (!isOk())
    {
        mName = _name;
        mId = _id;
        mNumId = -1;

        mInit = true;
    }
    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void WindowMenu::done()
{
    if (isOk())
    {
        int items = getNumItems();
        for ( int i = 0 ; i < items ; ++i )
        {
            WindowMenu* item = getItem(i);
            DISPOSE(item);
        }
        mItems.clear();
        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::addItem(const char* _name, const char* _id)
{
    WindowMenu* item = NEW WindowMenu();
    item->init(_name, _id);
    
    mItems.push_back(item);

    return item;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::getItem(int _index)
{
    WindowMenu* item = 0;

    ASSERT(_index >= 0 && _index < getNumItems());

    item = mItems[_index];

    return item;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
WindowMenu* WindowMenu::getItemByNumId(int _numId)
{
    WindowMenu* item = 0;

    if ( mNumId == _numId )
        item = this;

    if ( item == 0 )
    {
        int items = getNumItems();
        for ( int i = 0 ; item == 0 && i < items ; ++ i )
        {
            item = getItem(i)->getItemByNumId(_numId);
        }
    }

    return item;
}