
CC=emcc
LDFLAGS := -O2 --llvm-opts 2
G2C_ROOT = ../..
G2C := $(G2C_ROOT)/g2c

EXPORTS := "[ \
	'_init', '_resize', \
	'_step', '_draw', \
	'_mouseDown', '_mouseDragged', '_mouseUp']"

INCLUDES := \
	-I$(G2C) \
	-I.

SOURCE := \
	$(G2C)/app.cpp \
	$(G2C)/util.cpp \
	$(G2C)/listener.cpp \
	$(G2C)/app.cpp \
	$(G2C)/parse.cpp \
	$(G2C)/serializable.cpp \
	$(G2C)/sound.cpp \
	$(G2C)/bank.cpp \
	$(G2C)/emscriptenbank.cpp \
	$(G2C)/texture.cpp \
	$(G2C)/sprites.cpp \
	$(G2C)/fattening.cpp \
    $(G2C)/rendererg.cpp \
	$(G2C)/transforms.cpp \
	$(G2C)/graphics.cpp \
	$(G2C)/wave.cpp \
	$(G2C_ROOT)/examples/quad.cpp \
	main.cpp

main.js: $(SOURCE) makefile
	$(CC) -DEMSCRIPTEN -DSTUB_GL1 $(INCLUDES) $(SOURCE) -s FULL_ES2=1 -s EXPORTED_FUNCTIONS=$(EXPORTS) -o main.js

run: main.js trampoline.js index.html
	open index.html

clean:
	rm main.js

