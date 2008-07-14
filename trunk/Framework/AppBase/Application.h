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
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Window.h"
#include <list>
#include "MsgMgr.h"
#include "NWCommManager.h"

class NWSvcDataServer;
class GUI;
class NWCommManager;

//********************************************************************
//
//********************************************************************
class AppUserNotification
{
public:
    virtual void notification(int _id) = 0;
};

//********************************************************************
//
//********************************************************************
class Application : public WindowEventHandler, public AppUserNotification, public MsgMgrNotificationCallback, public NWCommManagerNotificationCallback
{
public:
    virtual ~Application () { }

    virtual bool        init                  (const char* _cmdLine);
    virtual void        done                  ();

    void                quit                  ();

    void addUserNotification(AppUserNotification* _notification, int _id);
    void sendUserNotification(int _id);

    NWSvcDataServer * getServerDataProvider() { return mServerSvcDataProvider; }
    NWSvcDataServer * getClientDataProvider() { return mClientSvcDataProvider; }

    // User must implement this functions too
    static Application*     CreateApplication       ();
    static void             DestroyApplication      (Application* _app);

protected:
    GUI* mGUI;
private:
    // WindowEventHandler
    virtual void windowCloseButtonPressed(Window* _window) { quit(); }
    virtual void windowUserMsg(int _id);


    virtual void notification(int _id);
    virtual void msgMgrNotification();
    virtual void networkMsgNotification();

    struct UserNotification
    {
        AppUserNotification* mUserNotification;
        int mId;
        int mWindowUserMsgId;
    };
    UserNotification* getUserNotification(int _id);
    UserNotification* getUserNotificationByUserMsgId(int _id);
    std::list<UserNotification*> mUserNotifications;

    enum eAppMsgNotification
    {
        eAppMsgNotification_Null = 0,
        eAppMsgNotification_DispatchMessages,
        eAppMsgNotification_NetworkMessage
    };
    NWSvcDataServer * mServerSvcDataProvider; // local server for data publication
    NWSvcDataServer * mClientSvcDataProvider; // local client server for use with local client data objects used by the app
};

#endif
