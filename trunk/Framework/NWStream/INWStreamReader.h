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
#ifndef INWSTREAMREADER_H_
#define INWSTREAMREADER_H_

#include "NWStreamTypes.h"

class INWStreamBlock;

//********************************************************************
//
//********************************************************************
class INWStreamReader
{
public:
    virtual    ~INWStreamReader () { }

    virtual INWStreamBlock* readBlock(bool _checkThread = true) = 0;

    virtual void disableRead(bool _disable) = 0;

    virtual ENWStreamType getType() const = 0;
    virtual ENWStreamSubType getSubType() const = 0;

    virtual INWStreamWriter* getStream() = 0;

    // TODO: Temporal
    // Tiempo absoluto de reproduccion (los bloques anteriores a este tiempo deberian
    // de ignorarse, o tratarse como tiempo 0)
    virtual u64 getStartTimeAbs() = 0;
    virtual void setStartTimeAbs(u64 _time) = 0;
};

#endif
