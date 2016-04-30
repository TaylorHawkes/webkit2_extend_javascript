CC=gcc
CFLAGS=-Wall -c -g -I$(pwd) $(shell pkg-config --cflags  webkit2gtk-4.0) 
LDFLAGS=$(shell pkg-config --libs webkit2gtk-4.0 )

SOURCES= js_extensions.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=browser

all: modules $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

modules: module.c modules/module.so
	$(CC) module.c -shared -fPIC -o modules/module.so  $(shell pkg-config --cflags  webkit2gtk-4.0 libpjproject) 

modules/module.so: module.c

.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -fr *.o $(EXECUTABLE) modules/*.so

