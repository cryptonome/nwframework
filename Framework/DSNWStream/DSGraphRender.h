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
