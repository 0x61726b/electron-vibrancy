var app = require('electron').app
var BrowserWindow = require('electron').BrowserWindow

var mainWindow = null

app.on('ready', function () {
  mainWindow = new BrowserWindow({
    center: true,
    width: 800,
    height: 600,
    minHeight: 100,
    minWidth: 100,
    transparent: true,
    frame: false,
    show: false
  })
  mainWindow.loadURL('file://' + __dirname + '/index.html')
  mainWindow.on('closed', function () { mainWindow = null })
  mainWindow.on('ready-to-show',function() {
    mainWindow.show();

      var nativeHandleBuffer = mainWindow.getNativeWindowHandle();
      var electronVibrancy = require('..');
      console.log(electronVibrancy.SetVibrancy(true,nativeHandleBuffer));
  })
})
