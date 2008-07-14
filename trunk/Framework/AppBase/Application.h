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
