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


namespace Vibrancy
{
	bool VibrancyHelper::EnableVibrancy(unsigned char* windowHandleBuffer,v8::Local<v8::Array> options)
	{
		NSView* view = *reinterpret_cast<NSView**>(windowHandleBuffer);

		NSRect rect = [[view window] frame];
		NSLog(@"%@",NSStringFromRect(rect)); //To verify the bounds

		
		vibrantView = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, rect.size.width, rect.size.height)];
 		[vibrantView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
 		[vibrantView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];

 		if(options->Length() > 0)
 		{
 			// Options
	 		V8Value vMaterial = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Material"));
	 		if(!vMaterial->IsNull() && vMaterial->IsInt32())
	 		{
	 			int materialNumber = vMaterial->Int32Value();

		 		if(materialNumber >= 0 && materialNumber <= 14) // would crash if you give anything other than those specified here https://developer.apple.com/reference/appkit/nsvisualeffectmaterial?language=objc
		 		{
		 			[vibrantView setMaterial:(NSVisualEffectMaterial)materialNumber];
		 		}
	 		}

	 		//Alpha
	 		V8Value vAlpha = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Alpha"));

	 		if(!vAlpha->IsNull() && vAlpha->IsNumber())
	 		{
	 			double alphaNumber = vAlpha->NumberValue();
	 			//[vibrantView setBackgroundColor:[NSColor colorWithCalibratedWhite:1.0 alpha:0.1]];
	 			vibrantView.alphaValue = alphaNumber;
	 		}
 		}

 		[view addSubview:vibrantView positioned:NSWindowBelow relativeTo:nil];

		return true;
	}

	bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer)
	{
		NSView* view = *reinterpret_cast<NSView**>(windowHandleBuffer);

		if(views_.size())
		{
			for(int i=0; i < views_.size();++i)
			{
				[[[view subviews] objectAtIndex:i] removeFromSuperview];		
			}
		}
		//[[[view subviews] objectAtIndex:0] removeFromSuperview];
		return true;
	}

	ViewOptions VibrancyHelper::GetOptions(v8::Local<v8::Array> options)
	{
		ViewOptions viewOptions;
		viewOptions.ResizeMask = 2; // auto width,height resize
		viewOptions.Width = 0;
		viewOptions.Height = 0;
		viewOptions.X = 0;
		viewOptions.Y = 0;
		viewOptions.ViewId = -1; 
		viewOptions.Material = 0;

		if(options->Length() > 0)
		{
			V8Value vPosition = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Position"));// { x,y }
			V8Value vSize = options->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "Size")); // { width,height }
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
				V8Value vY = vPosition->Get(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "y"));// IntegerFactory

				if(vX->IsInt32())
					viewOptions.X = vX->Int32Value();

				if(vY->IsInt32())
					viewOptions.Y = vY->Int32Value();
			}

			if(!vAutoResizeMask->IsNull() && vAutoResizeMask->IsInt32())
			{
				resizeMask = vAutoResizeMask->Int32Value();
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

		ViewOptions viewOptions = GetViewOptions(options);
		

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

		ViewOptions viewOptions = GetViewOptions(options);

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