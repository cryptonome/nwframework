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


#ifndef FORM_H_
#define FORM_H_

#include "GUIControl.h"
#include "Button.h"
#include "CheckBox.h"

class FormControl;

//********************************************************************
//
//********************************************************************
class Form : public GUIControl, public ButtonEventHandler, public CheckBoxEventHandler
{
public:
    Form   ();
    virtual    ~Form  ()                      { Form::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent, const char* _fileName, const char* _context);
    virtual void          done        ();

    // Type
    virtual StrId getControlType() const;
    static StrId staticGetControlType();

    // Child
    virtual void addChild(GUIControl* _child);
    virtual int getNumChilds() const;
    virtual GUIControl* getChild(int _index) const;

    // Name
    virtual void setName(const char* _name);
    virtual const char* getName() const;

    // Position/Size
    virtual void setPosition(int _x, int _y);
    virtual void getPosition(int& _x, int& _y) const;
    virtual void setSize(int _width, int _height);
    virtual void getSize(int& _width, int& _height) const;

    // Visible
    virtual void setVisible(bool _visible);
    virtual bool getVisible() const;

    // Enabled
    virtual void setEnabled(bool _enabled);
    virtual bool getEnabled() const;

    virtual GUIControl* getControl ();

protected:
    // GUIControl
    virtual void removeChild(GUIControl* _control);

    bool registerEventsFromControl(const char* _name);
    bool registerEventsFromControl(GUIControl* _control);
    void registerEventsFromButtons();

private:
    friend class FormControl;
    void formControlRemoved();

    typedef GUIControl Inherited;
    FormControl* mFormControl;
};


#endif
