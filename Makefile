CC=gcc
CFLAGS=-Isrc -Ilib/include
LFLAGS=-Llib -lraylib -lm
DEPS=src/dialog.h src/ui.h src/scene.h src/character.h src/button.h src/textbox.h

WEBCC=emcc
WEBCFLAGS=-Isrc -Ilib/include
WEBLFLAGS=-Isrc -Ilib/include -Llib/web -lraylib -lm -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -s MAXIMUM_MEMORY=4gb -s ASSERTIONS --preload-file ass@ --shell-file ass/web/minshell.html -DPLATFORM_WEB -s EXPORTED_FUNCTIONS=[\"_free\",\"_malloc\",\"_main\"] -s EXPORTED_RUNTIME_METHODS=ccall -s STACK_SIZE=1gb -s INITIAL_MEMORY=2gb

out: out/main
	cp -r ass/* out

run: out
	cd out; ./main

bld/web/%.o: src/%.c $(DEPS)
	$(WEBCC) -c -o $@ $< $(WEBCFLAGS)

bld/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out/web/index.html: setup bld/web/scene.o bld/web/main.o bld/web/dialog.o bld/web/ui.o bld/web/character.o bld/web/button.o bld/web/textbox.o
	$(WEBCC) -o out/web/index.html bld/web/*.o $(WEBLFLAGS)

out/main: setup bld/main.o bld/scene.o bld/dialog.o bld/ui.o bld/character.o bld/button.o bld/textbox.o
	$(CC) -o out/main bld/*.o $(LFLAGS)

setup:
	mkdir -p out bld
	mkdir -p out/web bld/web

web: out/web/index.html