#electron-vibrancy

This module is intended to give an [Electron](https://github.com/electron/electron) BrowserWindow blur on its behind. This isnt supported on Electron and this module uses native API calls to achieve the effect.

![](http://i.imgur.com/yvkK4mY.png)

![](http://i.imgur.com/IgN5KDJ.png)


# Things to note
- Window must be transparent.
- You can only blur the whole window, therefore only blurring behind a single element isnt possible.
- If your window has a frame, the frame will also be blurred.

## How to use

```
// Require the module
var electronVibrancy = require('..');
electronVibrancy.SetVibrancy(true,browserWindowInstance.getNativeWindowHandle());


// Preferred Usage

// mainWindow with show: false
mainWindow.on('ready-to-show',function() {
  var electronVibrancy = require('..');
  electronVibrancy.SetVibrancy(true,mainWindow.getNativeWindowHandle());
  mainWindow.show();
})

```


## Platform notices

### Windows
The addon use ```SetWindowCompositionAttribute``` on Windows 10, which is an undocumented API, which means it can be changed by Microsoft any time and break the functionality.
