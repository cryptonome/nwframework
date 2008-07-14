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

#include "PchUtils.h"


#include "NWCommSocket.h"

//****************************************************************************
//
//****************************************************************************
static NWServerSocket * create();
static void destroy(NWServerSocket * _serverSocket);

//****************************************************************************
//
//****************************************************************************
NWServerSocket::NWServerSocket() :
    mInitd(false),
    mListenPort(-1)
{
}

/*virtual*/ NWServerSocket::~NWServerSocket()
{
}

//****************************************************************************
//
//****************************************************************************
bool NWServerSocket::init(int _listenPort)
{
}

void NWServerSocket::done()
{
}

void NWServerSocket::release()
{
}

//****************************************************************************
//
//****************************************************************************
int NWServerSocket::getPort()
{
}

//****************************************************************************
//
//****************************************************************************
void NWServerSocket::send(unsigned char * _buffPtr, int size)
{
}

//****************************************************************************
//
//****************************************************************************
void NWServerSocket::addListener(IServerSocketListener * _listener)
{
}

void NWServerSocket::removeListener(IServerSocketListener * _listener)
{
}

//****************************************************************************
// NWThreadFn
//****************************************************************************
/*virtual*/ unsigned int NWServerSocket::threadMain(ThreadParams const * _threadParams)
{
}

//****************************************************************************
//
//****************************************************************************
void NWServerSocket::onData(MemBufferRef * _memBuff)
{
}
