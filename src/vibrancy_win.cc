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
	struct ACCENTPOLICY
	{
		int nAccentState;
		int nFlags;
		int nColor;
		int nAnimationId;
	};
	struct WINCOMPATTRDATA
	{
		int nAttribute;
		PVOID pData;
		ULONG ulDataSize;
	};

	enum AccentTypes
	{
		ACCENT_DISABLE = 0,
		ACCENT_ENABLE_GRADIENT = 1,
		ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
		ACCENT_ENABLE_BLURBEHIND = 3
	};

	bool SetBlurBehind(HWND hwnd,bool state)
	{
		bool result = false;
		const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
		if(hModule)
		{
			typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND,WINCOMPATTRDATA*);
			const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hModule,"SetWindowCompositionAttribute");

			// Only works on Win10
			if(SetWindowCompositionAttribute)
			{
				ACCENTPOLICY policy ={state ? ACCENT_ENABLE_BLURBEHIND : ACCENT_DISABLE ,0,0,0};
				WINCOMPATTRDATA data ={19,&policy,sizeof(ACCENTPOLICY)};
				result = SetWindowCompositionAttribute(hwnd,&data);
			}
			else
			{

			}
			FreeLibrary(hModule);
		}
		return result;
	}

	bool VibrancyHelper::EnableVibrancy(unsigned char* windowHandleBuffer)
	{
		unsigned long handle = *reinterpret_cast<unsigned long*>(windowHandleBuffer);
		HWND hwnd = (HWND)handle;

		HWND firstChild = GetWindow(hwnd,GW_CHILD);
		return SetBlurBehind(hwnd,true);
	}

	bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer)
	{
		unsigned long handle = *reinterpret_cast<unsigned long*>(windowHandleBuffer);
		HWND hwnd = (HWND)handle;

		HWND firstChild = GetWindow(hwnd,GW_CHILD);
		return SetBlurBehind(hwnd,false);
	}
}