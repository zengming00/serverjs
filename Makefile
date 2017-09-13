SERVERJS=serverjs

CC=gcc

HIREDIS=./hiredis
LIBHIREDIS=./hiredis/libhiredis.a

MUJS=./mujs
LIBMUJS=./mujs/build/release/libmujs.a


all:
	$(MAKE) -C $(HIREDIS)
	$(MAKE) -C $(MUJS)
	$(CC) -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -ffunction-sections -fdata-sections -Os -o main.o -c main.c -I$(MUJS)
	$(CC) -Wall -o js_file.o -c js_file.c -I$(MUJS)
	$(CC) -Wall -o js_redis.o -c js_redis.c -I$(MUJS) -I$(HIREDIS)
	$(CC) -Wl,--gc-sections -Wl,-s -o $(SERVERJS)  main.o js_file.o js_redis.o $(LIBMUJS) $(LIBHIREDIS) -lm


clean:
	$(MAKE) -C $(HIREDIS) clean
	$(MAKE) -C $(MUJS) clean
	rm hiredis-test *.o $(SERVERJS)

