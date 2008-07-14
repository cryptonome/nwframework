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
#ifndef INWSTREAMWRITER_H_
#define INWSTREAMWRITER_H_

#include "NWStreamTypes.h"

class INWStreamBlock;
class INWStreamReader;

//********************************************************************
//
//********************************************************************
class INWStreamWriter
{
public:
    virtual    ~INWStreamWriter () { }

    virtual void writeBlock(INWStreamBlock* _block, bool _checkThread = true) = 0;

    virtual void disableWrite(bool _disable) = 0;

    virtual ENWStreamType getType() const = 0;
    virtual ENWStreamSubType getSubType() const = 0;

    virtual INWStreamReader* createReader() = 0;
};

#endif
