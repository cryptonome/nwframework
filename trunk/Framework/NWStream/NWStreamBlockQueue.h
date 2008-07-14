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
#ifndef NWSTREAMBLOCKQUEUE_H_
#define NWSTREAMBLOCKQUEUE_H_

#include <list>

class INWStreamBlock;
class NWStreamWriter;
class NWCriticalSection;
class NWEvent;

//********************************************************************
//
//********************************************************************
class NWStreamBlockQueue
{
public:
    NWStreamBlockQueue  ();
    virtual    ~NWStreamBlockQueue ()                      { NWStreamBlockQueue::done(); }

    virtual bool          init                 (NWStreamWriter* _stream);
    bool                  isOk                 () const  { return mInit; }
    virtual void          done                 ();

    void writeBlock(INWStreamBlock* _block);
    INWStreamBlock* readBlock();

    void disableRead(bool _disable);
    void disableWrite(bool _disable);

private:
    bool          mInit : 1;
    int mWriteBuffer;

    //void swap();

    int getWriteBufferIndex() const { return mWriteBuffer; }
    int getReadBufferIndex() const { return (mWriteBuffer+1)&1;}
    std::list<INWStreamBlock*>& getWriteBuffer() { return mBlocks[getWriteBufferIndex()]; }
    std::list<INWStreamBlock*>& getReadBuffer() { return mBlocks[getReadBufferIndex()]; }

    bool mNeedEvent;
    std::list<INWStreamBlock*> mBlocks[2];

    NWCriticalSection* mCSSwap;
    NWEvent* mEventNewData;
    NWEvent* mEventMaxSize;

    volatile bool mDisableRead;
    volatile bool mDisableWrite;

    NWStreamWriter* mStream;
};

#endif
