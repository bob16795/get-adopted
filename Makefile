CC=gcc
CFLAGS=-Isrc -Ilib/include
DEPS=src/dialog.h src/ui.h

out: out/main
	cp ass/* out
	cp lib/

run: out
	cd out; ./main

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/main: bld/main.o bld/dialog.o bld/ui.o
	$(CC) -Llib -o out/main bld/*.o -lraylib -lm

setup:
	mkdir -p out bld
