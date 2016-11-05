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
{Application}           = require 'spectron'
assert                = require 'assert'
chai                  = require 'chai'
chaiAsPromised        = require 'chai-as-promised'
path                  = require 'path'

global.before ->
  chai.should()
  chai.use(chaiAsPromised)

module.exports = class Setup
  appPath: ->
    path.join(__dirname, 'app')
  getElectronPath: ->
    electronPath = path.join(__dirname, '..', 'node_modules','.bin','electron')
    console.log(electronPath)
    if (process.platform == 'win32')
      electronPath += '.cmd'
    electronPath

  setupTimeout: (test) ->
    if (process.env.CI)
      test.timeout(100000)
    else
      test.timeout(20000)

  startApplication: (options) ->
    options.path = @getElectronPath()


    options.env = Object.create(process.env)
    app = new Application(options)

    app.start().then ->
      assert.equal(app.isRunning(), true)
      chaiAsPromised.transferPromiseness = app.transferPromiseness
      app

  prettyPrintMainProcessLogs: (client) ->
    client.getMainProcessLogs().then (logs) ->
      _.forEach logs, (v,k) ->
        console.log v

  prettyPrintRendererProcessLogs: (client) ->
    client.getRenderProcessLogs().then (logs) ->
      _.forEach logs, (v,k) ->
        console.log v.message

  stopApplication:(app) ->
    if (!app || !app.isRunning())
      return

    app.stop()