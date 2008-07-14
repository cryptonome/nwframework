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
#ifndef FORMMAIN_H_
#define FORMMAIN_H_

#include "Form.h"

#include "NWSvcDataObject.h"
#include "NWSvcDataList.h"
#include "DataAdapters.h"
#include "TestDataServer.h"

class Window;
class NWSvcDataServer;

//********************************************************************
//
//********************************************************************
class FormMain : public Form, public NWSvcDataEventNotificationCallback, public NWSvcDataListCallback
{
public:
    FormMain   ();
    virtual ~FormMain();

    virtual bool          init        (GUI* _gui, const char* _name, GUIControl* _parent, const char* _fileName, const char* _context, NWSvcDataServer * _dataProvider);
    virtual void          done        ();

    virtual void receiveEventMsg(int _eventType, MemBufferRef * _eventData);
    virtual void dataListEvent(StrId _contextName, StrId _objName, int _eventType, int _updatedElementIndex, int _objUniqueId);

private:
    typedef Form Inherited;

    NWSVR_OBJ(FormMain);

    NWSvcDataServer * mDataProvider;

    CheckBox * mGUICheckbox1;
    CheckBox * mGUICheckbox2;
    CheckBox * mGUICheckbox3;

    NWSVC_DATA_OBJ(int,TestDelegateInt);
    NWSVC_DATA_EVENTHANDLER(mEventHandler);
    NWSVC_DATALIST(sDataListObj, mDataList);

    void createControls();
};

#endif
