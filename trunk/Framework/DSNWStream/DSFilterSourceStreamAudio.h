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
#ifndef DSFILTERSOURCESTREAMAUDIO_H_
#define DSFILTERSOURCESTREAMAUDIO_H_

#include "streams.h"

class DSStreamSourceStreamAudio;
class INWStreamReader;

//********************************************************************
//
//********************************************************************
class DSFilterSourceStreamAudio : public CSource
{
public:

    static DSFilterSourceStreamAudio* createInstance(INWStreamReader* _stream);

private:
    DSFilterSourceStreamAudio  ();
    virtual    ~DSFilterSourceStreamAudio ()                      { DSFilterSourceStreamAudio::done(); }

    virtual bool          init                      (INWStreamReader* _stream);
    bool                  isOk                      () const  { return mInit; }
    virtual void          done                      ();

    bool createStream(INWStreamReader* _stream);

    bool          mInit : 1;

    DSStreamSourceStreamAudio* mStream;
};


#endif
