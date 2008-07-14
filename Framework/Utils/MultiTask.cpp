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
