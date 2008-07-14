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
