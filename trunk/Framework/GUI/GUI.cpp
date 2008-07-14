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

#include "GUI.h"
#include "Window.h"
#include "ControlFactory.h"
#include "Button.h"
#include "Form.h"
#include "GUIAdapterFactory.h"

const int GUI_VERSION_NUMBER = 1;

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUI::GUI() :
    mInit(false),
    mRoot(NULL),
    mControlFactory(NULL),
    mAdapterFactory(NULL),
    mDataProvider(NULL)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GUI::init(const char* _name)
{
    bool bOK = true;

    if (!isOk())
    {
        mControlFactory = ControlFactory::createFactory(this);
        mAdapterFactory = NEW GUIAdapterFactory();
        bOK = mAdapterFactory->init(this);

        // Create main window
        mRoot = mControlFactory->create<Window>(_name!=0?_name:"", 0);
        if ( mRoot )
            mRoot->setSize(800,600);

        mInit = true;
    }
    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUI::done()
{
    if (isOk())
    {
        DISPOSE(mRoot);

        if (mControlFactory)
        {
            ControlFactory::destroyFactory(mControlFactory);        
        }
        DISPOSE(mAdapterFactory);

        mInit = false;
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Window* GUI::getMainWindow()
{
    return mRoot;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Form* GUI::createForm(const char* _fileName, GUIControl* _parent)
{
    Form* form = NEW Form;

    if ( !form->init(this,_fileName?_fileName:"",_parent,_fileName,0) )
        DISPOSE(form);

    return form;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ControlFactory* GUI::getControlFactory()
{
    return mControlFactory;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIAdapterFactory* GUI::getAdapterFactory()
{
    return mAdapterFactory;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int GUI::getVersionNumber()
{
    return GUI_VERSION_NUMBER;
}

