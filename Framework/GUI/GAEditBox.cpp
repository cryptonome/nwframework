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
#include "GAEditBox.h"
#include "StrUtils.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxString::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxString::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    mData.set(_editBox->getText());
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxString::update(const char * _contextName, const char * _objName)
{
    mControl->setText(mData.get().c_str(), false);
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxInt::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxInt::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    int val = StrUtils::aToI(_editBox->getText());
    mData.set(val);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxInt::update(const char * _contextName, const char * _objName)
{
    std::string str = StrUtils::iToA(mData.get());
    mControl->setText(str.c_str(), false);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GAEditBoxNWIP::initAdapter()
{
    mControl->setEditBoxEventHandler(this);
    mControl->setText("1.2.3.4", false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxNWIP::editBoxModified(::EditBox* _editBox)
{ 
    ASSERT(mControl == _editBox);

    NWIP ip = mData.get();
    ip.setFromStr(_editBox->getText());
    mData.set(ip);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GAEditBoxNWIP::update(const char * _contextName, const char * _objName)
{
    NWIP ip = mData.get();
    mControl->setText(ip.getAsStr().c_str(), false);
}
