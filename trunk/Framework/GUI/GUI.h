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
