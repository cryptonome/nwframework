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
#include "PchNomosVE.h"

#include "DataAdapters.h"
#include "CheckBox.h"

//****************************************************************************
// bool -> Checkbox
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
AdapterBoolCheckBox::AdapterBoolCheckBox() :
    mInitd(false),
    mBoolVar(NULL),
    mCheckBox(NULL)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
AdapterBoolCheckBox::~AdapterBoolCheckBox()
{
    ASSERT(!mInitd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool AdapterBoolCheckBox::init(NWSvcDataObj<bool> * _boolVar, CheckBox * _checkBox)
{
    bool retVal = false;

    if(!mInitd)
    {
        ASSERT(_boolVar && _checkBox);

        mBoolVar = _boolVar;
        mCheckBox = _checkBox;
        mBoolVar->addNotificationCallback(this);

        retVal = true;
        mInitd = true;
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void AdapterBoolCheckBox::shutdown()
{
    if(mInitd)
    {
        mBoolVar->removeNotificationCallback(this);
        mBoolVar = NULL;
        mCheckBox = NULL;
        mInitd = false;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void AdapterBoolCheckBox::update(const char * _contextName, const char * _objName)
{
    if(mInitd)
    {
        mCheckBox->setChecked(mBoolVar->get());
    }
}

//****************************************************************************
//
//****************************************************************************
