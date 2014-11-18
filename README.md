# Main topics
## at-spi2-examples

This repository contains several examples of usage of the library
libatspi, included with at-spi2, on the most common languages right
now. There are one directory per language. Right now those are:

* C : using directly libatspi2
* pyatspi2: using pyatspi2 python bindings
* python-atspi: using gobject introspection python bindings
* javascript: using gobject introspection javascript bindings

Each directory contains a README explaining how to run them.

We would like to keep all the languages synced. So for each example,
ideally we would like to be implemented on all the languages, keeping
the same name.

## What those programs are, again?

These programs are *examples*. So:

* Each example should focus on just one thing (or two) and being
  small. Full programs should have their own repository/project, like
  [Orca](https://git.gnome.org/browse/orca).

* They are not full debug tools. They could be used as a quick debug
  fix, but if you are interested on a real accessibility debug tool,
  please take a look to
  [Accerciser](https://git.gnome.org/browse/accerciser).

* They are not tests. Automatic tests are being added to
  at-spi2-core/atk at this moment, and their purpose is different, so
  the code.

# Appendix: explaining concepts

## What is AT-SPI2?

AT-SPI stands for Assistive Technology Service Provider Interface, and
it is the facto standard for providing accessibility to free and open
desktops, like GNU/Linux or OpenBSD, leading by the GNOME Project. The
current maintained project is in fact AT-SPI-2, that is the second
version of the same framework, mostly backwards compatible.

It provides a way to communicate client programs (Assistive Tools) and
server programs (the programs running on the desktop)
bidirectionally. The underlying technology to do this communication in
AT-SPI-2 is DBUS. On the case of AT-SPI1, it was CORBA.

## What is libatspi?

To leverage the work of clients programs to call directly DBUS, a
library was created to provide that functionally in a transparent way,
called libatspi. It is also the basis for all current bindings to
interact with at-spi, client side.

## Bindings

Although libatspi is a C library, it is not not API compatible with
libcspi. libcspi was the c-bindings for at-spi1.

In the old days, several bindings to other languages were created
manually. As part of this effort, pyatspi2 was created. pyatspi2
maintained the same API on both at-spi1 and at-spi2 (or at least,
mostly).

In current days, most bindings are created automatically using
gobject-introspection. libatspi is also gobject introspectable, so
there is available bindings created automatically to at-spi. That
includes python, and we usually call that python-atspi. Note that
python-atspi and pyatspi2 API are slightly different.

javascript bindings are used on gnome-shell successfully. Probably
there are other bindings available, but those are not on use right
now.

In summary, if you want to create a new AT, the language options are
the following:

* C: use libatspi directly
* python: pyatspi2 or python-atspi (gobject-introspection python bindings)
* javascript: gobject-introspection javascript bindings

## at-spi examples, why a different repository?

Current libatspi documentation only includes the [API
reference](https://developer.gnome.org/libatspi/2.15/), and at-spi2
repositories doesn't include examples right now. In the same way, the
only complete AT using atspi, Orca, is too big to handle for a
newcomer. Having some example using that library is a good complement
as documentation.

The main reason to keep these examples as a different project, instead
of adding them directly on at-spi2 repositories is avoid to add extras
dependencies needed to include the examples using the specific
bindings to other languages.
