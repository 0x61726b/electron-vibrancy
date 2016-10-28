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
#include <map>
#include <utility>

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

static std::map<int,std::string> materialIndexMap =
{
	std::make_pair(0,"NSVisualEffectMaterialAppearanceBased"),
	std::make_pair(1,"NSVisualEffectMaterialLight"),
	std::make_pair(2,"NSVisualEffectMaterialDark"),
	std::make_pair(3,"NSVisualEffectMaterialTitlebar"),
	std::make_pair(8,"NSVisualEffectMaterialMediumLight"),
	std::make_pair(5,"NSVisualEffectMaterialMenu"),
	std::make_pair(6,"NSVisualEffectMaterialPopover"),
	std::make_pair(7,"NSVisualEffectMaterialSidebar"),
	std::make_pair(9,"NSVisualEffectMaterialUltraDark"),
	std::make_pair(4,"NSVisualEffectMaterialSelection")
};


namespace Vibrancy
{
	bool IsHigherThanYosemite()
	{
		NSOperatingSystemVersion operatingSystemVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
		return operatingSystemVersion.majorVersion == 10 && operatingSystemVersion.minorVersion > 10;
	}
	bool VibrancyHelper::EnableVibrancy(unsigned char* windowHandleBuffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(windowHandleBuffer);

		NSRect rect = [[view window] frame];
		NSLog(@"%@",NSStringFromRect(rect)); //To verify the bounds

		
		fullSizeVibrantView_ = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, rect.size.width, rect.size.height)];
 		[fullSizeVibrantView_ setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
 		[fullSizeVibrantView_ setBlendingMode:NSVisualEffectBlendingModeBehindWindow];

 		if(options->Length() > 0)
 		{
 			// Options
	 		V8Value vMaterial = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Material"));
	 		if(!vMaterial->IsNull() && vMaterial->IsInt32())
	 		{
	 			int materialNumber = vMaterial->Int32Value();

		 		if(materialNumber >= 0 && materialNumber <= 9) // would crash if you give anything other than those specified here https://developer.apple.com/reference/appkit/nsvisualeffectmaterial?language=objc
		 		{
		 			if(materialNumber > 3 && !IsHigherThanYosemite())
		 			{
		 				return false;
		 			}
		 			[fullSizeVibrantView_ setMaterial:(NSVisualEffectMaterial)materialNumber];
		 		}
	 		}

	 		//Alpha
	 		V8Value vAlpha = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Alpha"));

	 		if(!vAlpha->IsNull() && vAlpha->IsNumber())
	 		{
	 			double alphaNumber = vAlpha->NumberValue();
	 			//[vibrantView setBackgroundColor:[NSColor colorWithCalibratedWhite:1.0 alpha:0.1]];
	 			fullSizeVibrantView_.alphaValue = alphaNumber;
	 		}
 		}

 		//[view addSubview:fullSizeVibrantView_ positioned:NSWindowBelow relativeTo:nil];
 		[view.window.contentView addSubview:fullSizeVibrantView_ positioned:NSWindowBelow relativeTo:nil];

 		views_.push_back(fullSizeVibrantView_);

		return true;
	}

	bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer)
	{
		NSView* view = *reinterpret_cast<NSView**>(windowHandleBuffer);

		NSLog(@"Disabling Vibrancy - View count %i",views_.size());

		if(views_.size() > 0)
		{
			NSLog(@"Disabling Vibrancy - View count %i",views_.size());

			NSView* contentView = (NSView*)view.window.contentView;
			for(int i=0; i < views_.size();++i)
			{
				NSView* viewToRemove = [[contentView subviews] objectAtIndex:i];
				NSLog(@"Removing subview %i %@",i,viewToRemove);
				[viewToRemove removeFromSuperview];
			}
		}
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

		if(options->Length() > 0)
		{
			V8Array vPosition = v8::Local<v8::Array>::Cast(options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Position")));// { x,y }
			V8Array vSize = v8::Local<v8::Array>::Cast(options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Size"))); // { width,height }
			V8Value vAutoResizeMask = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "ResizeMask")); // //Integer width = 0,height = 1,both = 2,off = 3
			V8Value vViewId = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "ViewId"));
			V8Value vMaterial = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Material"));

			if(!vMaterial->IsNull() && vMaterial->IsInt32())
				viewOptions.Material = vMaterial->Int32Value();

			if(!vViewId->IsNull() && vViewId->IsInt32())
				viewOptions.ViewId = vViewId->Int32Value();
			
			if(!vSize->IsNull())
			{
				// Size
				V8Value vWidth = vSize->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "width"));// Integer
				V8Value vHeight = vSize->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "height"));// Integer

				if(vWidth->IsInt32())
					viewOptions.Width = vWidth->Int32Value();

				if(vHeight->IsInt32())
					viewOptions.Height = vHeight->Int32Value();
			}

			if(!vPosition->IsNull())
			{
				// Position
				V8Value vX = vPosition->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "x"));// Integer
				V8Value vY = vPosition->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "y"));// Integer

				if(vX->IsInt32())
					viewOptions.X = vX->Int32Value();

				if(vY->IsInt32())
					viewOptions.Y = vY->Int32Value();
			}

			if(!vAutoResizeMask->IsNull() && vAutoResizeMask->IsInt32())
			{
				viewOptions.ResizeMask = vAutoResizeMask->Int32Value();
			}
		}
		return viewOptions;
	}

	int32_t VibrancyHelper::AddView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(buffer);

		if(!view)
			return -1;

		int32_t viewId = -1;

		NSRect rect = [[view window] frame];

		ViewOptions viewOptions = GetOptions(options);
		

		NSVisualEffectView* vibrantView = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(viewOptions.X, viewOptions.Y, viewOptions.Width, viewOptions.Height)];
		[vibrantView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];

		if(viewOptions.ResizeMask == 0)
			[vibrantView setAutoresizingMask:NSViewWidthSizable];
		if(viewOptions.ResizeMask == 1)
			[vibrantView setAutoresizingMask:NSViewHeightSizable];
		if(viewOptions.ResizeMask == 2)
			[vibrantView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];

		[vibrantView setMaterial:(NSVisualEffectMaterial)viewOptions.Material];
		[view addSubview:vibrantView positioned:NSWindowBelow relativeTo:nil];

		viewId = views_.size();
		views_.push_back(vibrantView);
		return viewId;
	}
	bool VibrancyHelper::UpdateView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(buffer);

		ViewOptions viewOptions = GetOptions(options);

		if(viewOptions.ViewId == -1)
			return false;
		
		NSVisualEffectView* vibrantView = views_[viewOptions.ViewId];

		// TODO 
		// Retrieve views current size,position and make them default instead so null parameters dont affect

		if(!vibrantView)
			return false;

		[vibrantView setFrame:NSMakeRect(viewOptions.X, viewOptions.Y, viewOptions.Width, viewOptions.Height)];
		[vibrantView setMaterial:(NSVisualEffectMaterial)viewOptions.Material];

		return true;
	}
	
	bool VibrancyHelper::RemoveView(unsigned char* buffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(buffer);

		bool result = false;
		if(options->Length() > 0)
		{
			V8Value vView = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "ViewId"));// Integer

			if(vView->IsNull() || !vView->IsInt32())
				return result;

			int viewId = vView->Int32Value();

			if(viewId == -1 || viewId > views_.size())
				return result;

			NSVisualEffectView* vibrantView = views_[viewId];

			views_.erase(views_.begin() + viewId);

			[[[view subviews] objectAtIndex:viewId] removeFromSuperview];
		}
		else
			return result;
	}
}