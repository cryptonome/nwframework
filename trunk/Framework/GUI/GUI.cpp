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

