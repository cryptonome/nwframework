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
