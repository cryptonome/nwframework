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
