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
