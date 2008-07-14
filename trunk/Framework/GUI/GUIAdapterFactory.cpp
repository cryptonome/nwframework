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


#include "PchGUI.h"

#include "GUIAdapterFactory.h"
#include "GUIControl.h"
#include "GUIAdapterCreator.h"
#include "GUI.h"
#include "GACheckBox.h"
#include "GAEditBox.h"
#include "GAComboBox.h"
#include "GATabControl.h"


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
GUIAdapterFactory::GUIAdapterFactory() :
    mInit(false)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool GUIAdapterFactory::init(GUI * _gui)
{
    bool bOK = true;

    if (!isOk())
    {
        mGUI = _gui;

        registerBaseGUIAdapterCreators();

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::done()
{
    if (isOk())
    {
        for ( std::list<IGUIAdapterCreator*>::iterator it = mCreators.begin() ; it != mCreators.end() ; ++it )
        {
            IGUIAdapterCreator* creator = *it;
            DISPOSE(creator);
        }
        mCreators.clear();

        mInit = false;
    }
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::registerAdapterCreator(IGUIAdapterCreator* _creator)
{
    mCreators.push_back(_creator);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IGUIAdapter* GUIAdapterFactory::createAdapter(GUIControl* _control, StrId _dataTypeString, StrId _dataObjContext, StrId _dataObjName)
{
    IGUIAdapter* guiAdapter = 0;

    for ( std::list<IGUIAdapterCreator*>::iterator it = mCreators.begin() ; guiAdapter == 0 && it != mCreators.end() ; ++it )
    {
        IGUIAdapterCreator* creator = *it;
        if ( _control->getControlType() == creator->getControlType() &&
             _dataTypeString == creator->getDataType() )
        {
             guiAdapter = creator->create(_control, mGUI->getDataProvider(), _dataObjContext, _dataObjName);

             ASSERT(guiAdapter);
             _control->setAdapter(guiAdapter);
        }
    }

    return guiAdapter;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void GUIAdapterFactory::registerBaseGUIAdapterCreators()
{
    REGISTER_ADAPTER_CREATOR(this, GACheckBoxBool);
    REGISTER_ADAPTER_CREATOR(this, GACheckBoxString);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxString);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxInt);
    REGISTER_ADAPTER_CREATOR(this, GAEditBoxNWIP);
    REGISTER_ADAPTER_CREATOR(this, GAComboBoxInt);
    REGISTER_ADAPTER_CREATOR(this, GAComboBoxString);
    REGISTER_ADAPTER_CREATOR(this, GATabControlString);
}
