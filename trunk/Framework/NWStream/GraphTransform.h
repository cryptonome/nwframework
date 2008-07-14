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
#ifndef GRAPHTRANSFORM_H_
#define GRAPHTRANSFORM_H_

#include "INWGraph.h"

class NWStreamGroupWrite;

//********************************************************************
//
//********************************************************************
class GraphTransform : public INWGraph
{
public:
    GraphTransform  ();
    virtual    ~GraphTransform ()                      { GraphTransform::done(); }

    virtual bool          init             ();
    bool                  isOk             () const  { return mInit; }
    virtual void          done             ();


    // INWGraph
    virtual bool connectInputGraph(INWGraph* _graph);
    virtual INWStreamGroupWrite* getStreamGroupOutput();

private:
    bool          mInit : 1;

    NWStreamGroupWrite* mStreamGroupOutput;
};

#endif
