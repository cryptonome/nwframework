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
#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "MsgDefs.h"
#include "MsgTypes.h"

//****************************************************************************
// The NWMSG macro creates a class with the provided name and 
// uses an (existing) enum called MsgType_CLASSNAME to provide
// the type value to the class.
// The enum must not repeat values, but you can create ranges
// (i.e. for different app areas)
// There is an optional Message Family type that can be used to filter msgs.
// To create a message with a specific family, use NWMSGF macro. By default
// NWMSG uses the MsgFamilyGeneral family.
// Samples:
// NWMSG(MsgTest1)   // struct MsgTest1 : public NWMsg<MsgTest1, MsgType_MsgTest1, MsgFamily_General>
// {
//     int val1;
// };
// NWMSGF(MsgTest3, MsgFamily_Test)   // struct MsgTest3 : public NWMsg<MsgTest3, MsgType_MsgTest3, MsgFamily_Test>
// {
//     int val3;
// };
//****************************************************************************


#endif // _MESSAGES_H_
