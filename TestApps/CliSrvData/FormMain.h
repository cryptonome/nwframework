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
