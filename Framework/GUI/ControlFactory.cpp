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
#include "PchGUI.h"
#include "ControlFactory.h"
#include "Win32ControlFactory.h"
#include "JuceControlFactory.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
ControlFactory* ControlFactory::createFactory(GUI* _gui)
{
    return Win32ControlFactory::createFactory(_gui);
    //return JuceControlFactory::createFactory(_gui);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void ControlFactory::destroyFactory(ControlFactory* _factory)
{
    DISPOSE(_factory);
}
