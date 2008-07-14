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
#ifndef NWSTREAM_H_
#define NWSTREAM_H_

#include "INWStream.h"

class NWStreamBlockQueue;
class NWStreamGroupWrite;
class NWStreamGroupRead;

//********************************************************************
//
//********************************************************************
class NWStreamWriter : public INWStreamWriter
{
public:
    // INWStream
    virtual ENWStreamType getType() const;
    virtual ENWStreamSubType getSubType() const;
    virtual void writeBlock(INWStreamBlock* _block, bool _checkThread = true);
    //virtual INWStreamBlock* readBlock(bool _checkThread = true);
    //virtual void disableRead(bool _disable);
    virtual void disableWrite(bool _disable);
    //virtual u64 getStartTimeAbs();
    //virtual void setStartTimeAbs(u64 _time);
    virtual INWStreamReader* createReader();

    // Used by the StreamGroup
    void setStreamGroupWrite(NWStreamGroupWrite* _streamGroup);
    //void setStreamGroupRead(NWStreamGroup* _streamGroup);

    // Used by the StreamQueue
    const NWStreamGroupWrite* getStreamGroupWrite() const { return mStreamGroupWrite; }
    //const NWStreamGroup* getStreamGroupRead() const { return mStreamGroupRead; }

    // Used by StreamRead
    NWStreamBlockQueue* getQueue() { return mQueue; }

protected:
    NWStreamWriter   ();
    virtual    ~NWStreamWriter  ()                      { NWStreamWriter::done(); }

    virtual bool          init        (ENWStreamType _type, ENWStreamSubType _subType);
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    NWStreamGroupWrite* mStreamGroupWrite;
    //NWStreamGroup* mStreamGroupRead;

private:
    //void checkReadThreadId();
    void checkWriteThreadId();

    bool          mInit : 1;
    ENWStreamType mType;
    ENWStreamSubType mSubType;
    NWStreamBlockQueue* mQueue;
    bool mDisabled;

    //unsigned int mReadThreadId;
    unsigned int mWriteThreadId;
};


//********************************************************************
//
//********************************************************************
class NWStreamReader : public INWStreamReader
{
public:
    // INWStream
    virtual ENWStreamType getType() const;
    virtual ENWStreamSubType getSubType() const;
    virtual INWStreamBlock* readBlock(bool _checkThread = true);
    virtual void disableRead(bool _disable);
    virtual INWStreamWriter* getStream() { return mStream; }

    virtual u64 getStartTimeAbs();
    virtual void setStartTimeAbs(u64 _time);

    // Used by the StreamGroup
    void setStreamGroupRead(NWStreamGroupRead* _streamGroup);

    // Used by the StreamQueue
    const NWStreamGroupRead* getStreamGroupRead() const { return mStreamGroupRead; }

protected:
    NWStreamReader   ();
    virtual    ~NWStreamReader  ()                      { NWStreamReader::done(); }

    virtual bool          init        (NWStreamWriter* _stream);
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    NWStreamGroupRead* mStreamGroupRead;

private:
    friend NWStreamWriter;

    void checkReadThreadId();

    bool mInit : 1;
    bool mDisabled;

    unsigned int mReadThreadId;

    NWStreamWriter* mStream;
};

#endif
