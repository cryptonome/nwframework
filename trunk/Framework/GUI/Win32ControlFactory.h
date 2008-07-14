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
#ifndef WIN32CONTROLFACTORY_H_
#define WIN32CONTROLFACTORY_H_

#include "ControlFactory.h"
#include <windows.h>
#include <list>

class Win32ControlData;

//********************************************************************
//
//********************************************************************
class Win32ControlFactory : public ControlFactory
{
public:
    Win32ControlFactory  ();
    virtual    ~Win32ControlFactory ()                      { Win32ControlFactory::done(); }

    virtual bool          init                  (GUI* _gui);
    bool                  isOk                  () const  { return mInit; }
    virtual void          done                  ();

    // ControlFactory
    virtual GUIControl*   create  (StrId _type, const char* _name, GUIControl* _parent);

    static ControlFactory* createFactory(GUI* _gui);

    // Win32
    void registerControl(GUIControl* _control);
    void unregisterControl(GUIControl* _control);
    Win32ControlData* getWin32Control(HWND hWnd);

private:
    bool          mInit : 1;
    GUI* mGUI;

    struct ControlElement
    {
        GUIControl* mControl;
        HWND mhWnd;
    };
    // TODO: use a hash
    std::list<ControlElement> mRegisteredControls;
};

#endif
