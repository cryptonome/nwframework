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
#ifndef NWSTREAMAUDIO_H_
#define NWSTREAMAUDIO_H_

#include "NWStreamMedia.h"

//********************************************************************
//
//********************************************************************
class NWStreamAudio : public NWStreamMedia
{
public:
    NWStreamAudio  ();
    virtual    ~NWStreamAudio ()                      { NWStreamAudio::done(); }

    virtual bool          init            ();
    virtual void          done            ();


private:
    typedef NWStreamMedia Inherited;

};

#endif