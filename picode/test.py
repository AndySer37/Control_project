#!/usr/bin/python
#encodeing:utf-8

import smbus
import time

bus=smbus.SMBus(1)

address=0x04

def readNumber():
  number=bus.read_byte(address)
  if number == Null:
    return 0
  return number

while True:
 number=readNumber()
 print"Arduino:",number
 print

 time.sleep(1)
