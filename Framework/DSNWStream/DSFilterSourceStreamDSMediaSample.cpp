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

#include "DSFilterSourceStreamDSMediaSample.h"
#include "DSStreamSourceStreamDSMediaSample.h"
#include "DSUtils.h"


//--------------------------------------------------------------------
// GUID
//--------------------------------------------------------------------
// {5500E796-51C0-4cb2-B31C-1318FA6EC62B}
static const GUID CLSID_FilterSourceStreamDSMediaSample = 
{ 0x5500e796, 0x51c0, 0x4cb2, { 0xb3, 0x1c, 0x13, 0x18, 0xfa, 0x6e, 0xc6, 0x2b } };


//********************************************************************
//
//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
DSFilterSourceStreamDSMediaSample* DSFilterSourceStreamDSMediaSample::createInstance(INWStreamReader* _stream)
{
    DSFilterSourceStreamDSMediaSample* filter = NEW DSFilterSourceStreamDSMediaSample();

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
DSFilterSourceStreamDSMediaSample::DSFilterSourceStreamDSMediaSample() : 
    CSource(NAME("FilterSourceStreamDSMediaSample"), 0, CLSID_FilterSourceStreamDSMediaSample),
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
bool DSFilterSourceStreamDSMediaSample::init(INWStreamReader* _stream)
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
void DSFilterSourceStreamDSMediaSample::done()
{
    if (isOk())
    {
        mInit = false;
    }
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
bool DSFilterSourceStreamDSMediaSample::createStream(INWStreamReader* _stream)
{
    CAutoLock cAutoLock(&m_cStateLock);

    bool bOK = true;

    HRESULT hr = S_OK;
    ASSERT(mStream == 0);
    mStream = NEW DSStreamSourceStreamDSMediaSample(&hr, this, L"", _stream);
    bOK = hr == S_OK;

    return bOK;
}

