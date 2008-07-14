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
