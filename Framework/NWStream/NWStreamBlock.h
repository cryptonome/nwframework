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
#ifndef NWSTREAMBLOCK_H_
#define NWSTREAMBLOCK_H_

#include "INWStreamBlock.h"

//********************************************************************
//
//********************************************************************
class NWStreamBlock : public INWStreamBlock
{
public:
    bool                  isOk            () const  { return mInit; }

    // INWStreamBlock
    virtual ENWStreamType getType() const;
    virtual ENWStreamSubType getSubType() const;

protected:
    NWStreamBlock  ();
    virtual    ~NWStreamBlock ()                      { NWStreamBlock::done(); }
    
    virtual bool          init            (ENWStreamType _type, ENWStreamSubType _subType);
    virtual void          done            ();

private:
    bool          mInit : 1;
    ENWStreamType mType;
    ENWStreamSubType mSubType;
};

#endif
