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
#ifndef INWSTREAMGROUP_H_
#define INWSTREAMGROUP_H_

#include "NWStreamTypes.h"

class INWStreamWriter;
class INWStreamReader;

//********************************************************************
//
//********************************************************************
class INWStreamGroupRead
{
public:
    virtual    ~INWStreamGroupRead () { }

    virtual int getNumStreams() const = 0;
    virtual INWStreamReader* getStream(int _index) = 0;

    virtual INWStreamReader* getStreamByMediaType(ENWStreamMediaType _mediaType) = 0;

    virtual void disableRead(bool _disable) = 0;
};

//********************************************************************
//
//********************************************************************
class INWStreamGroupWrite
{
public:
    virtual    ~INWStreamGroupWrite () { }

    virtual int getNumStreams() const = 0;
    virtual INWStreamWriter* getStream(int _index) = 0;

    virtual INWStreamWriter* getStreamByType(ENWStreamType _type, ENWStreamSubType _subType) = 0;
    virtual INWStreamWriter* getStreamByMediaType(ENWStreamMediaType _mediaType) = 0;

    virtual void disableWrite(bool _disable) = 0;
};

#endif
