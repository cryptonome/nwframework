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

#include "DSFilterSourceStreamVideo.h"
#include "DSStreamSourceStreamVideo.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {00744768-DED4-4fc3-B976-533FEE3308AF}
static const GUID CLSID_FilterSourceStreamVideo = 
{ 0x744768, 0xded4, 0x4fc3, { 0xb9, 0x76, 0x53, 0x3f, 0xee, 0x33, 0x8, 0xaf } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamVideo* DSFilterSourceStreamVideo::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamVideo* filter = NEW DSFilterSourceStreamVideo();

    if ( !filter->init(_stream) )
        DS_RELEASE(filter);

    return filter;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamVideo::DSFilterSourceStreamVideo() : 
    CSource(NAME("FilterSourceStreamVideo"), 0, CLSID_FilterSourceStreamVideo),
    mInit(false),
    mStream(0)
{
    int a = 0;
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamVideo::init(INWStreamReader* _stream)
{
    bool bOK = true;

    if (!isOk())
    {
        bOK = createStream(_stream);

        mInit = true;
    }
    return bOK;

}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void DSFilterSourceStreamVideo::done()
{
    if (isOk())
    {
        //DS_RELEASE(mStream);
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamVideo::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamVideo(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}
