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
#include "PchDSUtils.h"

#include "DSUtils.h"
#include "StrUtils.h"
#include <dshow.h>

//--------------------------------------------------------------------
// http://msdn2.microsoft.com/en-us/library/ms787252.aspx
//--------------------------------------------------------------------
static HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
    IMoniker * pMoniker = NULL;
    IRunningObjectTable *pROT = NULL;

    if (FAILED(GetRunningObjectTable(0, &pROT))) 
    {
        return E_FAIL;
    }
    
    const size_t STRING_LENGTH = 256;

    char buffer[256];
    sprintf(buffer,"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
    std::wstring str = StrUtils::multiCharToUniChar(buffer);
    
    HRESULT hr = CreateItemMoniker(L"!", str.c_str(), &pMoniker);
    if (SUCCEEDED(hr)) 
    {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }
    pROT->Release();

    *pdwRegister = 1;
    
    return S_OK;
}


//********************************************************************
// DSUtils
//********************************************************************
namespace DSUtils
{


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IGraphBuilder* createGraphBuilder()
{
    IGraphBuilder* graphBuilder = 0;
    
    if ( FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **) &graphBuilder)) )
        graphBuilder = NULL;

    return graphBuilder;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
template<typename T>
T* createFromGraph(IGraphBuilder* _graphBuilder)
{
    T* obj = 0;
    
    if ( FAILED(_graphBuilder->QueryInterface(&obj)) )
        obj = 0;

    return obj;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IMediaControl* createMediaControl(IGraphBuilder* _graphBuilder)
{
    return createFromGraph<IMediaControl>(_graphBuilder);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IMediaEvent* createMediaEvent(IGraphBuilder* _graphBuilder)
{
    return createFromGraph<IMediaEvent>(_graphBuilder);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IMediaFilter* createMediaFilter(IGraphBuilder* _graphBuilder)
{
    return createFromGraph<IMediaFilter>(_graphBuilder);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IEnumFilters* createEnumFilters(IGraphBuilder* _graphBuilder)
{
    IEnumFilters* enumFilters = 0;
    
    if ( _graphBuilder->EnumFilters(&enumFilters) != S_OK )
        enumFilters = 0;

    return enumFilters;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IMediaPosition* createMediaPosition(IGraphBuilder* _graphBuilder)
{
    return createFromGraph<IMediaPosition>(_graphBuilder);
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void destroyFiltersInGraph(IGraphBuilder* _graphBuilder)
{
    if (_graphBuilder)
    {
        IEnumFilters* enumFilters = 0;
        HRESULT hr = _graphBuilder->EnumFilters(&enumFilters);
        if (SUCCEEDED(hr))
        {
            IBaseFilter* filter = NULL;
            while (S_OK == enumFilters->Next(1, &filter, NULL))
             {
                 // Remove the filter.
                 _graphBuilder->RemoveFilter(filter);
                 // Reset the enumerator.
                 enumFilters->Reset();
                 DS_RELEASE(filter);
            }
            DS_RELEASE(enumFilters);
        }
    }
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool renderFile(IGraphBuilder* _graphBuilder, const char* _fileName, ERenderFileMode _mode)
{
    bool bOK = true;

    std::wstring fileName = StrUtils::multiCharToUniChar(_fileName);
    HRESULT hRes = _graphBuilder->RenderFile(fileName.c_str(),0);

    if ( _mode == E_RENDER_FILE_MODE_FULL )
        bOK = hRes == S_OK;
    else
        bOK = hRes == VFW_S_AUDIO_NOT_RENDERED || hRes == VFW_S_VIDEO_NOT_RENDERED || hRes == VFW_S_PARTIAL_RENDER; 

    return bOK;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void getPins(IBaseFilter* _filter, DSPinList& _pins, bool bInput, bool bOutput)
{
    IEnumPins* enumPins = 0;
    if ( _filter->EnumPins(&enumPins) == S_OK )
    {
        IPin* pin = 0;
        ULONG fetched = 0;
        while ( enumPins->Next(1,&pin,&fetched) == S_OK && fetched )
        {
            PIN_DIRECTION pinDir;
            if ( pin->QueryDirection(&pinDir) == S_OK )
            {
                if ( bInput && pinDir == PINDIR_INPUT ||
                     bOutput && pinDir == PINDIR_OUTPUT )
                    _pins.push_back(pin);
            }
            DS_RELEASE(pin);
        }
        DS_RELEASE(enumPins);
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void getOutputPins(IBaseFilter* _filter, DSPinList& _pins)
{
    getPins(_filter,_pins,false,true);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void getInputPins(IBaseFilter* _filter, DSPinList& _pins)
{
    getPins(_filter,_pins,true,false);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IPin* getInputPin(IBaseFilter* _filter)
{
    IPin* pin = 0;

    DSPinList pinList;
    getInputPins(_filter,pinList);
    if ( pinList.size() == 1 )
    {
        pin = pinList.front().getObj();
        pin->AddRef();
    }

    return pin;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IPin* getOutputPin(IBaseFilter* _filter)
{
    IPin* pin = 0;

    DSPinList pinList;
    getOutputPins(_filter,pinList);
    if ( pinList.size() == 1 )
    {
        pin = pinList.front().getObj();
        pin->AddRef();
    }

    return pin;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int getNumOutputPins(IBaseFilter* _filter)
{
    DSPinList pins;
    getOutputPins(_filter,pins);
    return (int)pins.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int getNumInputPins(IBaseFilter* _filter)
{
    DSPinList pins;
    getInputPins(_filter,pins);
    return (int)pins.size();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void getFilters(IGraphBuilder* _graphBuilder, DSFilterList& _filters)
{
    IEnumFilters* enumFilters = createEnumFilters(_graphBuilder);

    if ( enumFilters != 0 )
    {
        IBaseFilter* filter = 0;
        ULONG fetched = 0;
        while ( enumFilters->Next(1,&filter,&fetched) == S_OK && fetched == 1 )
        {
            DSFilter f(filter);
            _filters.push_back(f);
            DS_RELEASE(filter);
        }
    }
    DS_RELEASE(enumFilters);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void getFiltersRender(IGraphBuilder* _graphBuilder, DSFilterList& _filters)
{
    IEnumFilters* enumFilters = createEnumFilters(_graphBuilder);

    if ( enumFilters != 0 )
    {
        IBaseFilter* filter = 0;
        ULONG fetched = 0;
        while ( enumFilters->Next(1,&filter,&fetched) == S_OK && fetched == 1 )
        {
            if ( getNumOutputPins(filter) == 0 )
            {
                DSFilter f(filter);

                _filters.push_back(f);

                int a = 0;
            }

            DS_RELEASE(filter);
        }
    }
    DS_RELEASE(enumFilters);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IFilterGraph* getFilterGraph(IBaseFilter* _filter)
{
    IFilterGraph* graph = 0;

    FILTER_INFO fi;
    memset(&fi,0,sizeof(fi));
    if ( _filter->QueryFilterInfo(&fi) == S_OK )
    {
        graph = fi.pGraph;
    }

    return graph;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool isInputPinConnected(IBaseFilter* _filter)
{
    IPin* pin = DSUtils::getInputPin(_filter);
    bool connected = pin != 0 && DSUtils::isConnected(pin);
    DS_RELEASE(pin);

    return connected;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IPin* getConnectedTo(IPin* _pin)
{
    IPin* pin = 0;

    if ( _pin->ConnectedTo(&pin) != S_OK )
        pin = 0;

    return pin;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool isConnected(IPin* _pin)
{
    bool connected = false;

    IPin* pin = getConnectedTo(_pin);
    connected = pin != 0;
    DS_RELEASE(pin);

    return connected;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void disconnect(IPin* _pin)
{
    IFilterGraph* graph = getFilterGraph(_pin);

    if ( graph )
    {
        IPin* pinConnected = DSUtils::getConnectedTo(_pin);
        if ( pinConnected )
        {
            graph->Disconnect(_pin);
            graph->Disconnect(pinConnected);
        }
        DS_RELEASE(pinConnected);
    }

    DS_RELEASE(graph);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
IFilterGraph* getFilterGraph(IPin* _pin)
{
    IFilterGraph* graph = 0;

    // Get the filter
    IBaseFilter* filter = 0;
    PIN_INFO pi;
    memset(&pi,0,sizeof(pi));
    if ( _pin->QueryPinInfo(&pi) == S_OK )
        filter = pi.pFilter;

    if ( filter )
        graph = getFilterGraph(filter);

    DS_RELEASE(filter);

    return graph;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
unsigned int enableExternalDebugGraph(IGraphBuilder* _graphBuilder)
{
    return 0;
    // TODO: Usar el fichero de configuracion global para poder activar/desactivar esta funcionalidad
    DWORD id = 0xffffffff;
    
    HRESULT hr = AddToRot(_graphBuilder,&id);
    if ( hr != S_OK )
        id = 0xffffffff;

    return id;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool swapFilters(IGraphBuilder* _graphBuilder, IFilterSwap* _filterSwap, DSFilterList& _filters, bool _connectNew)
{
    bool bOK = true;

    // Swap each filter
    for ( DSFilterList::iterator it = _filters.begin() ; bOK && it != _filters.end() ; ++it )
    {
        IBaseFilter* filter = (*it).getObj();
        bool disconnected = false;

        // Get the pins
        DSPinList pins;
        getPins(filter,pins);

        // Disconnect the filters connected to this pin and swap it by a new one using the callback
        for ( DSPinList::iterator it = pins.begin() ; bOK && it != pins.end() ; ++it )
        {
            IPin* pin = (*it).getObj();
            IPin* pinConnected = getConnectedTo(pin);
            bOK = pinConnected != 0;

            // Create an instance of the new filter and connect it to the other pin
            if ( bOK )
            {
                IPin* pinSwap = 0;
                IBaseFilter* swapFilter = _filterSwap->swap(filter,pin,pinSwap);
                if ( swapFilter != 0 && pinSwap != 0 )
                {
                   bOK = _graphBuilder->AddFilter(swapFilter,L"") == S_OK;

                    // Disconnect the original pins
                    if ( bOK )
                        disconnect(pin);

                    // Connect the filter to the pinConnected
                    if ( bOK )
                    {
                        /*IEnumMediaTypes* enumMediaTypes = NULL;
                        if ( pinConnected->EnumMediaTypes(&enumMediaTypes) == S_OK )
                        {
                            AM_MEDIA_TYPE* mediaType;
                            while ( enumMediaTypes->Next(1,&mediaType,NULL) == S_OK )
                            {
                                int a = 0;
                            }
                            DS_RELEASE(enumMediaTypes);
                        }*/

                        if ( _connectNew )
                            bOK = _graphBuilder->Connect(pinConnected,pinSwap) == S_OK;
                        //ASSERT(bOK);
                        if ( bOK )
                            disconnected = true;
                        // Reconnect the original filter and destroy the swap filter
                        else
                        {
                            _graphBuilder->Connect(pinConnected,pin);
                            _graphBuilder->RemoveFilter(swapFilter);
                            bOK = true;
                        }
                    }
                }

                DS_RELEASE(pinSwap);
                //DS_RELEASE(swapFilter);
            }

            if ( !bOK )
                disconnected = false;

            DS_RELEASE(pinConnected);
        }

        // Destroy the filter instance from the graph
        if ( disconnected )
        {
            _graphBuilder->RemoveFilter(filter);
        }
    }

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool swapFiltersRender(IGraphBuilder* _graphBuilder, IFilterSwap* _filterSwap, bool _connectNew)
{
    // Get render filters
    DSFilterList filters;
    getFiltersRender(_graphBuilder,filters);

    bool bOK = swapFilters(_graphBuilder,_filterSwap,filters,_connectNew);

    return bOK;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool areEqual(const GUID& _guid1, const GUID& _guid2)
{
    return memcmp(&_guid1,&_guid2,sizeof(GUID)) == 0;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void setGraphNoClock(IGraphBuilder* _graphBuilder)
{
    IMediaFilter* mediaFilter = createMediaFilter(_graphBuilder);
    bool bOK = mediaFilter != 0;
    if ( bOK )
    {
        bOK = mediaFilter->SetSyncSource(NULL) == S_OK;
        DS_RELEASE(mediaFilter);
    }
}



//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void showFilterProperties(IBaseFilter* _filter)
{
    /* Obtain the filter's IBaseFilter interface. (Not shown) */
    ISpecifyPropertyPages *pProp;
    HRESULT hr = _filter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
    if (SUCCEEDED(hr)) 
    {
        // Get the filter's name and IUnknown pointer.
        FILTER_INFO FilterInfo;
        hr = _filter->QueryFilterInfo(&FilterInfo); 
        IUnknown *pFilterUnk;
        _filter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);

        // Show the page. 
        CAUUID caGUID;
        pProp->GetPages(&caGUID);
        pProp->Release();
        OleCreatePropertyFrame(
            0,                   // Parent window
            0, 0,                   // Reserved
            FilterInfo.achName,     // Caption for the dialog box
            1,                      // Number of objects (just the filter)
            &pFilterUnk,            // Array of object pointers. 
            caGUID.cElems,          // Number of property pages
            caGUID.pElems,          // Array of property page CLSIDs
            0,                      // Locale identifier
            0, NULL                 // Reserved
        );

        // Clean up.
        pFilterUnk->Release();
        FilterInfo.pGraph->Release(); 
        CoTaskMemFree(caGUID.pElems);
    }
}

} // DSUtils
