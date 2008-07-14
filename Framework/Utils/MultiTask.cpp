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
#include "PchProtoPlayer.h"

#include "MultiTask.h"
#include "NWEvent.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
MultiTask::MultiTask() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool MultiTask::init(int _tasks, ITaskProcessor* _processor)
{
    bool bOK = true;

    if (!isOk())
    {
        mTaskProcessor = _processor;

        for ( int i = 0 ; i < _tasks ; ++i )
        {
            Thread* thread = NEW Thread;
            thread->mEventExit = NWEvent::create(true);
            thread->mThread = 0;
            mThreads.push_back(thread);
        }

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void MultiTask::done()
{
    if (isOk())
    {
        stop();

        for ( std::list<Thread*>::iterator it = mThreads.begin() ; it != mThreads.end() ; ++it )
        {
            Thread* thread = *it;
            ASSERT(thread->mThread == 0);
            NWEvent::destroy(thread->mEventExit);
            DISPOSE(thread);
        }

        mInit = false;
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
unsigned int MultiTask::threadMain(ThreadParams const * _threadParams)
{
    Thread* thread = (Thread*)_threadParams->mUserParams;
    int index = getThreadIndex(thread);
    bool exit = false;
    while ( !thread->mEventExit->isSignaled() && !exit )
    {
        exit = mTaskProcessor->multiTaskProcess(this,index);
    }
    return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int MultiTask::getThreadIndex(Thread* thread)
{
    int i = -1;
    bool found = false;

    for ( std::list<Thread*>::iterator it = mThreads.begin() ; !found && it != mThreads.end() ; ++it, ++i )
    {
        Thread* threadIt = *it;
        if ( threadIt == thread )
            found = true;
    }

    ASSERT(found);
    ASSERT(i >= 0 && i < (int)mThreads.size());

    return i;
}



//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool MultiTask::start()
{
    bool bOK = true;

    // Start the threads
    for ( std::list<Thread*>::iterator it = mThreads.begin() ; it != mThreads.end() ; ++it )
    {
        Thread* threadIt = *it;
        ASSERT(threadIt->mThread == 0);
        threadIt->mEventExit->reset();
        threadIt->mThread = NWThread::create();
        threadIt->mThread->start(this,threadIt,NWT_PRIORITY_CRITICAL);
    }

    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void MultiTask::stop()
{
    // Start the threads
    for ( std::list<Thread*>::iterator it = mThreads.begin() ; it != mThreads.end() ; ++it )
    {
        Thread* threadIt = *it;
        threadIt->mEventExit->signal();
        NWThread::destroy(threadIt->mThread);
    }
}
