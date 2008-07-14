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
