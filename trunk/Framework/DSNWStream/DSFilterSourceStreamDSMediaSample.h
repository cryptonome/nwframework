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
#ifndef DSFILTERSOURCESTREAMDSMEDIASAMPLE_H_
#define DSFILTERSOURCESTREAMDSMEDIASAMPLE_H_

#include "streams.h"
#include "DSStreamSourceStreamDSMediaSample.h"

class INWStreamReader;

//********************************************************************
//
//********************************************************************
class DSFilterSourceStreamDSMediaSample : public CSource
{
public:

    static DSFilterSourceStreamDSMediaSample* createInstance(INWStreamReader* _stream);

    INWStreamReader* getStream() const { return mStream->getStream(); }

private:
    DSFilterSourceStreamDSMediaSample  ();
    virtual    ~DSFilterSourceStreamDSMediaSample ()                      { DSFilterSourceStreamDSMediaSample::done(); }

    virtual bool          init                      (INWStreamReader* _stream);
    bool                  isOk                      () const  { return mInit; }
    virtual void          done                      ();

    bool createStream(INWStreamReader* _stream);

    bool          mInit : 1;

    DSStreamSourceStreamDSMediaSample* mStream;
};

#endif
