#!/usr/bin/env python

import usb.core
import pygtk
pygtk.require('2.0')
import gtk

if __name__ == "__main__":
	dev = usb.core.find(idVendor=0xcafe, idProduct=0xcafe)
	if dev is None:
		raise ValueError('Device not found')

	dev.set_configuration()

	w = gtk.Window()
	w.connect("destroy", gtk.main_quit)
	toggle = gtk.ToggleButton("Toggle LED")
	def toggled(button):
		dev.ctrl_transfer(0x40, 0, button.get_active(), 0, 'Hello World!')
		print "hello"
	toggle.connect("toggled", toggled)
	w.add(toggle)
	w.show_all()
	gtk.main()

