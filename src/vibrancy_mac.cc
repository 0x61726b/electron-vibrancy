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