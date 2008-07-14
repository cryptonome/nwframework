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
