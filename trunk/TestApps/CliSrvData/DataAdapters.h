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
#ifndef DATA_ADAPTERS_H
#define DATA_ADAPTERS_H

#include "NWSvcDataObject.h"

class CheckBox;

class AdapterBoolCheckBox : public NWSvcDataObjNotificationCallback
{
public:
    AdapterBoolCheckBox();
    ~AdapterBoolCheckBox();

    bool init(NWSvcDataObj<bool> * _boolVar, CheckBox * _checkBox);
    void shutdown();

    virtual void update(const char * _contextName, const char * _objName);

private:
    bool mInitd;
    NWSvcDataObj<bool> * mBoolVar;
    CheckBox * mCheckBox;
};


#endif // DATA_ADAPTERS_H
