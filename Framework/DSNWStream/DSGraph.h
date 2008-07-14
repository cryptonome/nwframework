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
