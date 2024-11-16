CC=gcc
CFLAGS=-Isrc -Ilib/include
DEPS=src/dialog.h src/ui.h src/character.h src/button.h src/textbox.h

out: out/main
	cp -r ass/* out

run: out
	cd out; ./main

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/main: bld/main.o bld/dialog.o bld/ui.o bld/character.o bld/button.o bld/textbox.o
	$(CC) -Llib -o out/main bld/*.o -lraylib -lm

setup:
	mkdir -p out bld
