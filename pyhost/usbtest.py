#!/usr/bin/env python

import usb.core
import time
import timeit
import sys
from array import *

dev = usb.core.find(idVendor=0xcafe, idProduct=0xcafe)
if dev is None:
	raise ValueError('Device not found')
	
dev.set_configuration()

i = 0
count = 1000
start = time.time()
while i < count:
    i+=1
    dev.ctrl_transfer(0x40, 0, 0, 0, array('i',[0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]))
    #time.sleep(1)
    dev.ctrl_transfer(0x40, 0, 0, 0, array('i',[0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff]))
stop = time.time()

print (stop-start)/count