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
#ifndef SRC_VIBRANCY_H_
#define SRC_VIBRANCY_H_
//----------------------------------------------------------------------------
#include "./Common.h"
#include "./VibrancyHelper.h"
//----------------------------------------------------------------------------
namespace Vibrancy {
    class Vibrancy : public Nan::ObjectWrap {
     public:
        static void Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);

        explicit Vibrancy();
        ~Vibrancy();

        static NAN_METHOD(SetVibrancy);

        static NAN_METHOD(AddView);
        static NAN_METHOD(RemoveView);
        static NAN_METHOD(UpdateView);
        VibrancyHelper helper_;
    };
}  // namespace Vibrancy
//----------------------------------------------------------------------------
#endif  // SRC_VIBRANCY_H_
