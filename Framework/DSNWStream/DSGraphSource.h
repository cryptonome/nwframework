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
