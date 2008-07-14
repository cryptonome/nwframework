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


#ifndef NW_SVR_DATA_CONTEXT_H
#define NW_SVR_DATA_CONTEXT_H

//#include "NWDelegate.h"
//#include "NWSvcDataServer.h"

#include <map>
#include <string>

class NWSvcDataObject;
class NWSvcDataEventHandler;

//****************************************************************************
//
//****************************************************************************
class NWSvcDataContext
{
public:
    typedef std::multimap<std::string, NWSvcDataObject *>::iterator HashObjectsIt;
    typedef std::pair<HashObjectsIt, HashObjectsIt> HashObjectsItRange;

    typedef std::multimap<std::string, NWSvcDataEventHandler *>::iterator HashEventHandlersIt;
    typedef std::pair<HashEventHandlersIt, HashEventHandlersIt> HashEventHandlersItRange;

    NWSvcDataContext();
    virtual ~NWSvcDataContext();

    bool init(const char * _name);
    void shutdown();
    inline bool isOk();
        
    void regObject(const char * _objName, NWSvcDataObject * _svcDataObj);
    void unregObject(const char * _objName, NWSvcDataObject * _svcDataObj);
    HashObjectsItRange findObjects(const char * _objName);

    void regEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler);
    void unregEventHandler(const char * _handlerName, NWSvcDataEventHandler * _svcDataEventHandler);
    HashEventHandlersItRange findEventHandlers(const char * _handlerName);

    inline int getNumObjects(){return (int)mHashObjects.size();}
    inline int getNumEventHandlers(){return (int)mHashEventHandlers.size();}
    inline std::string getName();

private:
    bool mInitd;
    std::string mName;

    std::multimap<std::string, NWSvcDataObject *> mHashObjects;
    typedef std::multimap<std::string, NWSvcDataObject *>::value_type HashObjectsPair;
    std::multimap<std::string, NWSvcDataEventHandler *> mHashEventHandlers;
    typedef std::multimap<std::string, NWSvcDataEventHandler *>::value_type HashEventHandlersPair;
};

inline bool NWSvcDataContext::isOk()
{
    return mInitd;
}

inline std::string NWSvcDataContext::getName()
{
    return mName;
}

#endif // NW_SVR_DATA_CONTEXT_H
