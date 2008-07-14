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
#ifndef DSGRAPHSOURCE_H_
#define DSGRAPHSOURCE_H_

#include "DSGraph.h"

//********************************************************************
//
//********************************************************************
class DSGraphSource : public DSGraph
{
public:
    DSGraphSource  ();
    virtual    ~DSGraphSource ()                      { DSGraphSource::done(); }

    virtual bool          init            ();
    virtual void          done            ();

    // DSGraph
    virtual bool build();
    virtual bool postBuild();
    virtual INWStreamGroupWrite* getStreamGroupOutput();

protected:
    typedef DSGraph Inherited;

    // A implementar por los grafos que heredan de este
    virtual bool connectFiltersRender() = 0;

    // INWGraph
    //virtual bool inputGraphConnected();

    NWStreamGroupWrite* mStreamGroupOutput;

    bool mBuilt;
};

#endif
