#!/usr/bin/python
#
# print_text_at_offset.py
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
# This example demonstrates how the pyatspi2 text interface can be used.

import pyatspi

""" on_key_input:
                 listener method to allow script to exited
                 whenever the F4 key is pressed.
"""
def on_key_input(event):
  if event.event_string=='F4':
    pyatspi.Registry.stop()

""" on_key_input:
                 caret method to call print_text_at_offset method and ignore
                 terminal events.
"""
def on_caret_move(event):
    if event.source and event.source.getRole() == pyatspi.ROLE_TERMINAL:
        return
    print_text_at_offset(event.source, event.detail1)

""" char_at_offset:
                   method to query text interface
    @param obj     instance
    @param offet   position of the caret offset.
    @return:       character which is at the offset of the caret
"""
def char_at_offset(obj, offset):
  try:
    text = obj.queryText()
  except:
    return
  else:
    return text.getTextAtOffset(offset, pyatspi.TEXT_BOUNDARY_CHAR)

""" word_at_offset:
                       method to query text interface
    @param obj         instance
    @param offet       position of the caret offset.
    @return:           word which is at the offset of the caret
"""
def word_at_offset(obj, offset):
  try:
    text = obj.queryText()
  except:
    return
  else:
    return text.getTextAtOffset(offset, pyatspi.TEXT_BOUNDARY_WORD_START)

""" sentence_at_offset:
                       method to query text interface
    @param obj         instance
    @param offet       position of the caret offset.
    @return:           sentence which is at the offset of the caret
"""
def sentence_at_offset(obj, offset):
  try:
    text = obj.queryText()
  except:
    return
  else:
    return text.getTextAtOffset(offset, pyatspi.TEXT_BOUNDARY_SENTENCE_START)

""" line_at_offset:
                       method to query text interface
    @param obj         instance
    @param offet       position of the caret offset.
    @return:           line which is at the offset of the caret
"""
def line_at_offset(obj, offset):
  try:
    text = obj.queryText()
  except:
    return
  else:
    return text.getTextAtOffset(offset, pyatspi.TEXT_BOUNDARY_LINE_START)

""" print_text_at_offset:
                       method to query text interface
    @param obj         instance
    @param offet       position of the caret offset.
"""
def print_text_at_offset(obj, offset):
  char, char_start_offset, char_end_offset = char_at_offset(obj, offset)
  word, word_start_offset, word_end_offset = word_at_offset(obj, offset)
  sentence, sentence_start_offset, sentence_end_offset = sentence_at_offset(obj, offset)
  line, line_start_offset, line_end_offset = line_at_offset(obj, offset)
  print("\n\nChar:%s \nWord:%s \nSentence:%s Line:%s " % (char, word, sentence, line))

pyatspi.Registry.registerEventListener(on_caret_move, "object:text-caret-moved")
pyatspi.Registry.registerKeystrokeListener(on_key_input,
                                           kind=(pyatspi.KEY_PRESSED_EVENT,
                                                 pyatspi.KEY_RELEASED_EVENT))
pyatspi.Registry.start()

