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

#include "GUIAdapterFactory.h"
#include "GUIControl.h"
#include "GUIAdapterCreator.h"
#include "GUI.h"
#include "GACheckBox.h"
#include "GAEditBox.h"
#include "GAComboBox.h"
#include "GATabControl.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIAdapterFactory::GUIAdapterFactory() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GUIAdapterFactory::init(GUI * _gui)
{
    bool bOK = true;

    if (!isOk())
    {
        mGUI = _gui;

        registerBaseGUIAdapterCreators();

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::done()
{
    if (isOk())
    {
        for ( std::list<IGUIAdapterCreator*>::iterator it = mCreators.begin() ; it != mCreators.end() ; ++it )
        {
            IGUIAdapterCreator* creator = *it;
            DISPOSE(creator);
        }
        mCreators.clear();

        mInit = false;
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::registerAdapterCreator(IGUIAdapterCreator* _creator)
{
    mCreators.push_back(_creator);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IGUIAdapter* GUIAdapterFactory::createAdapter(GUIControl* _control, StrId _dataTypeString, StrId _dataObjContext, StrId _dataObjName)
{
    IGUIAdapter* guiAdapter = 0;

    for ( std::list<IGUIAdapterCreator*>::iterator it = mCreators.begin() ; guiAdapter == 0 && it != mCreators.end() ; ++it )
    {
        IGUIAdapterCreator* creator = *it;
        if ( _control->getControlType() == creator->getControlType() &&
             _dataTypeString == creator->getDataType() )
        {
             guiAdapter = creator->create(_control, mGUI->getDataProvider(), _dataObjContext, _dataObjName);

             ASSERT(guiAdapter);
             _control->setAdapter(guiAdapter);
        }
    }

    return guiAdapter;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::registerBaseGUIAdapterCreators()
{
    REGISTER_ADAPTER_CREATOR(this, GACheckBoxBool);
    REGISTER_ADAPTER_CREATOR(this, GACheckBoxString);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxString);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxInt);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxNWIP);
    REGISTER_ADAPTER_CREATOR(this, GAComboBoxInt);
    REGISTER_ADAPTER_CREATOR(this, GAComboBoxString);
    REGISTER_ADAPTER_CREATOR(this, GATabControlString);
}
