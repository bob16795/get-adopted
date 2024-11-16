CC=gcc
CFLAGS=-Isrc -Ilib/include
DEPS=src/dialog.h src/ui.h

out: out/main
	cp ass/* out

run: out
	cd out; ./main

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/main: bld/main.o bld/dialog.o bld/ui.o
	$(CC) -o out/main -l raylib bld/*.o

setup:
	mkdir -p out bld
