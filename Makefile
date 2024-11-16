CC=gcc
CFLAGS=-Isrc
DEPS=src/dialog.h

out: out/main
	cp ass/*

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/main: bld/main.o bld/dialog.o
	$(CC) -o out/main bld/*.o
