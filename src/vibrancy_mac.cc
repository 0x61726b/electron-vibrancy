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

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <objc/objc-runtime.h>

namespace Vibrancy
{
	bool VibrancyHelper::EnableVibrancy(unsigned char* windowHandleBuffer)
	{
		NSView* view = *reinterpret_cast<NSView**>(windowHandleBuffer);

		NSRect rect = [[view window] frame];
		NSLog(@"%@",NSStringFromRect(rect)); //To verify the bounds

		
		NSVisualEffectView *vibrantView = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, rect.size.width, rect.size.height)];
 		[vibrantView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];

 		[vibrantView setBlendingMode:NSVisualEffectBlendingModeBehindWindow];

 		[view addSubview:vibrantView positioned:NSWindowBelow relativeTo:nil];


		return false;
	}

	bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer)
	{
		return false;
	}
}