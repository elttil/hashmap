CFLAGS=-Wall -pedantic -Werror -O2

all: libhashmap.a libdynamic_hashmap.so

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c $^ -o $@

hash.o: hash.c
	$(CC) $(CFLAGS) -c $^ -o $@

libhashmap.a: hashmap.o hash.o
	ar rcs $@ $^

libdynamic_hashmap.so: hashmap.c hash.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ -lc

clean:
	rm hashmap.o hash.o libhashmap.a libdynamic_hashmap.so
