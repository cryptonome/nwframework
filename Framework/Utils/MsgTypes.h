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


#ifndef _MSG_TYPES_H_
#define _MSG_TYPES_H_

enum eMsgTypes
{
    MsgType_Null = 0,
    MsgType_Internal,

    // --- Reserved range for ClientServerDataService ---
    MsgType_CliSrvMsg_Start,
    MsgType_CliSrvMsg_End = MsgType_CliSrvMsg_Start + 10,

    // --- Reserved range for testing purposes ---
    MsgType_Test_Start,
    MsgType_Test_End    = MsgType_Test_Start + 10,

    // Add here more reserved msg types / ranges (only if used by the Framework, the App just can have their own enum starting at MsgType_App_Msgs_Start)

    MsgType_App_Msgs_Start
};

enum eMsgFamilies
{
    MsgFamily_Null = 0,

    // --- Family for ClientServerDataService
    MsgFamily_CliSrvDataService,

    // --- Family for testing purposes ---
    MsgFamily_Test,

    MsgFamily_General,

    MsgFamily_AppFamilies_Start
};

enum eCommNodeIds
{
    CommNodeId_Null = 0,

    CommNodeId_Reserved_SvrDataSvc_Start,   // reserved ids used by the ClientServerDataService
        CommNodeId_DataServiceServer = CommNodeId_Reserved_SvrDataSvc_Start,
        CommNodeId_DataServiceClient,
    CommNodeId_Reserved_SvrDataSvc_End = CommNodeId_DataServiceClient,

    CommNodeId_AppIds_Start
};

#endif //_MSG_TYPES_H_
