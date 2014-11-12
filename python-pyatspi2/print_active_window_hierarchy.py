#!/usr/bin/python
#
# print_active_window_hierarchy.py
#
# Copyright (c) Magdalen Berns 2014 <m.berns@thismagpie.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., Franklin Street, Fifth Floor,
# Boston MA  02110-1301 USA.
#
# A pyatspi2 example demonstating a client listener for Keypresses and releases
# in pyatspi2. If F3 is pressed then the active window is printed. If F4 is
# pressed then the program exits.

import pyatspi

""" on_key_input:
                  Callback to print the active window on key press and filter
                  out the key release
  @event:
                  keypress event
  @return:
                  Whether the keypress F3 or F4
"""
def on_key_input(event):

  if event.type == pyatspi.KEY_RELEASED_EVENT:
    return False
  if event.event_string=='F4':
    pyatspi.Registry.stop()
    return True
  if event.event_string =='F3':
    print_tree(0, active_window())
    return True

""" active_window:
                  Method for figuring out which window out of all the windows
                  in the desktop is the active one
  @return:
                  The Active window
"""
def active_window():
  desktop = pyatspi.Registry.getDesktop(0)
  for app in desktop:
    for window in app:
      if window.getState().contains(pyatspi.STATE_ACTIVE):
        return window

""" print_tree:
                  Recursive method which formats for print
"""
def print_tree(index, root):
  print ("%s-> %s" % (" " * index, root))
  for tree in root:
    print_tree(index + 1, tree)

""" print_prompt:
                  Prints the user prompt message
"""
def print_prompt():
  print("Press F3 to print the accessible hierarchy of the active window.")
  print("Press F4 to exit.")

print_prompt()
pyatspi.Registry.registerKeystrokeListener(on_key_input,
                                           kind=(pyatspi.KEY_PRESSED_EVENT,
                                           pyatspi.KEY_RELEASED_EVENT))
pyatspi.Registry.start()
