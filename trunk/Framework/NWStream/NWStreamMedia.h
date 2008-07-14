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
#ifndef NWSTREAMMEDIA_H_
#define NWSTREAMMEDIA_H_

#include "NWStream.h"

//********************************************************************
//
//********************************************************************
class NWStreamMedia : public NWStreamWriter
{
public:

    ENWStreamMediaType getMediaType();

protected:
    NWStreamMedia  ();
    virtual    ~NWStreamMedia ()                      { NWStreamMedia::done(); }

    virtual bool          init            (ENWStreamSubType _subType, ENWStreamMediaType _mediaType);
    virtual void          done            ();

    // NWStream
    //virtual INWStreamBlock* readBlock(bool _checkThread);

    void setMediaType(ENWStreamMediaType _mediaType);

private:
    typedef NWStreamWriter Inherited;

    //void checkMediaBlockSync(INWStreamBlock* _streamBlock);
    //u64 getLastReadTimeFromMediaStreams();

    volatile u64 mLastReadTime;

    ENWStreamMediaType mMediaType;
};

#endif
