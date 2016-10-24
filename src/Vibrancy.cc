//----------------------------------------------------------------------------
//Chiika
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
#include "Vibrancy.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
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
		v8::Isolate* isolate = info.GetIsolate();
		v8::HandleScope scope(isolate);

		bool toggleState = toggleStateObj->BooleanValue();
		
	
		char* bufferData = node::Buffer::Data(handleBuffer);
		VibrancyHelper helper;
		if(toggleState)
			helper.EnableVibrancy((unsigned char*)bufferData);
		else
			helper.DisableVibrancy((unsigned char*)bufferData);

	}
}
