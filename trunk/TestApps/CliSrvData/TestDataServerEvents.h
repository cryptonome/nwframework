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
#ifndef TEST_DATA_SERVER_EVENTS_H
#define TEST_DATA_SERVER_EVENTS_H

#include "NWSvcDataObject.h"

enum eTestDataServerEvents
{
    eTestDataServerEvents_Null = NWSvcDataEvent::eEventType_User,
    eTestDataServerEvents_SetValItem
};

class TestDataServerEvent_RqAddItem : public NWSvcDataEvent
{
public:
    int mPos;
    StrId mText;

    TestDataServerEvent_RqAddItem() : NWSvcDataEvent(eTestDataServerEvents_SetValItem),
        mPos(-1),
        mText("")
    {
    }

    virtual void serializeIn(MemorySerializerIn & _serializerIn)
    {
        mPos = _serializerIn.getInt();
        mText = _serializerIn.getString();
    }

    virtual void serializeOut(MemorySerializerOut & _serializerOut) const
    {
        _serializerOut.addInt(mPos);
        _serializerOut.addString(mText.c_str());
    }
};


#endif // TEST_DATA_SERVER_EVENTS_H
