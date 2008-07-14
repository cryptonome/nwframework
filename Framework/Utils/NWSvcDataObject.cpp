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

#include "NWSvcDataObject.h"

#include "NWSvcDataServer.h"
#include "NWSvcDataContext.h"
#include "MemBufferRef.h"

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWSvcDataBase::NWSvcDataBase() :
    mInitd(false),
    mName("INVALID_SVC_DATA_OBJ"),
    mContextName("INVALID_SVC_CONTEXT"),
    mServerMode(false),
    mReentryUpdateLevel(0),
    mSvcDataServer(NULL)
{
    mNotificationCallbackList.reserve(16);
}

/*virtual*/ NWSvcDataBase::~NWSvcDataBase()
{
    ASSERT(!mInitd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool NWSvcDataBase::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer)
{
    bool retVal = false;

    if(!mInitd)
    {
        mName = _objName;
        mContextName = _contextName;
        mSvcDataServer = _dataServer;
        mServerMode = _isServer;

        mInitd = true;
        retVal = true;
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataBase::shutdown()
{
    if(mInitd)
    {
        mNotificationCallbackList.clear();

        mInitd = false;
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWSvcDataObject::NWSvcDataObject() : NWSvcDataBase(),
    mCurrentUpdateLevel(0),
    mLastReceivedUpdateLevel(0)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ NWSvcDataObject::~NWSvcDataObject()
{
    ASSERT(!mInitd);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool NWSvcDataObject::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer)
{
    bool retVal = Inherited::init(_contextName, _objName, _dataServer, _isServer);

    if(retVal)
    {
        if(!_isServer)
        {
            NWSvcDataObject * ptr = this;
            mSvcDataServer->requestUpdate(ptr);  // send update request to get the value;
            mCurrentUpdateLevel++;
        }
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataObject::shutdown()
{
    if(mInitd)
    {
        if(mSvcDataServer)
            mSvcDataServer->unregSvrObj(mContextName.c_str(), mName.c_str(), this);

        Inherited::shutdown();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataObject::addNotificationCallback(NWSvcDataObjNotificationCallback * _callback)
{
    if(_callback)
    {
        NotificationCallbackNode node;
        node.mCallbackPtr = (void *)_callback;

        mNotificationCallbackList.push_back(node);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataObject::removeNotificationCallback(NWSvcDataObjNotificationCallback * _callback)
{
    if(_callback)
    {
        int num = (int)mNotificationCallbackList.size();
        for (int i=0; i<num; i++)
        {
            if(mNotificationCallbackList[i].mCallbackPtr == _callback)
                mNotificationCallbackList.erase(mNotificationCallbackList.begin() + i);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataObject::NotifyUpdate()
{
    int num = (int)mNotificationCallbackList.size();
    for (int i=0; i<num; i++)
    {
        NWSvcDataObjNotificationCallback * callback = (NWSvcDataObjNotificationCallback *)mNotificationCallbackList[i].mCallbackPtr;
        if(callback)
            callback->update(mContextName.c_str(), mName.c_str());
    }
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
NWSvcDataEventHandler::NWSvcDataEventHandler() : NWSvcDataBase()
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ NWSvcDataEventHandler::~NWSvcDataEventHandler()
{
    if(mInitd)
        shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ bool NWSvcDataEventHandler::init(const char * _contextName, const char * _objName, NWSvcDataServer * _dataServer, bool _isServer)
{
    bool retVal = Inherited::init(_contextName, _objName, _dataServer, _isServer);

    if(retVal)
    {
        // zzz
    }

    return retVal;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataEventHandler::shutdown()
{
    if(mInitd)
    {
        if(mSvcDataServer)
            mSvcDataServer->unregEventHandler(mContextName.c_str(), mName.c_str(), this);

        Inherited::shutdown();
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataEventHandler::addNotificationCallback(NWSvcDataEventNotificationCallback * _callback)
{
    if(_callback)
    {
        NotificationCallbackNode node;
        node.mCallbackPtr = (void *)_callback;

        mNotificationCallbackList.push_back(node);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataEventHandler::removeNotificationCallback(NWSvcDataEventNotificationCallback * _callback)
{
    if(_callback)
    {
        int num = (int)mNotificationCallbackList.size();
        for (int i=0; i<num; i++)
        {
            if(mNotificationCallbackList[i].mCallbackPtr == _callback)
                mNotificationCallbackList.erase(mNotificationCallbackList.begin() + i);
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataEventHandler::sendEvent(NWSvcDataEvent * _event)
{
    ASSERT(mInitd);
    ASSERT(_event);

    _event->mFromServer = isServer();

    MemorySerializerOut serializerOut;
    serializerOut.init(2048);

    _event->serializeOut(serializerOut);

    MemBufferRef buff = serializerOut.getMemBuffer();
    
    mSvcDataServer->sendEvent(getContextName(), getObjName(), _event->getGenericEventType(), buff, !isServer());
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
/*virtual*/ void NWSvcDataEventHandler::receiveEventMsg(int _eventType, MemBufferRef * _eventData)
{
    int num = (int)mNotificationCallbackList.size();
    for (int i=0; i<num; i++)
    {
        NWSvcDataEventNotificationCallback * callback = (NWSvcDataEventNotificationCallback *)mNotificationCallbackList[i].mCallbackPtr;
        if(callback)
            callback->receiveEventMsg(_eventType, _eventData);
    }
}
