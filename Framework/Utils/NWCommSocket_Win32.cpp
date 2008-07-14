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

#include "NWCommSocket.h"
#include "NWEvent.h"
#include "NWCriticalSection.h"

#include <WinSock.h>

struct NWSocketData
{
    SOCKET mSocket;
    NWCriticalSection * mCritSec;
};

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWSocket::NWSocket() : 
    mInitd(false),
    mSocketData(NULL)
{
}

/*virtual*/ NWSocket::~NWSocket()
{
    ASSERT(mInitd);
}

/*virtual*/ bool NWSocket::init()
{
    bool bRet = true;

    if(!mInitd)
    {
        mListenerList.reserve(8);
        mSocketData = NEW NWSocketData;

        mInitd = true;
        bRet = true;
    }
    
    return bRet;
}

/*virtual*/ void NWSocket::done()
{
    if(mInitd)
    {
        DISPOSE(mSocketData);
        mListenerList.clear();

        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ NWServerSocket * NWServerSocket::create()
{
    return NEW NWServerSocket();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ void NWServerSocket::destroy(NWServerSocket * _serverSocket)
{
    if(_serverSocket)
    {
        _serverSocket->done();
        DISPOSE(_serverSocket);
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWServerSocket::NWServerSocket() : NWSocket(),
    mListenPort(-1)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ NWServerSocket::~NWServerSocket()
{
    done();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWServerSocket::init(NWIP _interface, int _listenPort)
{
    bool bRet = false;

    if(!mInitd)
    {
        mListenerThread = NWThread::create();
        mListenerThread->start(this);

        mInitd = true;
        bRet = true;
    }

    return bRet;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::done()
{
    if(mInitd)
    {
        mInitd = false;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::release()
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int NWServerSocket::getPort()
{
    return -1;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::send(unsigned char * _buffPtr, int size)
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::addListener(IServerSocketListener * _listener)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::removeListener(IServerSocketListener * _listener)
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool NWServerSocket::messageAvailable()
{
    return false;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWServerSocket::dispatchMessages()
{
}

//****************************************************************************
// NWThreadFn
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ unsigned int NWServerSocket::threadMain(ThreadParams const * _threadParams)
{
    NWEvent * endEvent = _threadParams->mEventEndRequest;
    
    while(!endEvent->isSignaled())
    {
    }

    // close listen socket
    // close client sockets

    return 0;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWServerSocket::onData(MemBufferRef * _memBuff)
{
}


//****************************************************************************
// Client Socket
//****************************************************************************

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ NWClientSocket * NWClientSocket::create()
{
    return NEW NWClientSocket();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*static*/ void NWClientSocket::destroy(NWClientSocket * _socket)
{
    ASSERT(_socket);
    
    _socket->done();
    DISPOSE(_socket);
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWClientSocket::NWClientSocket() : NWSocket()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ NWClientSocket::~NWClientSocket()
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool NWClientSocket::init(NWIP _serverIp, int _serverPort)
{
    return false;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWClientSocket::done()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void NWClientSocket::release()
{
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ unsigned int NWClientSocket::threadMain(ThreadParams const * _threadParams)
{
    return 0;
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool NWClientSocket::messageAvailable()
{
    return false;
}

/*virtual*/ void NWClientSocket::dispatchMessages()
{
}
