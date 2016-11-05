var app = require('electron').app
var BrowserWindow = require('electron').BrowserWindow
var path = require('path')

var mainWindow = null

app.on('ready', function () {
  mainWindow = new BrowserWindow({
    center: true,
    width: 1400,
    height: 900,
    minHeight: 100,
    minWidth: 100,
    transparent: true,
    frame: false,
    show: false
  })
  mainWindow.loadURL('file://' + __dirname + '/index.html')
  mainWindow.on('closed', function () { mainWindow = null })
  mainWindow.on('ready-to-show',function() {
    var nativeHandleBuffer = mainWindow.getNativeWindowHandle();
    var electronVibrancy = require(path.join(__dirname,'..','..'));
    
    // Whole window vibrancy with Material 0 and auto resize
    electronVibrancy.SetVibrancy(mainWindow, 0);

    // auto resizing vibrant view at {0,0} with size {300,300} with Material 0
    //electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:2,Material:0 })

    // non-resizing vibrant view at {0,0} with size {300,300} with Material 0
    //electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:3,Material:0 })

    // Remove a view
    // var viewId = electronVibrancy.SetVibrancy(mainWindow, 0);
    // electronVibrancy.RemoveView(mainWindow,viewId);

    // Add a view then update it
    // var viewId = electronVibrancy.SetVibrancy(mainWindow, 0);
    // electronVibrancy.UpdateView(mainWindow,{ ViewId: viewId,Width: 600, Height: 600 });

    // Multipe views with different materials
    // var viewId1 = electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:0,Y:0,ResizeMask:3,Material:0 })
    // var viewId2 = electronVibrancy.AddView(mainWindow, { Width: 300,Height:300,X:300,Y:0,ResizeMask:3,Material:2 })

    // console.log(viewId1);
    // console.log(viewId2);

    // // electronVibrancy.RemoveView(mainWindow,0);
    // // electronVibrancy.RemoveView(mainWindow,1);

    // // or

    //electronVibrancy.DisableVibrancy(mainWindow);

    mainWindow.show();

    //electronVibrancy.DisableVibrancy(mainWindow);
  })
})
