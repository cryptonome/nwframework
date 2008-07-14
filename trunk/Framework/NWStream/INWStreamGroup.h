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
