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
#ifndef MULTITASK_H_
#define MULTITASK_H_

#include "NWThread.h"
#include <list>

//********************************************************************
//
//********************************************************************
class MultiTask : public NWThreadFn
{
public:
    MultiTask  ();
    virtual    ~MultiTask ()                      { MultiTask::done(); }

    //--------------------------------
    // ITaskProcessor
    //--------------------------------
    class ITaskProcessor
    {
    public:
        virtual ~ITaskProcessor() { }

        virtual bool multiTaskProcess(MultiTask* _multiTask, int _task) = 0;
    };


    virtual bool          init        (int _tasks, ITaskProcessor* _processor);
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    bool start();
    void stop();

private:
    struct Thread
    {
        NWThread* mThread;
        NWEvent* mEventExit;
    };

    // NWThreadFn
    virtual unsigned int threadMain(ThreadParams const * _threadParams);

    int getThreadIndex(Thread* thread);

    bool          mInit : 1;
    std::list<Thread*> mThreads;
    ITaskProcessor* mTaskProcessor;
};

#endif
