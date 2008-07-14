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


#include "PchUtils.h"

#include "NWCommManager.h"
#include "NWCommSocket.h"

//****************************************************************************
//
//****************************************************************************
/*static*/ NWCommManager * NWCommManager::mInstance = NULL;

//****************************************************************************
//
//****************************************************************************
/*static*/ bool NWCommManager::staticInit(int _serversReserve/*=eReserve_Servers*/, int _clientsReserve/*=eReserve_Clients*/)
{
    bool bRet = true;

    if(!mInstance)
    {
        mInstance = NEW NWCommManager();
        mInstance->init(_serversReserve , _clientsReserve);
    }

    return bRet;
}

/*static*/ void NWCommManager::staticShutdown()
{
    if(mInstance)
    {
        mInstance->shutdown();
        DISPOSE(mInstance);
    }
}

//****************************************************************************
//
//****************************************************************************
NWCommManager::NWCommManager() :
    mInitd(false),
    mNWCommManagerNotificationCallback(NULL)
{
}

NWCommManager::~NWCommManager()
{
}

//****************************************************************************
//
//****************************************************************************
bool NWCommManager::init(int _serversReserve/*=eReserve_Servers*/, int _clientsReserve/*=eReserve_Clients*/)
{
    bool bRet = false;

    if(!mInitd)
    {
        mServerList.reserve(_serversReserve);
        mClientList.reserve(_clientsReserve);

        mInitd = true;
        bRet = true;
    }

    return bRet;
}

void NWCommManager::shutdown()
{
    if(mInitd)
    {
        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
NWServerSocket * NWCommManager::createServer(NWIP _serverIp, int _listenPort)
{
    NWServerSocket * server = NWServerSocket::create();
    
    sSockNode node;
    node.mSocket = server;
    mServerList.push_back(node);

    server->init(_serverIp, _listenPort); // zzz

    return server;
}

NWClientSocket * NWCommManager::createClient(NWIP _serverIp, int _serverPort)
{
    NWClientSocket * client = NWClientSocket::create();

    sSockNode node;
    node.mSocket = client;
    mClientList.push_back(node);

    client->init(_serverIp, _serverPort);
    return client;
}

//****************************************************************************
//
//****************************************************************************
void NWCommManager::destroyServer(NWServerSocket * _server)
{
    int num = (int)mServerList.size();

    for(int i=0; i<num; i++)
    {
        if(_server == mServerList[i].mSocket)
        {
            NWServerSocket * server = (NWServerSocket *)mServerList[i].mSocket;
            server->done();
            NWServerSocket::destroy(server);
            
            mServerList.erase(mServerList.begin() + i);
        }
    }
}

void NWCommManager::destroyClient(NWClientSocket * _client)
{
    int num = (int)mClientList.size();

    for(int i=0; i<num; i++)
    {
        if(_client == mClientList[i].mSocket)
        {
            NWClientSocket * client = (NWClientSocket *)mClientList[i].mSocket;
            client->done();
            NWClientSocket::destroy(client);

            mClientList.erase(mClientList.begin() + i);
        }
    }
}

//****************************************************************************
//
//****************************************************************************
int NWCommManager::getNumIntefaces()
{
    return 1;
}

NWIP NWCommManager::getInterface(int _idx)
{
    NWIP ip;
    ip.setFromStr("127.0.0.1"); // zzz pending
    
    return ip;
}

//****************************************************************************
//
//****************************************************************************
void NWCommManager::dispatchNetworkMessages()
{
    int numServers = (int)mServerList.size();
    for(int i=0; i<numServers; i++)
    {
        NWSocket * server = mServerList[i].mSocket;
        if(server->messageAvailable())
        {
            server->dispatchMessages();
        }
    }

    int numClients = (int)mClientList.size();
    for(int i=0; i<numClients; i++)
    {
        NWSocket * client = mClientList[i].mSocket;
        if(client->messageAvailable())
        {
            client->dispatchMessages();
        }
    }
}


//****************************************************************************
//
//****************************************************************************
void NWCommManager::sendNotification()
{
    mNWCommManagerNotificationCallback->networkMsgNotification();
}
