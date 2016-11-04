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

#include "VibrancyHelper.h"

#import <CoreServices/CoreServices.h>


// NSVisualEffectMaterialAppearanceBased 10.10 - 0
// NSVisualEffectMaterialLight 10.10 		   - 1
// NSVisualEffectMaterialDark 10.10			   - 2
// NSVisualEffectMaterialTitlebar 10.10		   - 3
//
// NSVisualEffectMaterialSelection 10.11	   - 4
// NSVisualEffectMaterialMenu 10.11			   - 5
// NSVisualEffectMaterialPopover			   - 6
// NSVisualEffectMaterialSidebar 10.11		   - 7
// NSVisualEffectMaterialMediumLight 10.11	   - 8
// NSVisualEffectMaterialUltraDark 10.11	   - 9

// static std::map<int,std::string> materialIndexMap =
// {
// 	std::make_pair(0,"NSVisualEffectMaterialAppearanceBased"),
// 	std::make_pair(1,"NSVisualEffectMaterialLight"),
// 	std::make_pair(2,"NSVisualEffectMaterialDark"),
// 	std::make_pair(3,"NSVisualEffectMaterialTitlebar"),
// 	std::make_pair(8,"NSVisualEffectMaterialMediumLight"),
// 	std::make_pair(5,"NSVisualEffectMaterialMenu"),
// 	std::make_pair(6,"NSVisualEffectMaterialPopover"),
// 	std::make_pair(7,"NSVisualEffectMaterialSidebar"),
// 	std::make_pair(9,"NSVisualEffectMaterialUltraDark"),
// 	std::make_pair(4,"NSVisualEffectMaterialSelection")
// };


namespace Vibrancy
{
	bool IsHigherThanYosemite()
	{
		NSOperatingSystemVersion operatingSystemVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
		return operatingSystemVersion.majorVersion == 10 && operatingSystemVersion.minorVersion > 10;
	}

	VibrancyHelper::VibrancyHelper() : viewIndex_(0)
	{

	}

	int32_t VibrancyHelper::AddView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(buffer);

		if(!view)
			return -1;

		int32_t viewId = -1;

		NSRect rect = [[view window] frame];

		ViewOptions viewOptions = GetOptions(options);

		if(viewOptions.Width <= 0 || viewOptions.Width > rect.size.width)
			return viewId;

		if(viewOptions.Height <= 0 || viewOptions.Height > rect.size.height)
			return viewId;

		if(viewOptions.X < 0)
			return viewId;
		if(viewOptions.Y < 0)
			return viewId;
		

		NSVisualEffectView* vibrantView = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(viewOptions.X, viewOptions.Y, viewOptions.Width, viewOptions.Height)];
		[vibrantView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];

		if(viewOptions.ResizeMask == 0)
			[vibrantView setAutoresizingMask:NSViewWidthSizable];
		if(viewOptions.ResizeMask == 1)
			[vibrantView setAutoresizingMask:NSViewHeightSizable];
		if(viewOptions.ResizeMask == 2)
			[vibrantView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
		

		if(viewOptions.Material >= 0 && viewOptions.Material <= 9) // would crash if you give anything other than those specified here https://developer.apple.com/reference/appkit/nsvisualeffectmaterial?language=objc
		{
			if(viewOptions.Material > 3 && !IsHigherThanYosemite())
			{
		 		return -1;
		 	}
			[vibrantView setMaterial:(NSVisualEffectMaterial)viewOptions.Material];
		}

		[view.window.contentView addSubview:vibrantView positioned:NSWindowBelow relativeTo:nil];

		viewId = viewIndex_;
 		views_.insert(std::make_pair(viewId,vibrantView));
		viewIndex_++;
		return viewId;
	}
	bool VibrancyHelper::UpdateView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(buffer);

		ViewOptions viewOptions = GetOptions(options);

		if(viewOptions.ViewId == -1)
			return false;
		
		NSVisualEffectView* vibrantView = views_[viewOptions.ViewId];

		if(!vibrantView)
			return false;

		NSRect frame = [view.window frame];

		if(viewOptions.Width == 0)
			viewOptions.Width = frame.size.width;
		if(viewOptions.Height == 0)
			viewOptions.Height = frame.size.height;

		if(viewOptions.Width <= 0 && viewOptions.Width < frame.size.width)
			return false;

		if(viewOptions.Height <= 0 && viewOptions.Height < frame.size.height)
			return false;

		if(viewOptions.X < 0)
			return false;
		if(viewOptions.Y < 0)
			return false;

		[vibrantView setFrame:NSMakeRect(viewOptions.X,viewOptions.Y,viewOptions.Width, viewOptions.Height)];
		[vibrantView setMaterial:(NSVisualEffectMaterial)viewOptions.Material];

		return true;
	}
	
	bool VibrancyHelper::RemoveView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		bool result = false;
		V8Value vView = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "ViewId"));// Integer

		if(vView->IsNull() || !vView->IsInt32())
			return result;

		int viewId = vView->Int32Value();

		if(viewId == -1 || viewId > (int)views_.size())
			return result;

		std::map<int,NSVisualEffectView*>::iterator It = views_.find(viewId);

		if(It == views_.end())
			return false;

		NSVisualEffectView* vibrantView = It->second;

		if(!vibrantView)
			return result;

		views_.erase(viewId);

		NSView* viewToRemove = vibrantView;
		[viewToRemove removeFromSuperview];

		return true;
	}

	VibrancyHelper::ViewOptions VibrancyHelper::GetOptions(v8::Local<v8::Array> options)
	{		
		VibrancyHelper::ViewOptions viewOptions;
		viewOptions.ResizeMask = 2; // auto width,height resize
		viewOptions.Width = 0;
		viewOptions.Height = 0;
		viewOptions.X = 0;
		viewOptions.Y = 0;
		viewOptions.ViewId = -1; 
		viewOptions.Material = 0;

		v8::Isolate* isolate = v8::Isolate::GetCurrent();

		V8Value vPosition = options->Get(v8::String::NewFromUtf8(isolate, "Position"));
		V8Value vSize = options->Get(v8::String::NewFromUtf8(isolate,"Size"));

		V8Value vAutoResizeMask = options->Get(v8::String::NewFromUtf8(isolate, "ResizeMask")); // //Integer width = 0,height = 1,both = 2,off = 3
		V8Value vViewId = options->Get(v8::String::NewFromUtf8(isolate, "ViewId"));
		V8Value vMaterial = options->Get(v8::String::NewFromUtf8(isolate,"Material"));

		if(!vMaterial->IsNull() && vMaterial->IsInt32())
		{
			viewOptions.Material = vMaterial->Int32Value();
		}

		if(!vViewId->IsNull() && vViewId->IsInt32())
			viewOptions.ViewId = vViewId->Int32Value();
		

		if(!vSize->IsUndefined() && !vSize->IsNull())
		{
			V8Array vaSize = v8::Local<v8::Array>::Cast(vSize); // { width,height }
			// Size
			V8Value vWidth = vaSize->Get(v8::String::NewFromUtf8(isolate, "width"));// Integer
			V8Value vHeight = vaSize->Get(v8::String::NewFromUtf8(isolate,"height"));// Integer

			if(!vWidth->IsNull() && vWidth->IsInt32())
				viewOptions.Width = vWidth->Int32Value();

			if(!vHeight->IsNull() && vHeight->IsInt32())
				viewOptions.Height = vHeight->Int32Value();
		}

		if(!vPosition->IsUndefined() && !vPosition->IsNull())
		{
			V8Array vaPosition = v8::Local<v8::Array>::Cast(vPosition);// { x,y }
			// Position
			V8Value vX = vaPosition->Get(v8::String::NewFromUtf8(isolate, "x"));// Integer
			V8Value vY = vaPosition->Get(v8::String::NewFromUtf8(isolate, "y"));// Integer

			if(!vX->IsNull() && vX->IsInt32())
				viewOptions.X = vX->Int32Value();

			if(!vY->IsNull() && vY->IsInt32())
				viewOptions.Y = vY->Int32Value();
		}

		if(!vAutoResizeMask->IsNull() && vAutoResizeMask->IsInt32())
		{
			viewOptions.ResizeMask = vAutoResizeMask->Int32Value();
		}

		//NSLog(@"X:%i Y:%i W:%i H:%i Resize:%i Mat:%i ViewId: %i",viewOptions.X,viewOptions.Y,viewOptions.Width,viewOptions.Height, viewOptions.ResizeMask,viewOptions.Material,viewOptions.ViewId);
		return viewOptions;
	}

	bool VibrancyHelper::EnableVibrancy(unsigned char* windowHandleBuffer,v8::Local<v8::Array> options)
	{
		//See AddView
		return false;
	}

	bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer)
	{
		if(views_.size() > 0)
		{
			for(size_t i=0; i < views_.size();++i)
			{
				NSView* viewToRemove = views_[i];
				[viewToRemove removeFromSuperview];
			}

			views_.clear();
		}
		return true;
	}
}