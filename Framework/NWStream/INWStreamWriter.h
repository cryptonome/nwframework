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
