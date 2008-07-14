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
#ifndef GUI_H_
#define GUI_H_

class Window;
class Form;
class ControlFactory;
class GUIControl;
class GUIAdapterFactory;
class NWSvcDataServer;

//********************************************************************
//
//********************************************************************
class GUI
{
public:
    GUI             ();
    virtual ~GUI    ()                      { GUI::done(); }

    bool            init            (const char* _name = NULL);
    bool            isOk            () const  { return mInit; }
    void            done            ();

    Window*         getMainWindow   ();

    Form*           createForm(const char* _fileName, GUIControl* _parent);

    ControlFactory* getControlFactory ();
    GUIAdapterFactory* getAdapterFactory();

    inline void setDataProvider(NWSvcDataServer * _svcDataProvider);
    inline NWSvcDataServer * getDataProvider() const;

    static int      getVersionNumber();

private:
    bool            mInit : 1;
    Window*         mRoot;
    ControlFactory* mControlFactory;
    GUIAdapterFactory* mAdapterFactory;
    NWSvcDataServer * mDataProvider;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
inline void GUI::setDataProvider(NWSvcDataServer * _svcDataProvider)
{
    ASSERT(mDataProvider == 0);
    mDataProvider = _svcDataProvider;
}

inline NWSvcDataServer * GUI::getDataProvider() const
{
    return mDataProvider;
}

#endif
