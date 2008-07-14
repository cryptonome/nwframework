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

#include "Form.h"
#include "GUI.h"
#include "FormControl.h"
#include "ControlFactory.h"
#include "SystemUtils.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Form::Form() :
    mFormControl(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Form::init(GUI* _gui, const char* _name, GUIControl* _parent, const char* _fileName, const char* _context)
{
    bool bOK = true;

    if (!isOk())
    {
        mFormControl = 0;
        bool fileLoaded = false;

        DataScript ds;
        if ( _fileName ) 
        {
            fileLoaded = ds.loadFile(_fileName);
            if ( !fileLoaded )
                SystemUtils::messageBox("Error loading the Form(%s)",_fileName);
        }

        if ( bOK  )
        {
            mFormControl = _gui->getControlFactory()->create<FormControl>(_name,_parent);
            bOK = mFormControl != 0;
        }

        if ( bOK )
            mFormControl->setParentToDelete(this);

        if ( bOK && _fileName && fileLoaded )
        {
            bOK = mFormControl->load(ds);
            ASSERT(bOK); bOK;
            if ( bOK && _context )
                mFormControl->setDataObjContext(_context);
            if ( bOK )
                mFormControl->internalCreateAdapter(true);
        }

        /*if (bOK && _parent)
            _parent->addChild(mFormControl);*/

        if (bOK)
            bOK = Inherited::init(_gui,_name,0);
    }

    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::done()
{
    if (isOk())
    {
        if ( mFormControl )
        {
            mFormControl->setParentToDelete(0);
            DISPOSE(mFormControl);
        }
        Inherited::done();
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId Form::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId Form::staticGetControlType()
{ 
    return "Form"; 
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::addChild(GUIControl* _child)
{
    mFormControl->addChild(_child);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int Form::getNumChilds() const
{
    return mFormControl ? mFormControl->getNumChilds() : 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl* Form::getChild(int _index) const
{
    return mFormControl->getChild(_index);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
const char* Form::getName() const
{
    return mFormControl->getName();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::setName(const char* _name)
{
    mFormControl->setName(_name);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::setPosition (int _x, int _y)
{
    mFormControl->setPosition(_x,_y);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::getPosition (int& _x, int& _y) const
{
    mFormControl->getPosition(_x,_y);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::setSize (int _x, int _y)
{
    mFormControl->setSize(_x,_y);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::getSize (int& _x, int& _y) const
{
    mFormControl->getSize(_x,_y);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::setVisible(bool _visible)
{
    mFormControl->setVisible(_visible);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Form::getVisible() const
{
    return mFormControl->getVisible();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::setEnabled(bool _enabled)
{
    mFormControl->setEnabled(_enabled);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Form::getEnabled() const
{
    return mFormControl->getEnabled();
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIControl* Form::getControl()
{
    return mFormControl->getControl();
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::formControlRemoved()
{
    mFormControl = 0;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Form::registerEventsFromControl(const char* _name)
{
    GUIControl* control = getChildByName("new button1",true);

    return control ? registerEventsFromControl(control) : false;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Form::registerEventsFromControl(GUIControl* _control)
{
    bool ok = true;

    // Button
    Button* button = _control->convertTo<Button>(false);
    if ( button )
        button->setButtonEventHandler(this);

    // CheckBox
    CheckBox* checkbox = _control->convertTo<CheckBox>(false);
    if ( checkbox )
        checkbox->setCheckBoxEventHandler(this);

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::registerEventsFromButtons()
{
    for ( int i = 0 ; i < getNumChilds() ; ++i )
    {
        Button* button = getChild(i)->convertTo<::Button>(false);
        if ( button )
            button->setButtonEventHandler(this);
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Form::removeChild(GUIControl* _control)
{
    mFormControl->removeChild(_control);
}


