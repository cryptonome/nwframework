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
#ifndef NWSTREAMGROUP_H_
#define NWSTREAMGROUP_H_

#include <vector>
#include "INWStreamGroup.h"

class NWCriticalSection;

//********************************************************************
//
//********************************************************************
class NWStreamGroupWrite : public INWStreamGroupWrite
{
public:
    NWStreamGroupWrite  ();
    virtual    ~NWStreamGroupWrite ()                      { NWStreamGroupWrite::done(); }

    virtual bool          init            (const char* _name);
    bool                  isOk            () const  { return mInit; }
    virtual void          done            ();

    // INWStreamGroup
    virtual int getNumStreams() const;
    virtual INWStreamWriter* getStream(int _index);
    virtual INWStreamWriter* getStreamByType(ENWStreamType _type, ENWStreamSubType _subType);
    virtual INWStreamWriter* getStreamByMediaType(ENWStreamMediaType _mediaType);
    virtual void disableWrite(bool _disable);
    
    void addStream(INWStreamWriter* _stream);

    const char* getName() const { return mName.c_str(); }

private:
    bool          mInit : 1;

    std::vector<INWStreamWriter*> mStreams;
    bool mWrite;
    volatile u64 mStartTimeAbs;
    NWCriticalSection* mCSStartTimeAbs;
    
    std::string mName;
};


//********************************************************************
//
//********************************************************************
class NWStreamGroupRead : public INWStreamGroupRead
{
public:
    NWStreamGroupRead  ();
    virtual    ~NWStreamGroupRead ()                      { NWStreamGroupRead::done(); }

    virtual bool          init            (const char* _name);
    bool                  isOk            () const  { return mInit; }
    virtual void          done            ();

    void addStream(INWStreamReader* _stream);

    // INWStreamGroupRead
    virtual int getNumStreams() const;
    virtual INWStreamReader* getStream(int _index);
    virtual INWStreamReader* getStreamByMediaType(ENWStreamMediaType _mediaType);
    virtual void disableRead(bool _disable);

    // Used by NWStreamReader
    u64 getStartTimeAbs() const;
    void setStartTimeAbs(u64 _time);


private:
    bool mInit;
    volatile u64 mStartTimeAbs;
    NWCriticalSection* mCSStartTimeAbs;

    std::vector<INWStreamReader*> mStreams;
    std::string mName;
};




#endif
