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
