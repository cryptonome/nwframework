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

#include "GACheckBox.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GACheckBoxBool::initAdapter()
{
    mControl->setCheckBoxEventHandler(this);
    mControl->setChecked(mData.get(), false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxBool::checkBoxModified(::CheckBox* _checkbox)
{ 
    ASSERT(mControl == _checkbox);

    bool checked = _checkbox->getChecked();
    mData.set(checked);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxBool::update(const char * _contextName, const char * _objName)
{
    mControl->setChecked(mData.get(), false);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GACheckBoxString::initAdapter()
{
    mControl->setCheckBoxEventHandler(this);
    bool checked = strcmp(mData.get().c_str(),mControl->getDataObjParams()) == 0;
    mControl->setChecked(checked, false);
    return true;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxString::checkBoxModified(::CheckBox* _checkbox)
{ 
    ASSERT(mControl == _checkbox);

    bool checked = _checkbox->getChecked();
    if ( checked )
        mData.set(mControl->getDataObjParams());
    else
        mData.set(mData.get());
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GACheckBoxString::update(const char * _contextName, const char * _objName)
{
    bool checked = strcmp(mData.get().c_str(),mControl->getDataObjParams()) == 0;
    mControl->setChecked(checked, false);
}
