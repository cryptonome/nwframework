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
