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
