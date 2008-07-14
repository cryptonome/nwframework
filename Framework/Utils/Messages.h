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
