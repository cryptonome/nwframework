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
