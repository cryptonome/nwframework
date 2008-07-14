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
