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
#include "./Vibrancy.h"

//----------------------------------------------------------------------------
namespace Vibrancy {
    static VibrancyHelper vibHelper_;

    Vibrancy::Vibrancy() {
    }

    Vibrancy::~Vibrancy() {
    }
    void Vibrancy::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
        v8::Local<v8::FunctionTemplate> tpl =
            Nan::New<v8::FunctionTemplate>(SetVibrancy);

        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::FunctionTemplate> tpl1 =
            Nan::New<v8::FunctionTemplate>(AddView);
        tpl1->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::FunctionTemplate> tpl2 =
            Nan::New<v8::FunctionTemplate>(UpdateView);
        tpl2->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::FunctionTemplate> tpl3 =
            Nan::New<v8::FunctionTemplate>(RemoveView);
        tpl3->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::Set(target,
            Nan::New("SetVibrancy").ToLocalChecked(),
            Nan::GetFunction(tpl).ToLocalChecked());

        Nan::Set(target,
            Nan::New("AddView").ToLocalChecked(),
            Nan::GetFunction(tpl1).ToLocalChecked());

        Nan::Set(target,
            Nan::New("UpdateView").ToLocalChecked(),
            Nan::GetFunction(tpl2).ToLocalChecked());

        Nan::Set(target,
            Nan::New("RemoveView").ToLocalChecked(),
            Nan::GetFunction(tpl3).ToLocalChecked());
    }

    NAN_METHOD(Vibrancy::SetVibrancy) {
        v8::Local<v8::Object> toggleStateObj =
            info[0].As<v8::Object>();
        v8::Local<v8::Object> handleBuffer =
            info[1].As<v8::Object>();

        v8::Isolate* isolate = info.GetIsolate();
        v8::HandleScope scope(isolate);

        if (toggleStateObj->IsNull())
            return;

        if (handleBuffer->IsNull())
            return;

        bool toggleState = toggleStateObj->BooleanValue(isolate);

        char* bufferData = node::Buffer::Data(handleBuffer);

        bool result = false;

        if (!toggleState)
            result = vibHelper_.DisableVibrancy((unsigned char*)bufferData);

        info.GetReturnValue().Set(result);
    }

    NAN_METHOD(Vibrancy::AddView) {
        v8::Local<v8::Object> handleBuffer = info[0].As<v8::Object>();
        v8::Local<v8::Array> options = info[1].As<v8::Array>();

        v8::Isolate* isolate = info.GetIsolate();
        v8::HandleScope scope(isolate);

        char* bufferData = node::Buffer::Data(handleBuffer);

        int32_t result = -1;

        result = vibHelper_.AddView((unsigned char*)bufferData, options);

        info.GetReturnValue().Set(result);
    }

    NAN_METHOD(Vibrancy::UpdateView) {
        v8::Local<v8::Object> handleBuffer =
            info[0].As<v8::Object>();
        v8::Local<v8::Array> options =
            info[1].As<v8::Array>();

        v8::Isolate* isolate = info.GetIsolate();
        v8::HandleScope scope(isolate);

        char* bufferData = node::Buffer::Data(handleBuffer);

        bool result = false;

        result = vibHelper_.UpdateView((unsigned char*)bufferData, options);

        info.GetReturnValue().Set(result);
    }

    NAN_METHOD(Vibrancy::RemoveView) {
        v8::Local<v8::Object> handleBuffer =
            info[0].As<v8::Object>();
        v8::Local<v8::Array> options =
            info[1].As<v8::Array>();

        v8::Isolate* isolate = info.GetIsolate();
        v8::HandleScope scope(isolate);

        char* bufferData = node::Buffer::Data(handleBuffer);

        bool result = false;

        result = vibHelper_.RemoveView((unsigned char*)bufferData, options);

        info.GetReturnValue().Set(result);
    }
}   //  namespace Vibrancy
