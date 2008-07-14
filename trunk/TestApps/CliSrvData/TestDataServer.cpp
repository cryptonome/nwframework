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
#include "PchNomosVE.h"

#include "TestDataServer.h"
#include "TestDataServerEvents.h"
#include "MemBufferRef.h"

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
TestDataServer::TestDataServer() :
    mInitd(false),
    mSvrDataService(NULL),
    mSvrBool(false),
    mSvrText("Test Text")
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
TestDataServer::~TestDataServer()
{
    ASSERT(!mInitd);

    shutdown();
}

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool TestDataServer::init(NWSvcDataServer * _svrDataService)
{
    bool retVal = false;

    if(!mInitd)
    {
        mSvrDataService = _svrDataService;

        REG_START(_svrDataService, "CLI_SVR_DLG_DATA");
            REG_SVR_OBJ(SvrBool, false);
            REG_SVR_OBJ(SvrText, "");
            REG_SVR_EVENTHANDLER("EVENT", mEventHandler);
            //REG_SVR_OBJ_DELEGATE_MEMBER(DelegateInt, TestIntDelegate::set, TestIntDelegate::get, mTestIntValue);
            REG_SVR_DATALIST("DataList", mDataList);
            REG_SVR_OBJ_DELEGATE(TestDelegate2);
        REG_END();

        //mDelegateDelegateInt.set(99);
        //int kk = mDelegateDelegateInt.get();

        mEventHandler.addNotificationCallback(this);

        retVal = true;
        mInitd = true;
    }

    return retVal;
}

DELEGATE_SET_FN(TestDataServer, int, TestDelegate2)
{
}

DELEGATE_GET_FN(TestDataServer, int, TestDelegate2)
{
    return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void TestDataServer::shutdown()
{
    if(mInitd)
    {
        mSvrBool.shutdown();
        mSvrText.shutdown();
        //mDelegateDelegateInt.shutdown();
        mEventHandler.shutdown();

        mInitd = false;
    }
}

/*virtual*/ void TestDataServer::receiveEventMsg(int _eventType, MemBufferRef * _eventData)
{
    MemorySerializerIn serializerIn; // move this to sender and pass as parameter instead the MemBuffer
    serializerIn.setBuffer(_eventData->getPtr(), _eventData->getSize());

    switch(_eventType)
    {
        case NWSvcDataEvent::eEventType_GenericUserData2:
        {
            NWGenericEventUserData2 ev;
            ev.serializeIn(serializerIn);

            mEventHandler.sendEvent(&ev);
            break;
        }

        case eTestDataServerEvents_SetValItem:
        {
            TestDataServerEvent_RqAddItem ev;
            ev.serializeIn(serializerIn);

            mEventHandler.sendEvent(&ev);
            break;
        }
    }
}
