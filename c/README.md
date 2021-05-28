## Dependencies

You need the development packages for atk, atk-bridge and at-spi2. As
a reference, on Ubuntu you would install them with the following
commands:

 $ sudo apt-get install libatk1.0-dev libatk-bridge2.0-dev libatspi2.0-dev


## Building

There is a custom Makefile to build all the C files. So you just need
to do the following:

 $ make

And if you want to clean up all the binaries

 $ make clear

## To Run Examples

If everything went well, the make command would create some
binaries. So to execute any of them, just execute this on the
terminal:

` ./$EXAMPLE_BINARY`

Where $EXAMPLE_BINARY is the name of the specific binary built

## Documentation

Each example includes their documentation at the file header.

