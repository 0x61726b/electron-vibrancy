var app = require('electron').app
var BrowserWindow = require('electron').BrowserWindow

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
      var electronVibrancy = require('..');
      var vib = electronVibrancy.SetVibrancy(true,nativeHandleBuffer, { Material: 0 });
      console.log(vib);

      mainWindow.show();
      mainWindow.openDevTools({detach: true});

      // function removeVibrancy() {
      //   electronVibrancy.SetVibrancy(false,nativeHandleBuffer);
      //   console.log("removed");
      // }
      // setTimeout(removeVibrancy,3000);

      //Windows8.1 hack
      //mainWindow.minimize();
      //mainWindow.restore();
  })
})
