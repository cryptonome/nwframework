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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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


