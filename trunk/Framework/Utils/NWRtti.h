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
*
*      Permission is hereby granted, free of charge, to any person obtaining
*      a copy of this software and associated documentation files (the
*      "Software"), to deal in the Software without restriction, including
*      without limitation the rights to use, copy, modify, merge, publish,
*      distribute, sublicense, and/or sell copies of the Software, and to
*      permit persons to whom the Software is furnished to do so, subject to
*      the following conditions:
*
*      The above copyright notice and this permission notice shall be
*      included in all copies or substantial portions of the Software.
*
*      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
*      LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
*      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
*      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/




#ifndef _NW_RTTI_H_
#define _NW_RTTI_H_

typedef intptr_t NWRttiTypeId;

//----------------------------------------------------------------------------
// The RTTI itself
//----------------------------------------------------------------------------
template <class T> class NWRtti
{
public:
    static inline NWRttiTypeId getTypeId();

private:
    static int mDummyVar;
};

template <class T> /*static*/ int NWRtti<T>::mDummyVar = 0;

template <class T> /*static*/ inline NWRttiTypeId NWRtti<T>::getTypeId()
{
    ++mDummyVar;
    return (NWRttiTypeId)&getTypeId;
}

#define RTTI_TYPE_ID(T) static inline NWRttiTypeId getTypeId() {return NWRtti<T>::getTypeId();} // use this macro inside classes yo want to have TypeId

//----------------------------------------------------------------------------
// Helper class to inherit from by classes exporting interfaces
//----------------------------------------------------------------------------
class NWInterfaceProvider
{
public:
    template <class T> inline T * getInterface();

protected:
    virtual void * getInterfaceFromID(NWRttiTypeId _typeId)=0;      // derived class must implement this function
};

template <class T> inline T * NWInterfaceProvider::getInterface()
{
    return (T *) getInterfaceFromID(NWRtti<T>::getTypeId());
}

#endif //_NW_RTTI_H_

//****************************************************************************
// Example
//****************************************************************************

//----------------------------------------------------------------------------
// Some interfaces we want share
//----------------------------------------------------------------------------
//struct IEncoder
//{
//    virtual unsigned int getEncodingCaps()=0;
//    ...
//
//    RTTI_TYPE_ID(IEncoder)
//};
//
//struct IVideoEncoder
//{
//    virtual unsigned int getVideoEncoderCaps()=0;
//    ...
//
//    RTTI_TYPE_ID(IVideoEncoder)
//}
//
//struct IVideoEncoderH264
//{
//    virtual int getVideoCodecH264KeyFramePeriod()=0;
//    ...
//
//    RTTI_TYPE_ID(IVideoEncoderH264)
//}

//----------------------------------------------------------------------------
// The exporter (or it's base class if any) inherits from NWInterfaceProvider
//----------------------------------------------------------------------------
//class VideoEncoderH264 : public NWInterfaceProvider
//{
//public:
//    ...
//
//protected:
//    virtual void * getInterfaceFromID(NWRttiTypeId _iid);     // <--- Implements this virtual fn to provide the valid interfaces
//
//private:
//    ...
//};

//----------------------------------------------------------------------------
// Exporter class fn (this class inherits from the interface):
//----------------------------------------------------------------------------
///*virtual*/ void * VideoEncoderH264::getInterfaceFromID(NWRttiTypeId _iid)
//{
//    if(_iid == IVideoEncoderH264::getTypeId())
//    {
//        return static_cast<IVideoEncoderH264*>(this);
//    }
//    else if(_iid == IVideoEncoder::getTypeId())
//    {
//        return static_cast<IVideoEncoder*>(this);
//    }
//    else if(_iid == IEncoder::getTypeId())
//    {
//        return static_cast<IEncoder*>(this);
//    }
//
//    return NULL; // if no base class just return NULL
//    //return Inherited::getInterfaceFromID(_iid); // in case that the base class has some other exported interfaces
//}

//----------------------------------------------------------------------------
// External use
//----------------------------------------------------------------------------
//void test()
//{
//    VideoEncoderH264 encoderObj;
//
//    encoderObj.init();
//
//    IEncoder * encoder = encoderObj.getInterface<IEncoder>();
//    if(encoder)
//    {
//        unsigned int caps = encoder->getEncodingCaps();
//    }
//
//    IVideoEncoder * videoEncoder = encoderObj.getInterface<IVideoEncoder>();
//    if(videoEncoder)
//    {
//        unsigned int caps = videoEncoder->getVideoEncoderCaps();
//    }
//
//    IVideoEncoderH264 * videoEncoderH264 = encoderObj.getInterface<IVideoEncoderH264>();
//    if(videoEncoderH264)
//    {
//        int h264KeyFramePeriod = videoEncoderH264->getVideoCodecH264KeyFramePeriod();
//    }
//
//    encoderObj.shutdown();
//}
