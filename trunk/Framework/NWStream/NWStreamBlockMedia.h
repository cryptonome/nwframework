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
#ifndef NWSTREAMBLOCKMEDIA_H_
#define NWSTREAMBLOCKMEDIA_H_

#include "NWStreamBlock.h"

//********************************************************************
//
//********************************************************************
class NWStreamBlockMedia : public NWStreamBlock
{
public:

    void setTime(u64 _time);
    u64 getTime() const;


    virtual void setEnd(bool _end);
    virtual bool IsEnd() const;

protected:
    NWStreamBlockMedia  ();
    virtual    ~NWStreamBlockMedia ()                      { NWStreamBlockMedia::done(); }

    virtual bool          init                 (ENWStreamSubType _subType);
    virtual void          done                 ();

private:
    typedef NWStreamBlock Inherited;
    bool mEnd;
    u64 mTime;
};

#endif
