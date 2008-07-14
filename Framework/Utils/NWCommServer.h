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
#ifndef NWCOMM_SERVER_H
#define NWCOMM_SERVER_H

#include "NWCommSocket.h"
#include "StrId.h"

class IServerListener
{
public:
    virtual bool onAccept(NWServerSocket * _socket) = 0;
    virtual void onClientConnected(int _clientId) = 0;
    virtual void onClientDisconnected(int _clientId) = 0;
    virtual void onClientData(int _clientIdFrom, MemBufferRef * _memBuff) = 0;
    virtual void onPing(int _clientId, int _ms) = 0;
};

class IClientListener
{
public:
    virtual void onConnect() = 0;
    virtual void onDisconnect() = 0;
    virtual void onServerData(int _msgType, MemBufferRef * _memBuff) = 0;
    virtual void onPing(int _ms) = 0;
};

//****************************************************************************
//
//****************************************************************************
class NWCommServer : public IServerSocketListener
{
public:
    struct ClientData
    {
        int mClientId;
        int mLastPing;
        int mAveragePing;
    };

    NWCommServer();
    virtual ~NWCommServer();

    bool init();
    void done();

    inline bool isInitd() const;

    bool start(StrId _serverName, int _port, int _softVer);
    void stop();

    int getNumClients() const;
    ClientData getClientData(int _index);

    int getServerPort();

    void sendMessage(int _clientId, int _msgType, MemBufferRef * _memBuff);
    void sendMessageAll(int _msgType, MemBufferRef * _memBuff);

    void ping(int _clientId);
    void pingAll();

    int getClientId(int _clientId) const;
    int getLastPingClient(int _clientId);
    int getAveragePingClient(int _clientId);

    void setPingInterval(int _ms);

    void addListener(IServerListener * _serverListener);
    void removeListener(IServerListener * _serverListener);

private:
    enum eMsgTypes
    {
        eMsgType_Ping = 0,
        eMsgType_User,
        eMsgType_Invalid
    };

    NWServerSocket * mSocket;
};

#endif // NWCOMM_SERVER_H
