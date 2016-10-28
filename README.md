#electron-vibrancy

This module is intended to give an [Electron](https://github.com/electron/electron) BrowserWindow blur on its behind. This isnt supported on Electron and this module uses native API calls to achieve the effect.

![](http://i.imgur.com/0sRPzpn.png)


# Running

Since this is a native addon, you will need your platforms build tools. Visual Studio,XCode etc.
```
npm install
npm test
```

```npm install``` will compile the addon for Electron ```1.4.2```, if you are using different version, change the electron version at ```package.json``` scripts.


# Things to note
- Window must be transparent.
- You can only blur the whole window, therefore only blurring behind a single element isnt possible.
- If your window has a frame, the frame will also be blurred.
- Requires Yosemite on macOS.
- On Windows 8.x, this wont work because Microsoft completely removed Aero Glass feature. It is still possible though but it is not something that an end user should do.
See Platforms section for more info.
- It is known that **Dev Tools attached to the window** *sometimes* breaks transparency, it is not related to this module.

# API
There are several methods depending on what you want to do and what platform you are on.

### `SetVibrancy(window, material)` _win_ , _macOS_

Returns `Integer`.View id of `NSVisualEffectView`. You need this for `UpdateView` or `RemoveView`.

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

Adds a `NSVisualEffectView` to the window with the specified properties.If you dont specify a `ResizeMask`, you have to use `UpdateView` or the `NSVisualEffectView` will be static.


### `UpdateView(windowBuffer,options)` _macOS_

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


### `RemoveView(window,options)` _macOS_

Returns `Boolean`.

* `window` `BrowserWindow` instance
* `options` Object
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


## Platform notices

### Windows
On **Windows 10** the addon uses ```SetWindowCompositionAttribute```, which is an undocumented API, which means it can be changed by Microsoft any time and break the functionality.

On **Windows 8.x**, Aero Glass or any other blurring feature is removed by Microsoft.But you can use [Aero Glass](http://www.glass8.eu/) to enable it again. If you install it, the addon calls
`DwmEnableBlurBehindWindow` and the window will blend with whats behind.

![](http://i.imgur.com/4gpkRm5.jpg)

### MacOS
Requires Yosemite and above.

### Linux
Soon™


## License

This project is under MIT.
See [LICENSE](https://github.com/arkenthera/electron-vibrancy/blob/master/LICENSE)
