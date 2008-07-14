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
