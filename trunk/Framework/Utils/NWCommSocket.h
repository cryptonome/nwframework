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
