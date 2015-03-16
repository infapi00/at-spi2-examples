#!/usr/bin/python
#
# print_focused_selected.py
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

import pyatspi

""" on_key_input:
                 listener method to allow script to exited
                 whenever the F4 key is pressed.
"""
def on_key_input(event):
  if event.event_string=='F4':
    pyatspi.Registry.stop()
 
""" print_changed:
                  method to print focused and selected events
"""
def print_changed(event):
  if event.type in ('object:state-changed:selected'):
    print("Selected object %s\n %s\n" % (event.source_name, event.source_role))
  if event.type in ('object:state-changed:focused'):
    print("Focused object %s\n %s\n" % (event.source_name, event.source_role))
 
pyatspi.Registry.registerEventListener(print_changed,
                                       'object:state-changed:selected',
                                       'object:state-changed:focused')
pyatspi.Registry.registerKeystrokeListener(on_key_input,
                                           kind=(pyatspi.KEY_PRESSED_EVENT,
                                                 pyatspi.KEY_RELEASED_EVENT))
pyatspi.Registry.start()

