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
#ifndef INWGRAPH_H_
#define INWGRAPH_H_

class INWStreamGroupWrite;

//********************************************************************
//
//********************************************************************
class INWGraph
{
public:
    virtual ~INWGraph () { }

    /*virtual bool connectInputGraph(INWGraph* _graph) = 0;   // Se utiliza para conectar la entrada de un grafo a la salida de otro grafo. Se ha de llammar siempre al grafo con la entrada
    virtual bool inputGraphConnected() = 0; // Se llama desde el grafo con la entrada al grafo con la salida una vez esta listo el grafo con la entrada*/
    
    virtual INWStreamGroupWrite* getStreamGroupOutput() = 0;

    virtual bool build() = 0;
    virtual bool postBuild() = 0;
    virtual bool start() = 0;
    virtual void stop() = 0;
};

#endif
