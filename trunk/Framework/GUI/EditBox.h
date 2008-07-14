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
#ifndef EDITBOX_H_
#define EDITBOX_H_

#include "GUIControl.h"

class EditBox;

//********************************************************************
//
//********************************************************************
class EditBoxEventHandler
{
public:
    virtual ~EditBoxEventHandler() {}

    virtual void editBoxModified(::EditBox* _editBox) { }
};

//********************************************************************
//
//********************************************************************
class EditBox : public GUIControl
{
public:
    EditBox     ();
    virtual    ~EditBox    ()                      { EditBox::done(); }

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent);
    virtual void          done        ();

    // Text
    virtual void setText(const char* _text, bool _sendMofifiedEvent = true) = 0;
    virtual const char* getText() const = 0;

    void setEditBoxEventHandler(EditBoxEventHandler* _eventHandler) { mEditBoxEventHandler = _eventHandler; }

    virtual bool load(const DataScript& _data);
    virtual void save(DataScript& _data);

    virtual StrId getControlType() const;
    static StrId staticGetControlType();

protected:
    // Events
    void sendEventEditBoxModified();

private:
    typedef GUIControl Inherited;

    EditBoxEventHandler* mEditBoxEventHandler;
};

#endif
