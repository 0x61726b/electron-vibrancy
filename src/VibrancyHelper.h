//----------------------------------------------------------------------------
// electron-vibrancy
// Copyright 2016 arkenthera
//
// MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//----------------------------------------------------------------------------
#ifndef SRC_VIBRANCYHELPER_H_
#define SRC_VIBRANCYHELPER_H_
//----------------------------------------------------------------------------
#include <map>

#include "./Common.h"
//----------------------------------------------------------------------------
namespace Vibrancy {
    class VibrancyHelper {
     public:
        VibrancyHelper();
        ~VibrancyHelper() { }
        bool DisableVibrancy(unsigned char* buffer);
        #ifdef PLATFORM_OSX
            struct ViewOptions {
                int ViewId;
                int ResizeMask;
                int Width;
                int Height;
                int X;
                int Y;
                int Material;
            };
            ViewOptions GetOptions(v8::Local<v8::Array> options);
            std::map<int, NSVisualEffectView* > views_;
            int viewIndex_;
        #endif
        int32_t AddView(unsigned char* buffer, v8::Local<v8::Array> options);
        bool UpdateView(unsigned char* buffer, v8::Local<v8::Array> options);
        bool RemoveView(unsigned char* buffer, v8::Local<v8::Array> options);
    };
}  // namespace Vibrancy
//----------------------------------------------------------------------------
#endif  // SRC_VIBRANCYHELPER_H_
