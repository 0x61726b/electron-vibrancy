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

		[[[view subviews] objectAtIndex:0] removeFromSuperview];
		return true;
	}
}