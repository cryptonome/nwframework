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
#ifndef DSGRAPHTRANSFORM_H_
#define DSGRAPHTRANSFORM_H_

#include "DSGraph.h"

class NWStreamGroupRead;

//********************************************************************
//
//********************************************************************
class DSGraphTransform : public DSGraph
{
public:
    DSGraphTransform  ();
    virtual    ~DSGraphTransform ()                      { DSGraphTransform::done(); }

    virtual bool          init               ();
    virtual void          done               ();

    // DSGraph
    /*virtual bool connectInputGraph(INWGraph* _graph);
    virtual bool inputGraphConnected();*/
    INWStreamGroupWrite* getStreamGroupOutput();
    virtual bool build();
    virtual bool postBuild();
    virtual bool start();

protected:
    typedef DSGraph Inherited;

    // A implementar por los grafos que heredan de este
    virtual bool connectFiltersRender() = 0;

    virtual bool createTransformGraph() { return true; }

    NWStreamGroupRead* mStreamGroupInput;
    INWGraph* mGraphInput;

    NWStreamGroupWrite* mStreamGroupOutput;

    bool mBuilt;
};

#endif
