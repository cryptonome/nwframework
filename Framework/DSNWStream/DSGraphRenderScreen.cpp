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
#include "PchDSNWStream.h"

#include "DSGraphRenderScreen.h"
#include "DSUtils.h"
#include "NWStreamGroup.h"
#include "INWStream.h"
#include "DSFilterSourceStreamVideo.h"
#include "DSFilterSourceStreamAudio.h"
#include "DSFilterSourceStreamDSMediaSample.h"

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSGraphRenderScreen::DSGraphRenderScreen() : Inherited(),
    mhWnd(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRenderScreen::init(void* hWnd)
{
    bool bOK = true;

    if (!isOk())
    {
        mhWnd = hWnd;
        bOK = Inherited::init();
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRenderScreen::done()
{
    if (isOk())
    {

        Inherited::done();
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
/*bool DSGraphRenderScreen::createRenderGraph()
{
    bool bOK = true;

    // Create our streams
    INWStreamGroup* outputStreamGroup = mGraphInput->getStreamGroupOutput();
    for ( int i = 0 ; i < outputStreamGroup->getNumStreams() ; ++i )
    {
        INWStream* stream = outputStreamGroup->getStream(i);
        mStreamGroupInput->addStream(stream);
    }

    // Create the source filters
    std::list<IBaseFilter*> filters;
    for ( int i = 0 ; i < mStreamGroupInput->getNumStreams() ; ++i )
    {
        INWStream* stream = mStreamGroupInput->getStream(i);

        if ( stream->getType() == NWSTREAM_TYPE_MEDIA )
        {
            //switch ( stream->getSubType() )
            //{
            //    // Video
            //    case NWSTREAM_SUBTYPE_MEDIA_VIDEO:
            //        {
            //            DSFilterSourceStreamVideo* filter = DSFilterSourceStreamVideo::createInstance(stream);
            //            HRESULT hr = mGraphBuilder->AddFilter(filter,L"");

            //            filter->AddRef();
            //            filters.push_back(filter);
            //            ASSERT(hr == S_OK); hr;
            //        }
            //    break;
            //    // Audio
            //    case NWSTREAM_SUBTYPE_MEDIA_AUDIO:
            //        {
            //            DSFilterSourceStreamAudio* filter = DSFilterSourceStreamAudio::createInstance(stream);
            //            HRESULT hr = mGraphBuilder->AddFilter(filter,L"");

            //            filter->AddRef();
            //            filters.push_back(filter);
            //            ASSERT(hr == S_OK); hr;
            //        }
            //    break;
            //    default:
            //        ASSERT(false);
            //        stream->disableWrite(true);
            //        break;
            //}
            ASSERT(stream->getSubType() == NWSTREAM_SUBTYPE_MEDIA_DS_MEDIASAMPLE);

            DSFilterSourceStreamDSMediaSample* filter = DSFilterSourceStreamDSMediaSample::createInstance(stream);
            HRESULT hr = mGraphBuilder->AddFilter(filter,L"");

            filter->AddRef();
            filters.push_back(filter);
            ASSERT(hr == S_OK); hr;
        }
        else
        {
            ASSERT(false);
        }
    }

    // Render the Source filters
    for ( std::list<IBaseFilter*>::iterator it = filters.begin() ; it != filters.end() ; ++it )
    {
        IBaseFilter* filter = *it;
        IPin* pin = DSUtils::getOutputPin(filter);
        HRESULT hr = mGraphBuilder->Render(pin);
        ASSERT(hr == S_OK);
        DS_RELEASE(pin);
        DS_RELEASE(filter);
    }

    // Configure hWnd
    //if ( bOK )
    //{
    //    IVideoWindow* pVW = NULL;
    //    mGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVW);

    //    if ( pVW )
    //    {
    //        RECT rect;
    //        GetWindowRect((HWND)mhWnd,&rect);
    //        pVW->put_Owner((OAHWND)mhWnd);
    //        pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    //        pVW->put_Left(0);
    //        pVW->put_Top(0);
    //        pVW->put_Width(rect.right-rect.left);
    //        pVW->put_Height(rect.bottom-rect.top);
    //        pVW->Release();
    //    }
    //}

    if ( bOK )
        DSUtils::enableExternalDebugGraph(mGraphBuilder);


    return bOK;
}*/


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRenderScreen::createFilters()
{
    bool bOK = true;

    // We'll use the graphBuilder to create the filters when we
    // connect them on the connectSourceFilter() function

    return bOK;
}


//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRenderScreen::connectSourceFilter(IBaseFilter* _filter, IPin* _pin, INWStreamReader* _stream)
{
    bool bOK = true;

    HRESULT hr = mGraphBuilder->Render(_pin);
    bOK = hr == S_OK;
    ASSERT(bOK);

    return bOK;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSGraphRenderScreen::renderGraphCreated()
{ 
    bool bOK = true;

    // Configure hWnd
    if ( bOK && mhWnd )
    {
        IVideoWindow* pVW = NULL;
        mGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVW);

        if ( pVW )
        {
            RECT rect;
            GetWindowRect((HWND)mhWnd,&rect);
            pVW->put_Owner((OAHWND)mhWnd);
            pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
            pVW->put_Left(0);
            pVW->put_Top(0);
            pVW->put_Width(rect.right-rect.left);
            pVW->put_Height(rect.bottom-rect.top);
            pVW->Release();
        }
    }

    return bOK;
}


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSGraphRenderScreen::resized()
{
    if (mhWnd)
    {
        IVideoWindow* pVW = NULL;
        mGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVW);

        if ( pVW )
        {
            RECT rect;
            GetWindowRect((HWND)mhWnd,&rect);
            pVW->put_Left(0);
            pVW->put_Top(0);
            pVW->put_Width(rect.right-rect.left);
            pVW->put_Height(rect.bottom-rect.top);
            pVW->Release();
        }
    }
}
