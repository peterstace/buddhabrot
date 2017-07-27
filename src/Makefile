CC=gcc
CFLAGS=-c -Wall -std=c99 -O2
LDFLAGS=-lpthread

COMPUTE_SRC=compute.c array_io.c
COMPUTE_OBJ=$(COMPUTE_SRC:.c=.o)
COMPUTE_H=$(COMPUTE_SRC:.c=.h)
RENDER_SRC=render.c array_io.c bmp_writer.c
RENDER_OBJ=$(RENDER_SRC:.c=.o)
RENDER_H=$(RENDER_SRC:.c=.h)

all: render compute
	
render: $(RENDER_OBJ)
	$(CC) $(LDFLAGS) $(RENDER_OBJ) -o $@

compute: $(COMPUTE_OBJ)
	$(CC) $(LDFLAGS) $(COMPUTE_OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


	
clean:
	rm -f $(COMPUTE_OBJ) $(RENDER_OBJ)
	
clean_all:
	rm -f $(COMPUTE_OBJ) $(RENDER_OBJ) render compute
