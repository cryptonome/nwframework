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
#ifndef WINDOWMENU_H_
#define WINDOWMENU_H_

#include <string>
#include <vector>

//********************************************************************
//
//********************************************************************
class WindowMenu
{
public:
    WindowMenu ();
    virtual    ~WindowMenu()                      { WindowMenu::done(); }

    virtual bool          init        ();
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    WindowMenu* addItem(const char* _name, const char* _id);
    WindowMenu* getItem(int _index);
    int getNumItems() const { return (int)mItems.size(); }

    const char* getName() const { return mName.c_str(); }
    const char* getId() const { return mId.c_str(); }

    void setNumId(int _numId) { mNumId = _numId; }
    WindowMenu* getItemByNumId(int _numId);

private:
    bool          mInit : 1;

    bool          init        (const char* _name, const char* _id);

    std::string mName;
    std::string mId;
    int mNumId;

    std::vector<WindowMenu*> mItems;
};

#endif
