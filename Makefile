EXEC=projet
SOURCES=json.c color.c scene3D.c main.c
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall -g
 
.PHONY: default clean
 
default: $(EXEC)
 
json.o : json.h json.c
color.o : json.h color.h color.c
scene3D.o : json.h color.h scene3D.h scene3D.c 
main.o: main.c scene3D.c
 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~