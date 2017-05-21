#electron-vibrancy

[![Build Status](https://travis-ci.org/arkenthera/electron-vibrancy.svg?branch=master)](https://travis-ci.org/arkenthera/electron-vibrancy)
[![Build status](https://ci.appveyor.com/api/projects/status/41na3l88kgs8wir1?svg=true)](https://ci.appveyor.com/project/arkenthera/electron-vibrancy)
[![npm version](https://badge.fury.io/js/electron-vibrancy.svg)](https://badge.fury.io/js/electron-vibrancy)
![](https://img.shields.io/npm/dm/electron-vibrancy.svg)
![](https://david-dm.org/arkenthera/electron-vibrancy/status.svg)

# As of [#7898](https://github.com/electron/electron/pull/7898) Vibrancy is now supported in Electron for *macOS*.

This module is intended to give an [Electron](https://github.com/electron/electron) BrowserWindow blur on its behind. Electron does not support 'blur behind' from a transparent window and this module uses native API calls to achieve the effect.

![](http://i.imgur.com/0sRPzpn.png)

![](http://i.imgur.com/42jOnRV.png)

# Running

Since this is a native addon, you will need your platforms build tools. Visual Studio,XCode etc.Also Python for `node-gyp`.

```
git clone https://github.com/arkenthera/electron-vibrancy
cd electron-vibrancy
npm install
cd spec/app # Go to sample app
electron . # electron --version should be 1.6.8
```

To rebuild again:

```
npm run conf
npm run rebuild
```

Also available through NPM.

```
npm install electron-vibrancy
```

To run tests see [scripts/ci](https://github.com/arkenthera/electron-vibrancy/blob/master/scripts/ci.sh)

# Current Supported Platforms
- macOS 10.10+
- Windows 10 (stable) It just works ™
- Windows 8.x (unstable + requires modifications)
- Windows 7 (unstable + requires modifications)

# Things to note
- `BrowserWindow` must be transparent. (`transparent:true`)
- Requires Yosemite on macOS.
- On Windows 8.x, this wont work because Microsoft completely removed Aero Glass feature. It is still possible though but it is not something that an end user should do.
See Platforms section for more info.
- On Windows 7, an Aero theme must be activated.
- If you get `A dynamic link library (DLL) initialization routine failed.` error, it means that the module isn't compiled against Electron or compiled against the wrong version. 

Although it works, I dont recommend using this module on a machine below Windows 10. See platforms section below for more information for macOS.

# API
There are several methods depending on what you want to do and what platform you are on.

### `SetVibrancy(window, material)` _win_ , _macOS_

Returns `Integer`.View id of `NSVisualEffectView`. You need this for `UpdateView` or `RemoveView`. `material` has no effect on Windows.

* `window` `BrowserWindow` instance
* `Material` - Integer. The Material for `NSVisualEffectMaterial`.
  * `0` - `NSVisualEffectMaterialAppearanceBased` *10.10+*
  * `1` - `NSVisualEffectMaterialLight` *10.10+*
  * `2` - `NSVisualEffectMaterialDark` *10.10+*
  * `3` - `NSVisualEffectMaterialTitlebar` *10.10+*
  * `4` - `NSVisualEffectMaterialSelection` *10.11+*
  * `5` - `NSVisualEffectMaterialMenu` *10.11+*
  * `6` - `NSVisualEffectMaterialPopover` *10.11+*
  * `7` - `NSVisualEffectMaterialSidebar` *10.11+*
  * `8` - `NSVisualEffectMaterialMediumLight` *10.11+*
  * `9` - `NSVisualEffectMaterialUltraDark` *10.11+*

Enables or disables vibrancy for the **WHOLE** window. It will resize automatically. If you want something custom, see `AddView`.
See [here](https://developer.apple.com/reference/appkit/nsvisualeffectmaterial?language=objc) for more info about `NSVisualEffectMaterial`.


### `DisableVibrancy(window)` _win_, _macOS_

Disables Vibrancy completely.

* `window` `BrowserWindow` instance


### `AddView(window,options)` _macOS_

Returns `Integer`.View id of `NSVisualEffectView`. You need this for `UpdateView` or `RemoveView`.

* `window` `BrowserWindow` instance
* `options` Object
  * `Material` - Integer. The Material for `NSVisualEffectMaterial`.See `SetVibrancy` method for material properties.
  * `X` X Position of the `NSVisualEffectView` relative to the main `BrowserWindow`.
  * `Y` X Position of the `NSVisualEffectView` relative to the main `BrowserWindow`.
  * `Width` - Integer Width of the `NSVisualEffectView`. Should not be larger than the window's.
  * `Height` - Integer Height of the `NSVisualEffectView`. Should not be larger than the window's.
  * `ResizeMask`- Integer.Resize mask for the `NSVisualEffectView`.
    * `0` - Auto width resize
    * `1` - Auto height resize
    * `2` - Auto width-height resize
    * `3` - No resize

Adds a `NSVisualEffectView` to the window with the specified properties.If you dont specify a `ResizeMask`,default value for it is `2`.


### `UpdateView(window,options)` _macOS_

Returns `Boolean`.

* `window` `BrowserWindow` instance
* `options` Object
  * `ViewId` - Integer. Return value from `AddView`.
  * `Material` - Integer. The Material for `NSVisualEffectMaterial`.See `SetVibrancy` method for material properties.
  * `X` X Position of the `NSVisualEffectView` relative to the main `BrowserWindow`.
  * `Y` X Position of the `NSVisualEffectView` relative to the main `BrowserWindow`.
  * `Width` - Integer Width of the `NSVisualEffectView`. Should not be larger than the window's.
  * `Height` - Integer Height of the `NSVisualEffectView`. Should not be larger than the window's.

Updates the `NSVisualEffectView` with the specified properties.


### `RemoveView(window,viewId)` _macOS_

Returns `Boolean`.

* `window` `BrowserWindow` instance
* `ViewId`- Integer.Identifier of `NSVisualEffectView`.

Removes the `NSVisualEffectView`.



## How to use

```
// Require the module
var electronVibrancy = require('..');
electronVibrancy.SetVibrancy(true,browserWindowInstance.getNativeWindowHandle());


// Preferred Usage

// mainWindow with show: false
mainWindow.on('ready-to-show',function() {
  var electronVibrancy = require('..');
  
  // Whole window vibrancy with Material 0 and auto resize
  electronVibrancy.SetVibrancy(mainWindow, 0);

  // auto resizing vibrant view at {0,0} with size {300,300} with Material 0
  electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:2,Material:0 })

  // non-resizing vibrant view at {0,0} with size {300,300} with Material 0
  electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:3,Material:0 })

  //Remove a view
  var viewId = electronVibrancy.SetVibrancy(mainWindow, 0);
  electronVibrancy.RemoveView(mainWindow,viewId);

  // Add a view then update it
  var viewId = electronVibrancy.SetVibrancy(mainWindow, 0);
  electronVibrancy.UpdateView(mainWindow,{ ViewId: viewId,Width: 600, Height: 600 });

  // Multipe views with different materials
  var viewId1 = electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:3,Material:0 })
  var viewId2 = electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:300,Y:0,ResizeMask:3,Material:2 })

  console.log(viewId1);
  console.log(viewId2);

  // electronVibrancy.RemoveView(mainWindow,0);
  // electronVibrancy.RemoveView(mainWindow,1);

  // or

  electronVibrancy.DisableVibrancy(mainWindow);
})

```


## Screenshots

![](https://cloud.githubusercontent.com/assets/174864/19833319/bc7214f8-9e0b-11e6-8331-be49ca3eeab9.png)

![](https://cloud.githubusercontent.com/assets/174864/19833322/bc7f168a-9e0b-11e6-9c84-c2a746538edc.png)

![](https://cloud.githubusercontent.com/assets/174864/19833327/bc8b2c2c-9e0b-11e6-9272-8d84ad3b7116.png)


## Platform notices

### Windows
On **Windows 10** the addon uses ```SetWindowCompositionAttribute```, which is an undocumented API, which means it can be changed by Microsoft any time and break the functionality.

### MacOS
Requires Yosemite and above.Some materials require 10.11+. Since this is the case, if you use a material that's not available on that macOS version, it will
fallback to the default material value which is `0`, which might not be what you want.


## License

This project is under MIT.
See [LICENSE](https://github.com/arkenthera/electron-vibrancy/blob/master/LICENSE)
