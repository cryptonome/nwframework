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
