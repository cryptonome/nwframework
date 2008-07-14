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
#ifndef DSGRAPHSOURCEFILE_H_
#define DSGRAPHSOURCEFILE_H_

#include "DSGraphSource.h"
#include "DSUtils.h"
#include <string>

class DSFilterRenderStreamVideo;
class DSFilterRenderStreamAudio;

//********************************************************************
//
//********************************************************************
class DSGraphSourceFile : public DSGraphSource, public DSUtils::IFilterSwap
{
public:
    DSGraphSourceFile  ();
    virtual    ~DSGraphSourceFile ()                      { DSGraphSourceFile::done(); }

    virtual bool          init                (const char* _fileName);
    virtual void          done                ();

    void setFileName(const char* _fileName);

    // DSGraphSource
    virtual bool build();

private:
    typedef DSGraphSource Inherited;

    bool createSourceFileGraph();
    void destroySourceFileGraph();

    // DSGraphSource
    virtual bool connectFiltersRender();

    // IFilterSwap
    virtual IBaseFilter* swap(IBaseFilter* _filter, IPin* _pin, IPin*& pinNew_);

    std::string mFileName;
    DSFilterRenderStreamVideo* mFilterRenderVideo;
    DSFilterRenderStreamAudio* mFilterRenderAudio;

    struct FilterRender
    {
        IBaseFilter* filter;
        IPin* pinToConnect;
    };
    std::list<FilterRender> mFiltersRender;

    bool mFiltersRenderConnected;
};

#endif
