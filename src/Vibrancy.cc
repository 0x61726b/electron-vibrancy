//----------------------------------------------------------------------------
// electron-vibrancy
//
// MIT License
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//----------------------------------------------------------------------------
#include "Vibrancy.h"
//----------------------------------------------------------------------------
namespace Vibrancy
{
	Vibrancy::Vibrancy()
	{
	}

	Vibrancy::~Vibrancy()
	{
	}

	void Vibrancy::Init(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target)
	{
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(SetVibrancy);
		v8::Local<v8::ObjectTemplate> inst = tpl->InstanceTemplate();
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		Nan::Set(target,Nan::New("SetVibrancy").ToLocalChecked(),Nan::GetFunction(tpl).ToLocalChecked());
	}

	NAN_METHOD(Vibrancy::SetVibrancy)
	{
		v8::Local<v8::Object> toggleStateObj = info[0].As<v8::Object>();
		v8::Local<v8::Object> handleBuffer = info[1].As<v8::Object>();
		v8::Local<v8::Array> options = info[2].As<v8::Array>();

		v8::Isolate* isolate = info.GetIsolate();
		v8::HandleScope scope(isolate);

		bool toggleState = toggleStateObj->BooleanValue();
		
	
		char* bufferData = node::Buffer::Data(handleBuffer);

		// Options


		VibrancyHelper helper;
		if(toggleState)
			helper.EnableVibrancy((unsigned char*)bufferData,options);
		else
			helper.DisableVibrancy((unsigned char*)bufferData);

	}
}
