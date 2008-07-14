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
