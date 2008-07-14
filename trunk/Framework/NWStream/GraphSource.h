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
#ifndef GRAPHSOURCE_H_
#define GRAPHSOURCE_H_

#include "INWGraph.h"

class NWStreamGroupWrite;

//********************************************************************
//
//********************************************************************
class GraphSource : public INWGraph
{
public:
    GraphSource  ();
    virtual    ~GraphSource ()                      { GraphSource::done(); }

    virtual bool          init          ();
    bool                  isOk          () const  { return mInit; }
    virtual void          done          ();

    virtual double getDuration() const { return 1.0; }
    virtual double getCurrentPosition() const { return 0.0; }
    virtual void setCurrentPosition(double _pos) { }

protected:
    NWStreamGroupWrite* mStreamGroupOutput;

private:
    // INWGraph
    virtual bool connectInputGraph(INWGraph* _graph) { return false; }
    virtual INWStreamGroupWrite* getStreamGroupOutput();

    bool          mInit : 1;
};

#endif
