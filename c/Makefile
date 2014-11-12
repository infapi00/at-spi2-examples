CC=gcc
CFLAGS=-O0 -g -Wall -I/usr/include -I/usr/X11R6/include `pkg-config --cflags glib-2.0 gobject-2.0 atk-bridge-2.0 atspi-2`
LDFLAGS=-pthread -L/usr/X11R6/lib -lm `pkg-config --libs glib-2.0 gobject-2.0 atk-bridge-2.0 atspi-2`
EXAMPLES = $(patsubst %.c,%,$(wildcard *.c))

all: $(EXAMPLES)

%:%.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm list-applications
	rm dump-tree
	rm print-focused-selected
	rm notify-value-changes
	rm -f *~

