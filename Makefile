CFLAGS=-Wall -pedantic -Werror -O2

all: libhashmap.a libdynamic_hashmap.so

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c $^ -o $@

libhashmap.a: hashmap.o
	ar rcs $@ $^

libdynamic_hashmap.so: hashmap.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ -lc

clean:
	rm hashmap.o libhashmap.a libdynamic_hashmap.so
