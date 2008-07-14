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
#ifndef _APP_MODULES_H_
#define _APP_MODULES_H_

#include "Application.h"
#include "MsgMgr.h"

class Config;
class GUI;
class NWSvcDataServer;
class Application;

//****************************************************************************
// Manualy initialized singleton
//****************************************************************************
class AppModules
{
public:
    // --- Singleton area ---
    static bool initModules();
    static void shutdownModules();

    static inline AppModules * getInstance();

    // --- non static area, call using Instance ---
	inline Config * getConfig();

private:
    static AppModules * mInstance;

    bool init();
    void shutdown();

	Config * mConfig;

    AppModules();
    ~AppModules();
};

//****************************************************************************
//
//****************************************************************************
/*static*/ inline AppModules * AppModules::getInstance()
{
    return mInstance;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline Config * AppModules::getConfig()
{
	return mConfig;
}

#endif // _APP_MODULES_H_
