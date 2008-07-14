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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "PchNomosVE.h"

#include "FormMain.h"
#include "GUI.h"
#include "Window.h"
#include "Form.h"
#include "ControlFactory.h"
#include "GUIAdapterFactory.h"
#include "TestDataServerEvents.h"
#include "MemBufferRef.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
FormMain::FormMain() : Inherited(),
    mDataProvider(NULL),
    mGUICheckbox1(NULL),
    mGUICheckbox2(NULL),
    mGUICheckbox3(NULL)
{
}

/*virtual*/FormMain::~FormMain()
{
    done();
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool FormMain::init(GUI* _gui, const char* _name, GUIControl* _parent, const char* _fileName, const char* _context, NWSvcDataServer * _dataProvider)
{
    bool bOK = true;

    if (!isOk())
    {

        bOK = Inherited::init(_gui, _name, _parent, 0, _context);

        setSize(800,600);

        mDataProvider = _dataProvider;

        createControls();

        mGUI->getAdapterFactory()->createAdapter(mGUICheckbox1, "bool", "CLI_SVR_DLG_DATA", "SvrBool");
        mGUI->getAdapterFactory()->createAdapter(mGUICheckbox2, "bool", "CLI_SVR_DLG_DATA", "SvrBool");
        mGUI->getAdapterFactory()->createAdapter(mGUICheckbox3, "bool", "CLI_SVR_DLG_DATA", "SvrBool");

        REG_START(_dataProvider, "CLI_SVR_DLG_DATA");
            REG_CLI_OBJ("DelegateInt", mTestDelegateInt);
            REG_CLI_EVENTHANDLER("EVENT", mEventHandler);
            REG_CLI_DATALIST("DataList", mDataList);
        REG_END();

        mEventHandler.addNotificationCallback((NWSvcDataEventNotificationCallback*)this);
        mDataList.addNotificationCallback((NWSvcDataListCallback*)this);

        int testIntDelegate = mTestDelegateInt.get();

        TestDataServerEvent_RqAddItem ev;
        ev.mPos = 7;
        ev.mText = "Event Test";
        mEventHandler.sendEvent(&ev);

        /*
        NWGenericEventUserData2 ev2;
        ev2.mEventType = 999991;
        ev2.mUserParam1 = 666;
        ev2.mUserParam2 = 999;
        mEventHandler.sendEvent(&ev2);
        */
    }

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void FormMain::done()
{
    if (isOk())
    {
        mEventHandler.shutdown();

        Inherited::done();
    }
}

/*virtual*/ void FormMain::receiveEventMsg(int _eventType, MemBufferRef * _eventData)
{
    MemorySerializerIn serializerIn; // move this to sender and pass as parameter instead the MemBuffer
    serializerIn.setBuffer(_eventData->getPtr(), _eventData->getSize());

    switch(_eventType)
    {
    case NWSvcDataEvent::eEventType_GenericUserData2:
        {
            NWGenericEventUserData2 ev;
            ev.serializeIn(serializerIn);
            if(ev.mEventType == 999991)
            {
                ASSERT(ev.mUserParam1 == 666);
                ASSERT(ev.mUserParam2 == 999);
            }
            break;
        }

    case eTestDataServerEvents_SetValItem:
        {
            TestDataServerEvent_RqAddItem ev;
            ev.serializeIn(serializerIn);

            sDataListObj obj2(1, "Segundo");
            mDataList.createAsHead(obj2);

            sDataListObj obj(0, "Primero");
            mDataList.createAsHead(obj);

            sDataListObj obj3(3, "Cuarto");
            mDataList.createAsTail(obj3);
            break;
        }
    }
}

//****************************************************************************
//
//****************************************************************************
/*virtual*/ void FormMain::dataListEvent(StrId _contextName, StrId _objName, int _eventType, int _updatedElementIndex, int _objUniqueId)
{
    switch(_eventType)
    {
        case NWSvcDataList<sDataListObj>::eDataListNotification_ElemCreated:
        {
            switch(_objUniqueId)
            {
                case 3:
                {
                    sDataListObj obj(2, "Tercero");
                    mDataList.createAsPrevOf(2, obj);
                    break;
                }
                case 4:
                {
                    sDataListObj obj(4, "Quinto");
                    mDataList.createAsNextOf(3, obj);
                    break;
                }
                case 5:
                {
                    mDataList.eraseHead();
                    break;
                }
            }
            break;
        }

        case NWSvcDataList<sDataListObj>::eDataListNotification_ElemErased:
        {
            switch(_objUniqueId)
            {
                case 0:
                {
                    mDataList.eraseTail();
                    break;
                }
                case 1:
                {
                    mDataList.erase(2);
                    break;
                }
                case 2:
                {
                    mDataList.eraseHead();
                    break;
                }
                case 3:
                {
                    if(mDataList.getNumElems() > 0)
                        mDataList.erase(0);
                    break;
                }
                case 4:
                {
                    ASSERT(mDataList.getNumElems() == 0);
                    break;
                }
                case 5:
                {
                    sDataListObj obj(4, "Valor Cambiado");
                    mDataList.setVal(0, obj);
                    break;
                }
            }
            break;
        }

        case NWSvcDataList<sDataListObj>::eDataListNotification_ValueChanged:
        {
            if(mDataList.getNumElems() > 0)
                mDataList.eraseTail();
            break;
        }
    }
}

//****************************************************************************
//
//****************************************************************************
void FormMain::createControls()
{
    int x = 30;
    int sx = 200;
    int y = 100;
    int sy = 40;

    mGUICheckbox1 = mGUI->getControlFactory()->create<CheckBox>("CheckBox1", this);
    mGUICheckbox1->setSize(sx,sy);
    mGUICheckbox1->setPosition(x,y);
    mGUICheckbox1->setText("CheckBox1");
    //registerEventsFromControl(mGUICheckbox1);

    y += 50;
    mGUICheckbox2 = mGUI->getControlFactory()->create<CheckBox>("CheckBox2", this);
    mGUICheckbox2->setSize(sx,sy);
    mGUICheckbox2->setPosition(x,y);
    mGUICheckbox2->setText("CheckBox2");
    //registerEventsFromControl(mGUICheckbox2);

    y += 50;
    mGUICheckbox3 = mGUI->getControlFactory()->create<CheckBox>("CheckBox3", this);
    mGUICheckbox3->setSize(sx,sy);
    mGUICheckbox3->setPosition(x,y);
    mGUICheckbox3->setText("CheckBox3");
    //registerEventsFromControl(mGUICheckbox3);
}
