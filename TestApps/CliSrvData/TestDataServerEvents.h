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
