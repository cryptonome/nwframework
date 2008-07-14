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
