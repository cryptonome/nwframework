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
