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

