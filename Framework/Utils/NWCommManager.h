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
#ifndef NWCOMM_MANAGER_H
#define NWCOMM_MANAGER_H

#include "NWIP.h"
#include "StrId.h"
#include <vector>

class NWSocket;
class NWServerSocket;
class NWClientSocket;

struct NWCommManagerNotificationCallback
{
    virtual void networkMsgNotification() = 0;
};

class NWCommManager
{
public:
    enum eReserve
    {
        eReserve_Servers = 16,
        eReserve_Clients = 16
    };

    // singleton management
    static bool staticInit(int _serversReserve=eReserve_Servers, int _clientsReserve=eReserve_Clients);
    static void staticShutdown();
    static inline NWCommManager * instance();

    // --- ---
    bool init(int _serversReserve, int _clientsReserve);
    void shutdown();

    inline void setNotificationCallback(NWCommManagerNotificationCallback * _callback);

    NWServerSocket * createServer(NWIP _serverIp, int _listenPort);
    NWClientSocket * createClient(NWIP _serverIp, int _serverPort);

    void destroyServer(NWServerSocket * _server);
    void destroyClient(NWClientSocket * _client);

    int getNumIntefaces();
    NWIP getInterface(int _idx);

    void dispatchNetworkMessages();

    void sendNotification();

private:
    struct sSockNode
    {
        NWSocket * mSocket;
    };

    static NWCommManager * mInstance;

    bool mInitd;
    std::vector<sSockNode> mServerList;
    std::vector<sSockNode> mClientList;

    NWCommManagerNotificationCallback * mNWCommManagerNotificationCallback;

    NWCommManager();
    ~NWCommManager();
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ inline NWCommManager * NWCommManager::instance()
{
    return mInstance;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline void NWCommManager::setNotificationCallback(NWCommManagerNotificationCallback * _callback)
{
    mNWCommManagerNotificationCallback = _callback;
}

#endif // NWCOMM_MANAGER_H
