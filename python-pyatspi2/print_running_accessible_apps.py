#!/usr/bin/python
#
# print_running_accessible_apps.py
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
# A pyatspi2 example to demonstrate how to find the number of accessible
# applications currently running on the desktop.

import pyatspi

""" on_key_input:
                  Callback to print the active window on key press and filter
                  out the key release
  @event:
                  keypress event
  @return:
                  Whether the keypress was a 'p', a 'q' (other keypress events
                  will get ignored)
"""
def on_key_input(event):
  if event.type == pyatspi.KEY_RELEASED_EVENT:
    return False
  if event.event_string=='q':             # User chose to quit
    pyatspi.Registry.stop()
    return True
  if event.event_string =='p':            # User chose to print apps
    print_running_apps()
    print("")
    user_prompt()
    return True

""" print_running_apps:
                       Method for printing running accessible applications
"""
def print_running_apps():
  desktop = pyatspi.Registry.getDesktop(0)
  print ("Desktop is running %s applications:\n" % (desktop.childCount))
  for app in desktop:
    print ("    " + app.name)

""" user_prompt:
                       Method for printing running accessible applications
"""
def user_prompt():
  print("Press p to print the running apps.\nPress q to exit.")

user_prompt()
pyatspi.Registry.registerKeystrokeListener(on_key_input, kind=(pyatspi.KEY_PRESSED_EVENT, pyatspi.KEY_RELEASED_EVENT))
pyatspi.Registry.start()

