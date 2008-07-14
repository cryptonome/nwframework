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
#ifndef _MSG_MGR_DEFS_H_
#define _MSG_MGR_DEFS_H_

typedef int ChannelId;
typedef int CommNodeId;

enum
{
    InvalidChannelId = 0,
    InvalidCommNodeID = 0,
    NumMaxMsgsDispatched = 100
};

#endif // _MSG_MGR_DEFS_H_
