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
