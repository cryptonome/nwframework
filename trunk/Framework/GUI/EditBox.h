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
