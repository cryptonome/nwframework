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
#ifndef NWCOMM_SOCKET_H
#define NWCOMM_SOCKET_H

#include "NWThread.h"
#include "NWIP.h"

#include <vector>

struct NWSocketData;
class NWServerSocket;
class MemBufferRef;

//****************************************************************************
//
//****************************************************************************
class NWSocket : public NWThreadFn
{
public:
    NWSocket();
    virtual ~NWSocket();

    virtual bool init();
    virtual void done();

    inline bool isInitd();

    virtual bool messageAvailable() = 0;
    virtual void dispatchMessages() = 0;

protected:
    struct sListener
    {
        void * mListener;
    };

    bool mInitd;
    std::vector<sListener> mListenerList;
    NWSocketData * mSocketData;
};

inline bool NWSocket::isInitd()
{
    return mInitd;
}

//****************************************************************************
//
//****************************************************************************
struct IServerSocketListener
{
    virtual bool onAccept(NWServerSocket * _socket) = 0;
    virtual void onClientConnected(int _clientId) = 0;
    virtual void onClientDisconnected(int _clientId, int _reason) = 0;
    virtual void onClientData(int _clientIdFrom, MemBufferRef * _memBuff) = 0;
    virtual void onPing(int _clientId, int _ms) = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
struct sListenerThreadParams
{
};

class NWServerSocket : public NWSocket
{
public:
    static NWServerSocket * create();
    static void destroy(NWServerSocket * _serverSocket);

    bool init(NWIP _interface, int _listenPort);
    void done();
    void release();

    int getPort();
   
    void send(unsigned char * _buffPtr, int size);

    void addListener(IServerSocketListener * _listener);
    void removeListener(IServerSocketListener * _listener);

protected:
    struct sStoredMessage
    {
        MemBufferRef * mData;

        sStoredMessage() : mData(0) {}
    };

    int mListenPort;
    NWThread * mListenerThread;
    sListenerThreadParams mListenerThreadParams;

    NWServerSocket();
    virtual ~NWServerSocket();

    virtual bool messageAvailable();
    virtual void dispatchMessages();

    // NWThreadFn
    virtual unsigned int threadMain(ThreadParams const * _threadParams);

    void onData(MemBufferRef * _memBuff);

    NWServerSocket(NWServerSocket const & _other);              // disabled copy
    NWServerSocket operator=(NWServerSocket const & _other);    // disabled copy
};

//****************************************************************************
//
//****************************************************************************
struct IClientSocketListener
{
    virtual void onConnected() = 0;
    virtual void onDisconnected(int _reason) = 0;
    virtual void onData(MemBufferRef * _memBuff) = 0;
    virtual void onPing(int _ms) = 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
class NWClientSocket : public NWSocket
{
public:
    static NWClientSocket * create();
    static void destroy(NWClientSocket * _socket);

    bool init(NWIP _serverIp, int _serverPort);
    void done();
    void release();

protected:
    NWClientSocket();
    virtual ~NWClientSocket();

    // NWThreadFn
    virtual unsigned int threadMain(ThreadParams const * _threadParams);

    virtual bool messageAvailable();
    virtual void dispatchMessages();
};

#endif // NWCOMM_SOCKET_H
