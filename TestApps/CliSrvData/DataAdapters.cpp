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
