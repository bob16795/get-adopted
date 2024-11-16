CC=gcc
CFLAGS=-Isrc
DEPS=src/dialog.h

out: out/main
	cp ass/* out

run: out
	cd out; ./main

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/main: bld/main.o bld/dialog.o
	$(CC) -o out/main bld/*.o

setup:
	mkdir -p out bld
