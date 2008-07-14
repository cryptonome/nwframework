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
#ifndef DSUTILS_H_
#define DSUTILS_H_

#include <list>
#include <guiddef.h>
#include "DSUtilsObj.h"

struct IGraphBuilder;
struct IMediaControl;
struct IMediaEvent;
struct IMediaFilter;
struct IEnumFilters;
struct IFilterGraph;
struct IMediaPosition;

//********************************************************************
// DSUtils
//********************************************************************
namespace DSUtils
{
    ////////////////////////
    // Create
    ////////////////////////
    IGraphBuilder* createGraphBuilder();
    IMediaControl* createMediaControl(IGraphBuilder* _graphBuilder);
    IMediaEvent* createMediaEvent(IGraphBuilder* _graphBuilder);
    IMediaFilter* createMediaFilter(IGraphBuilder* _graphBuilder);
    IEnumFilters* createEnumFilters(IGraphBuilder* _graphBuilder);
    IMediaPosition* createMediaPosition(IGraphBuilder* _graphBuilder);

    ////////////////////////
    // Destroy
    ////////////////////////
    void destroyFiltersInGraph(IGraphBuilder* _graphBuilder);

    ////////////////////////
    // Render
    ////////////////////////
    enum ERenderFileMode
    {
        E_RENDER_FILE_MODE_FULL = 0,            // Se deben haber podido crear render para todos los streams para que no devuelva error
        E_RENDER_FILE_MODE_PARTIAL_ALLOWED,     // Con que 1 stream se haya completado correctamente no devuelve error
    };
    bool renderFile(IGraphBuilder* _graphBuilder, const char* _fileName, ERenderFileMode _mode);

    ////////////////////////
    // Filters
    ////////////////////////
    void getPins(IBaseFilter* _filter, DSPinList& _pins, bool bInput = true, bool bOutput = true);
    void getOutputPins(IBaseFilter* _filter, DSPinList& _pins);
    void getInputPins(IBaseFilter* _filter, DSPinList& _pins);
    IPin* getInputPin(IBaseFilter* _filter);
    IPin* getOutputPin(IBaseFilter* _filter);
    int getNumOutputPins(IBaseFilter* _filter);
    int getNumInputPins(IBaseFilter* _filter);
    void getFilters(IGraphBuilder* _graphBuilder, DSFilterList& _filters);
    void getFiltersRender(IGraphBuilder* _graphBuilder, DSFilterList& _filters);
    IFilterGraph* getFilterGraph(IBaseFilter* _filter);
    bool isInputPinConnected(IBaseFilter* _filter);
    void showFilterProperties(IBaseFilter* _filter);

    ////////////////////////
    // Pins
    ////////////////////////
    IPin* getConnectedTo(IPin* _pin);
    bool isConnected(IPin* _pin);
    void disconnect(IPin* _pin);
    IFilterGraph* getFilterGraph(IPin* _pin);

    ////////////////////////
    // Debug
    ////////////////////////
    unsigned int enableExternalDebugGraph(IGraphBuilder* _graphBuilder);

    ////////////////////////
    // Filter Swap
    ////////////////////////
    class IFilterSwap
    {
    public:
        virtual ~IFilterSwap() { }

        // A new filter must be returned and the _pin overwrited with
        // the pin of the filter to connect
        virtual IBaseFilter* swap(IBaseFilter* _filter, IPin* _pin, IPin*& pinNew_) = 0;
    };
    bool swapFiltersRender(IGraphBuilder* _graphBuilder, IFilterSwap* _filterSwap, bool _connectNew = true);


    ////////////////////////
    // GUID
    ////////////////////////
    bool areEqual(const GUID& _guid1, const GUID& _guid2);


    ////////////////////////
    // Clock
    ////////////////////////
    void setGraphNoClock(IGraphBuilder* _graphBuilder);

} // DSUtils

#endif
