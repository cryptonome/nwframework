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

#include "Window.h"
#include "NWCriticalSection.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Window::Window() : Inherited()
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Window::init(GUI* _gui, const char* _name, GUIControl* _parent)
{
    bool bOK = true;

    if (!isOk())
    {
        mWindowEventHandler = 0;
        mNextUserMsgId = 0;
        mcsUserMessages = NWCriticalSection::create();

        bOK = Inherited::init(_gui,_name,_parent);
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Window::done()
{
    if (isOk())
    {
        Inherited::done();

        NWCriticalSection::destroy(mcsUserMessages);
        for ( std::list<UserMessage*>::iterator it = mUserMessages.begin() ; it != mUserMessages.end() ; ++it )
        {
            UserMessage* um = *it;
            DISPOSE(um);
        }
        mUserMessages.clear();
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId Window::getControlType() const
{ 
    return staticGetControlType(); 
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
StrId Window::staticGetControlType() 
{ 
    return StrId("Window");
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Window::sendEventCloseButtonPressed()
{
    if ( mWindowEventHandler )
        mWindowEventHandler->windowCloseButtonPressed(this);
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int Window::addUserMsg()
{
    mcsUserMessages->enter();

    int id = mNextUserMsgId++;
    UserMessage* um = NEW UserMessage;
    um->mId = id;
    um->mPending = false;
    mUserMessages.push_back(um);

    mcsUserMessages->leave();

    return id;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Window::UserMessage* Window::getUserMessage(int _id) const
{
    UserMessage* um = 0;

    for ( std::list<UserMessage*>::const_iterator it = mUserMessages.begin() ; um == 0 && it != mUserMessages.end() ; ++it )
    {
        UserMessage* itUm = *it;
        if ( _id == itUm->mId )
            um = itUm;
    }

    return um;
}

