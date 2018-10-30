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


#include "./VibrancyHelper.h"


#pragma comment(lib, "dwmapi.lib")

namespace Vibrancy {
    struct ACCENTPOLICY {
        int nAccentState;
        int nFlags;
        int nColor;
        int nAnimationId;
    };
    struct WINCOMPATTRDATA {
        int nAttribute;
        PVOID pData;
        ULONG ulDataSize;
    };

    enum AccentTypes {
        ACCENT_DISABLE = 0,
        ACCENT_ENABLE_GRADIENT = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND = 3
    };

    bool IsWindows10() {
        OSVERSIONINFOA info;
        ZeroMemory(&info, sizeof(OSVERSIONINFOA));
        info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

        GetVersionEx(&info);

        return info.dwMajorVersion == 10;
    }

    bool SetBlurBehind(HWND hwnd, bool state) {
        bool result = false;

        if (IsWindows10()) {
            const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
            if (hModule) {
                typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND,
                    WINCOMPATTRDATA*);
                const pSetWindowCompositionAttribute
                    SetWindowCompositionAttribute =
                    (pSetWindowCompositionAttribute)GetProcAddress(
                        hModule,
                        "SetWindowCompositionAttribute");

                // Only works on Win10
                if (SetWindowCompositionAttribute) {
                    ACCENTPOLICY policy =
                        { state ? ACCENT_ENABLE_BLURBEHIND
                            : ACCENT_DISABLE , 0, 0, 0 };
                    WINCOMPATTRDATA data = {19, &policy, sizeof(ACCENTPOLICY) };
                    result = SetWindowCompositionAttribute(hwnd, &data);
                }
                FreeLibrary(hModule);
            }
        } else {
            HRESULT hr = S_OK;

            RECT rect;  
            GetWindowRect(hwnd, &rect);
            HRGN hRgn = CreateRectRgn(5, 5, rect.right - rect.left - 5, rect.bottom - rect.top - 5);

            // Create and populate the Blur Behind structure
            DWM_BLURBEHIND bb = { 0 };

            // Enable Blur Behind and apply to the entire client area
            bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
            bb.fEnable = state;
            bb.hRgnBlur = hRgn;

            // Apply Blur Behind
            hr = DwmEnableBlurBehindWindow(hwnd, &bb);
            if (SUCCEEDED(hr)) {
                result = true;
            }
        }
        return result;
    }
    bool SetBlurBehinds(HWND hwnd, bool state, v8::Local<v8::Array> options) {
        bool result = false;

        if (IsWindows10()) {
            const HINSTANCE hModule = LoadLibrary(TEXT("user32.dll"));
            if (hModule) {
                typedef BOOL(WINAPI*pSetWindowCompositionAttribute)(HWND,
                    WINCOMPATTRDATA*);
                const pSetWindowCompositionAttribute
                    SetWindowCompositionAttribute =
                    (pSetWindowCompositionAttribute)GetProcAddress(
                        hModule,
                        "SetWindowCompositionAttribute");

                // Only works on Win10
                if (SetWindowCompositionAttribute) {
                    ACCENTPOLICY policy =
                        { state ? ACCENT_ENABLE_BLURBEHIND
                            : ACCENT_DISABLE , 0, 0, 0 };
                    WINCOMPATTRDATA data = {19, &policy, sizeof(ACCENTPOLICY) };
                    result = SetWindowCompositionAttribute(hwnd, &data);
                }
                FreeLibrary(hModule);
            }
        } else {
            HRESULT hr = S_OK;

            struct Option
            {
                int vX;
                int vY;
                int vWidth;
                int vHeight;
                int borderRadius;
            } opt;

            opt.vX = 0;
            opt.vY = 0;
            opt.vWidth = 0;
            opt.vHeight = 0;
            opt.borderRadius = 0;



            v8::Isolate* isolate = v8::Isolate::GetCurrent();

            V8Value vPosition = options->Get(
                v8::String::NewFromUtf8(isolate, "Position"));
            V8Value vSize = options->Get(v8::String::NewFromUtf8(isolate, "Size"));

            V8Value vAutoResizeMask = options->Get(
                v8::String::NewFromUtf8(isolate, "ResizeMask"));
            V8Value vViewId = options->Get(
                v8::String::NewFromUtf8(isolate, "ViewId"));
            V8Value vMaterial = options->Get(
                v8::String::NewFromUtf8(isolate, "Material"));

            V8Value borderRadius = options->Get(
                v8::String::NewFromUtf8(isolate, "BorderRadius"));

            if (!borderRadius->IsNull() && borderRadius->IsInt32()) {
                opt.borderRadius = borderRadius->Int32Value();
            }
            

            if (!vSize->IsUndefined() && !vSize->IsNull()) {
                V8Array vaSize =
                    v8::Local<v8::Array>::Cast(vSize);

                V8Value vWidth =
                    vaSize->Get(v8::String::NewFromUtf8(isolate, "width"));
                V8Value vHeight =
                    vaSize->Get(v8::String::NewFromUtf8(isolate, "height"));

                if (!vWidth->IsNull() && vWidth->IsInt32())
                    opt.vWidth = vWidth->Int32Value();

                if (!vHeight->IsNull() && vHeight->IsInt32())
                    opt.vHeight = vHeight->Int32Value();
            }

            if (!vPosition->IsUndefined() && !vPosition->IsNull()) {
                V8Array vaPosition = v8::Local<v8::Array>::Cast(vPosition);

                V8Value vX = vaPosition->Get(v8::String::NewFromUtf8(isolate, "x"));
                V8Value vY = vaPosition->Get(v8::String::NewFromUtf8(isolate, "y"));

                if (!vX->IsNull() && vX->IsInt32())
                    opt.vX = vX->Int32Value();

                if (!vY->IsNull() && vY->IsInt32())
                    opt.vY = vY->Int32Value();
            }

            // RECT rect;  
            // GetWindowRect(hwnd, &rect);
            HRGN hRgn = CreateRoundRectRgn(opt.vX, opt.vY, opt.vWidth - opt.vX, opt.vHeight - opt.vY, opt.borderRadius, opt.borderRadius);

            // Create and populate the Blur Behind structure
            DWM_BLURBEHIND bb = { 0 };

            // Enable Blur Behind and apply to the entire client area
            bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
            bb.fEnable = state;
            bb.hRgnBlur = hRgn;

            // Apply Blur Behind
            hr = DwmEnableBlurBehindWindow(hwnd, &bb);
            if (SUCCEEDED(hr)) {
                result = true;
            }
        }
        return result;
    }

    VibrancyHelper::VibrancyHelper() {
    }

    bool VibrancyHelper::DisableVibrancy(unsigned char* windowHandleBuffer) {
        uint32_t handle =
            *reinterpret_cast<uint32_t*>(windowHandleBuffer);
        HWND hwnd = (HWND)handle;
        return SetBlurBehind(hwnd, false);
    }

    int32_t VibrancyHelper::AddView(unsigned char* buffer,
        v8::Local<v8::Array> options) {
        uint32_t handle =
            *reinterpret_cast<uint32_t*>(buffer);
        HWND hwnd = (HWND)handle;
        return SetBlurBehinds(hwnd, true, options);
    }

    // These are here so compiling doesnt fail.
    bool VibrancyHelper::UpdateView(unsigned char* buffer,
        v8::Local<v8::Array> options) {
        return false;
    }

    bool VibrancyHelper::RemoveView(unsigned char* buffer,
        v8::Local<v8::Array> options) {
        return false;
    }
}  // namespace Vibrancy
