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
//***************************************************************************
//
//***************************************************************************
#ifndef _APP_MSGS_H_
#define _APP_MSGS_H_

#include "MsgDefs.h"
#include "MsgTypes.h"

#include "MemBufferRef.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
enum eCliSrvMsg
{
    MsgType_DataServerEvent = MsgType_CliSrvMsg_Start,
    MsgType_MsgClientUpdateReq,
    MsgType_MsgServerUpdateClient,
    MsgType_MsgClientSetValue,
    MsgType_MsgSvcEvent

    // the last msg must be <= MsgType_CliSrvMsg_End
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#pragma pack(push, 4)
NWMSGF(MsgClientUpdateReq, MsgFamily_CliSrvDataService)
{
    std::string mContext;
    std::string mObjName;
};
#pragma pack(pop)

#pragma pack(push, 4)
NWMSGF(MsgServerUpdateClient, MsgFamily_CliSrvDataService)
{
    std::string mContext;
    std::string mObjName;
    MemBufferRef mMemBuffer;

    MsgServerUpdateClient()
    {
    }
};
#pragma pack(pop)

#pragma pack(push, 4)
NWMSGF(MsgClientSetValue, MsgFamily_CliSrvDataService)
{
    std::string mContext;
    std::string mObjName;
    MemBufferRef mMemBuffer;

    MsgClientSetValue()
    {
    }

};
#pragma pack(pop)

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
#pragma pack(push, 4)
NWMSGF(MsgSvcEvent, MsgFamily_CliSrvDataService)
{
    bool mServerMsg;

    std::string mContext;
    std::string mObjName;

    int mEventType;
    MemBufferRef mMemBuffer;

    MsgSvcEvent() :
        mServerMsg(false),
        mEventType(0)
    {
    }
};
#pragma pack(pop)

#endif  // _APP_MSGS_H_
