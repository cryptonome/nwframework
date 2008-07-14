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

#include "DSFilterSourceStreamAudio.h"
#include "DSStreamSourceStreamAudio.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {74AB5943-0ED6-4ba2-BAD7-EA8BDCE0BE31}
static const GUID CLSID_FilterSourceStreamAudio = 
{ 0x74ab5943, 0xed6, 0x4ba2, { 0xba, 0xd7, 0xea, 0x8b, 0xdc, 0xe0, 0xbe, 0x31 } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamAudio* DSFilterSourceStreamAudio::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamAudio* filter = NEW DSFilterSourceStreamAudio();

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
DSFilterSourceStreamAudio::DSFilterSourceStreamAudio() : 
    CSource(NAME("FilterSourceStreamAudio"), 0, CLSID_FilterSourceStreamAudio),
    mInit(false),
    mStream(0)
{
}

//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamAudio::init(INWStreamReader* _stream)
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
void DSFilterSourceStreamAudio::done()
{
    if (isOk())
    {
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamAudio::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamAudio(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}

