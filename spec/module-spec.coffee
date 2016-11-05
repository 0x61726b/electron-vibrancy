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

path                      = require 'path'
_                         = require 'lodash'
GlobalSetup               = require './global-setup'

dummyBrowserWindow =
    getNativeWindowHandle: ->
      new Buffer([0])
    getSize: ->
      [0,0]

describe 'Module General', ->
  vibrancy = require(path.join(__dirname,'..'))

  it 'SetVibrancy with no buffer argument should return -1',->
    result = vibrancy.SetVibrancy()
    result.should.be.equal(-1)


  #it 'SetVibrancy should return Integer',->
  #result = vibrancy.SetVibrancy(dummyBrowserWindow,0)
  #(typeof result).should.be.equal('number')

