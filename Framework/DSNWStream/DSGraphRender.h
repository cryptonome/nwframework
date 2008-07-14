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
#ifndef DSGRAPHRENDER_H_
#define DSGRAPHRENDER_H_

#include "DSGraph.h"
#include <list>

class NWStreamGroupRead;
class INWStreamReader;

struct IBaseFilter;
struct IPin;

//********************************************************************
//
//********************************************************************
class DSGraphRender : public DSGraph
{
public:
    DSGraphRender  ();
    virtual    ~DSGraphRender ()                      { DSGraphRender::done(); }

    virtual bool          init            ();
    virtual void          done            ();

    // DSGraph
    /*virtual bool connectInputGraph(INWGraph* _graph);
    virtual bool inputGraphConnected() { return true; }*/
    virtual bool build();
    virtual bool postBuild();
    virtual bool start();
    virtual void stop();
    virtual void pause();

    void addStream(INWStreamReader* _stream);

protected:
    typedef DSGraph Inherited;

    virtual bool createRenderGraph();
    virtual bool createFilters() = 0;
    virtual bool connectSourceFilter(IBaseFilter* _filter, IPin* _pin, INWStreamReader* _stream) = 0;
    virtual bool renderGraphCreated() { return true; }

    NWStreamGroupRead* mStreamGroupInput;

    //INWGraph* mGraphInput;
    std::list<INWStreamReader*> mStreamsToBuild;
    bool mBuilt;
};

#endif
