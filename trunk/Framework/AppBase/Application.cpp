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

#include "PchAppBase.h"

#include "Application.h"
#include "GUI.h"
#include "MsgMgr.h"
#include "MsgTypes.h"
#include "NWSvcDataServer.h"
#include "NWCommManager.h"

static bool gAppInit = false;
static const char * SVC_DATA_SERVER_NAME = "DATA_SERVER";
static const char * SVC_CLIENT_DATA_SERVER_NAME = "CLIENT_DATA_SERVER";

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool Application::init(const char* _cmdLine)
{
    bool ok = true;

    ASSERT(gAppInit == false);
    
    if ( ok )
    {
        mGUI = NEW GUI();
        ok = mGUI->init(_cmdLine);
    }

    if ( ok )
    {
        Window* mainWindow = mGUI->getMainWindow();
        mainWindow->setWindowEventHandler(this);
    }

    if ( ok )
    {
        ok = MsgMgr::init(0);
        ASSERT(ok);
        MsgMgr::instance()->regNotificationCallback(this);
        addUserNotification(this, eAppMsgNotification_DispatchMessages);
    }


    if ( ok )
    {
        mServerSvcDataProvider = NEW NWSvcDataServer();
        ok = mServerSvcDataProvider->init(SVC_DATA_SERVER_NAME, CommNodeId_DataServiceServer);
        ASSERT(ok);
    }

    if ( ok )
    {
        mClientSvcDataProvider = NEW NWSvcDataServer();
        ok = mClientSvcDataProvider->init(SVC_CLIENT_DATA_SERVER_NAME, CommNodeId_DataServiceClient);
        ASSERT(ok);
    }

    if( ok )
    {
        ok = NWCommManager::staticInit();
        ASSERT(ok);
        NWCommManager::instance()->setNotificationCallback(this);
    }

    if ( ok )
    {
        mGUI->setDataProvider(getClientDataProvider());
    }

    gAppInit = true;
    if ( !ok )
        done();

    return ok;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::done()
{
    ASSERT(gAppInit == true);
    
    NWCommManager::staticShutdown();

    mServerSvcDataProvider->shutdown();
    DISPOSE(mServerSvcDataProvider);

    mClientSvcDataProvider->shutdown();
    DISPOSE(mClientSvcDataProvider);

    MsgMgr::done();

    DISPOSE(mGUI);
    for ( std::list<UserNotification*>::iterator it = mUserNotifications.begin() ; it != mUserNotifications.end() ; ++it )
    {
        UserNotification* un = *it;
        DISPOSE(un);
    }
    mUserNotifications.clear();
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::addUserNotification(AppUserNotification* _notification, int _id)
{
    ASSERT(getUserNotification(_id) == 0);
    
    UserNotification* un = NEW UserNotification;
    un->mUserNotification = _notification;
    un->mId = _id;
    un->mWindowUserMsgId = mGUI->getMainWindow()->addUserMsg();
    
    mUserNotifications.push_back(un);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::sendUserNotification(int _id)
{
    UserNotification* un = getUserNotification(_id);
    ASSERT(un);

    if ( un )
        mGUI->getMainWindow()->sendUserMsg(un->mWindowUserMsgId);
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Application::UserNotification* Application::getUserNotification(int _id)
{
    UserNotification* un = 0;
    for ( std::list<UserNotification*>::iterator it = mUserNotifications.begin() ; un == 0 && it != mUserNotifications.end() ; ++it )
    {
        UserNotification* itUn = *it;
        if ( itUn->mId == _id )
            un = itUn;
    }
    return un;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
Application::UserNotification* Application::getUserNotificationByUserMsgId(int _id)
{
    UserNotification* un = 0;
    for ( std::list<UserNotification*>::iterator it = mUserNotifications.begin() ; un == 0 && it != mUserNotifications.end() ; ++it )
    {
        UserNotification* itUn = *it;
        if ( itUn->mWindowUserMsgId == _id )
            un = itUn;
    }
    return un;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::windowUserMsg(int _id)
{
    UserNotification* un = getUserNotificationByUserMsgId(_id);
    ASSERT(un);

    if ( un )
    {
        un->mUserNotification->notification(un->mId);
    }
}

//****************************************************************************
//
//****************************************************************************
void Application::notification(int _id)
{
    switch(_id)
    {
        case eAppMsgNotification_DispatchMessages:
        {
            MsgMgr::instance()->dispatchNotificationMessages();
            break;
        }

        case eAppMsgNotification_NetworkMessage:
        {
            NWCommManager::instance()->dispatchNetworkMessages();
            break;
        }
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::msgMgrNotification()
{
    sendUserNotification(eAppMsgNotification_DispatchMessages);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void Application::networkMsgNotification()
{
    sendUserNotification(eAppMsgNotification_NetworkMessage);
}
