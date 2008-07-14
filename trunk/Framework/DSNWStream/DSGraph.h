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
#ifndef DSGRAPH_H_
#define DSGRAPH_H_

#include "INWGraph.h"
#include "DSGraphTypes.h"

class INWStreamGroupWrite;
class NWStreamGroupWrite;
struct IGraphBuilder;
struct IMediaControl;
struct IMediaEvent;
struct IMediaPosition;

//********************************************************************
//
//********************************************************************
class DSGraph : public INWGraph
{
public:
    DSGraph    ();
    virtual    ~DSGraph   ()                      { DSGraph::done(); }

    virtual bool          init        ();
    bool                  isOk        () const  { return mInit; }
    virtual void          done        ();

    // INWGraph
    virtual INWStreamGroupWrite* getStreamGroupOutput() { return 0; }
    //virtual bool connectInputGraph(INWGraph* _graph);
    virtual bool start();
    virtual void stop();
    
    virtual void waitEnd();
    virtual double getDuration() const;
    virtual double getCurrentPosition() const;
    virtual void setCurrentPosition(double _pos);

    IGraphBuilder* getGraphBuilder() { return mGraphBuilder; }

protected:

    bool mStarted;
    IGraphBuilder*  mGraphBuilder;
    IMediaControl*  mMediaControl;
    IMediaEvent*    mMediaEvent;
    IMediaPosition* mMediaPosition;

private:
    bool createBaseGraphComponents();
    void destroyBaseGraphComponents();

    bool          mInit : 1;
};

#endif
