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
