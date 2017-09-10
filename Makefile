SERVERJS=serverjs

CC=gcc

HIREDIS=./hiredis
LIBHIREDIS=./hiredis/libhiredis.a

MUJS=./mujs
LIBMUJS=./mujs/build/release/libmujs.o


all:
	$(MAKE) -C $(HIREDIS)
	$(CC) -o hiredis-test -O3 -fPIC  -Wall -W -Wstrict-prototypes -Wwrite-strings -g -ggdb  -I$(HIREDIS) hiredis-test.c $(LIBHIREDIS) 
	$(MAKE) -C $(MUJS)
	$(CC) -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -ffunction-sections -fdata-sections -Os -o main.o -c main.c -I$(MUJS)
	$(CC) -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -ffunction-sections -fdata-sections -Os -o js_file.o -c js_file.c -I$(MUJS)
	$(CC) -Wl,--gc-sections -Wl,-s -o $(SERVERJS) $(LIBMUJS) main.o js_file.o -lm


clean:
	$(MAKE) -C $(HIREDIS) clean
	$(MAKE) -C $(MUJS) clean
	rm hiredis-test *.o $(SERVERJS)

