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
#include "PchAppBase.h"

#include "Application.h"
#include "juce.h"

class InCrewApplication;

static InCrewApplication* sApplication = 0;

//****************************************************************************
//
//****************************************************************************
//==============================================================================
class InCrewApplication : public JUCEApplication
{
    /* Important! NEVER embed objects directly inside your JUCEApplication class! Use
       ONLY pointers to objects, which you should create during the initialise() method
       (NOT in the constructor!) and delete in the shutdown() method (NOT in the
       destructor!)

       This is because the application object gets created before Juce has been properly
       initialised, so any embedded objects would also get constructed too soon.
   */
    Application* mApp;

public:
    //==============================================================================
    InCrewApplication()
        : mApp(0)
    {
        // NEVER do anything in here that could involve any Juce function being called
        // - leave all your startup tasks until the initialise() method.
        sApplication = this;
    }

    ~InCrewApplication()
    {
        // Your shutdown() method should already have done all the things necessary to
        // clean up this app object, so you should never need to put anything in
        // the destructor.

        // Making any Juce calls in here could be very dangerous...
        sApplication = 0;
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        Utils::init(false);

        ASSERT(mApp == 0);
        mApp = Application::CreateApplication();
        mApp->init(commandLine);
    }

    void shutdown()
    {
        if (mApp)
        {
            mApp->done();
            Application::DestroyApplication(mApp);
            mApp = 0;
        }
    
        Utils::done();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return T("InCrew App");
    }

    const String getApplicationVersion()
    {
        return T("1.0");
    }

    bool moreThanOneInstanceAllowed()
    {
        return true;
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        // This will get called if the user launches another copy of the app, but
        // there's nothing that the demo app needs to do here.
    }

    /*virtual void systemRequestedQuit()
    {
        mApp->systemRequestedQuit();
    }*/
};


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
START_JUCE_APPLICATION (InCrewApplication)

//****************************************************************************
//
//****************************************************************************
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Application::quit()
{
    sApplication->quit();
}
