#!/usr/bin/python
#
# print_window_signals.py
#
# Copyright (C) 2015 Magdalen Berns <m.berns@thismagpie.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., Franklin Street, Fifth Floor,
# Boston MA  02110-1301 USA.
#
# This example demonstrates a windows signals listener (i.e. print_event)

import pyatspi

""" on_key_input:
                 listener method to allow script to exited
                 whenever the F4 key is pressed.
"""
def on_exit_key(e):
  if e.event_string=='F4':
    pyatspi.Registry.stop()
 
""" print_event:
                function to print registered window events
"""
def print_event(e):
    print("Signal: " + e.type)

pyatspi.Registry.registerEventListener(print_event, "window:activate")
pyatspi.Registry.registerEventListener(print_event, "window:create")
pyatspi.Registry.registerEventListener(print_event, "window:deactivate")
pyatspi.Registry.registerEventListener(print_event, "window:destroy")
pyatspi.Registry.registerEventListener(print_event, "window:maximize")
pyatspi.Registry.registerEventListener(print_event, "window:minimize")
pyatspi.Registry.registerEventListener(print_event, "window:move")
pyatspi.Registry.registerEventListener(print_event, "window:restore")

pyatspi.Registry.registerKeystrokeListener(on_exit_key,
                                           kind=(pyatspi.KEY_PRESSED_EVENT,
                                                 pyatspi.KEY_RELEASED_EVENT))
pyatspi.Registry.start()

