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
#include "PchNomosVE.h"

#include "AppModules.h"
#include "Config.h"
#include "SystemUtils.h"
#include "GUI.h"

#include "MsgMgr.h"
#include "MsgTypes.h"
#include "TestDataServer.h"

static const char * CONFIG_FILE_NAME = "NOMOS.INI";
static const char * SVC_DATA_SERVER_NAME = "DATA_SERVER";

//****************************************************************************
//
//****************************************************************************
void MsgManagerNotificationCallback()
{
    //gCliSvrDataDlg->PostMessage(NOTIFICATION_CALLBACK_MSG);
}

//****************************************************************************
//
//****************************************************************************
/*static*/ AppModules * AppModules::mInstance = 0;

//****************************************************************************
//
//****************************************************************************
/*static*/ bool AppModules::initModules()
{
    if(mInstance == 0)
    {
        mInstance = NEW AppModules();
    }

    return mInstance->init();
}

/*static*/ void AppModules::shutdownModules()
{
    if(mInstance != 0)
    {
        delete mInstance;
        mInstance = 0;
    }
}

//****************************************************************************
// --- non static area, call using Instance ---
//****************************************************************************

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
AppModules::AppModules() :
	mConfig(0)
{
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
AppModules::~AppModules()
{
    shutdown();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
bool AppModules::init()
{
    bool bOK = true;

    std::string currentDir = SystemUtils::getCurrentDirectory();
	mConfig = NEW Config();
	bOK &= mConfig->init(CONFIG_FILE_NAME,currentDir.c_str());
    ASSERT(bOK);

    return bOK;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void AppModules::shutdown()
{
	if (mConfig)
	{
		mConfig->done();
        DISPOSE(mConfig);
	}
}
