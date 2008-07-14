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
#ifndef DSGRAPHRENDERSCREEN_H_
#define DSGRAPHRENDERSCREEN_H_

#include "DSGraphRender.h"

//********************************************************************
//
//********************************************************************
class DSGraphRenderScreen : public DSGraphRender
{
public:
    DSGraphRenderScreen  ();
    virtual    ~DSGraphRenderScreen ()                      { DSGraphRenderScreen::done(); }

    virtual bool          init                  (void* hWnd);
    virtual void          done                  ();

    void resized();

private:
    typedef DSGraphRender Inherited;

    // DSGraphRender
    virtual bool createFilters();
    virtual bool connectSourceFilter(IBaseFilter* _filter, IPin* _pin, INWStreamReader* _stream);
    virtual bool renderGraphCreated();

    void* mhWnd;
};

#endif
