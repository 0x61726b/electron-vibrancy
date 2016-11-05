#----------------------------------------------------------------------------
#Chiika
#Copyright (C) 2016 arkenthera
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2 of the License, or
#(at your option) any later version.
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#Date: 23.1.2016
#authors: arkenthera
#Description:
#----------------------------------------------------------------------------

GlobalSetup               = require './global-setup'
path                      = require 'path'
_                         = require 'lodash'


describe 'Vibrancy', ->
  setup = new GlobalSetup()
  setup.setupTimeout(this)

  runApp = ->
    new Promise (resolve) ->
      setup.startApplication({
        args: [setup.appPath()]
      })
      .then (startedApp) ->
        resolve(startedApp)

  stopApp = (app) ->
    setup.stopApplication(app)

  it 'BrowserWindow with vibrancy enabled should not crash', ->
    runApp().then (app) ->
      app.client.waitUntilWindowLoaded()
      .then ->
        stopApp(app)