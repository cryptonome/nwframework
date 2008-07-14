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
