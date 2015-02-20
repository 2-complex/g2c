
CC=emcc
LDFLAGS := -O2 --llvm-opts 2
EXPORTS := "['_initGL', '_drawTriangle', '_init', '_resize', '_step', '_draw', '_mouseDown', '_mouseDragged', '_mouseUp']"

INCLUDES := \
	-I../g2c

SOURCE := \
	../g2c/app.cpp \
	../g2c/util.cpp \
	../g2c/listener.cpp \
	../g2c/app.cpp \
	../g2c/parse.cpp \
	../g2c/serializable.cpp \
	../g2c/sound.cpp \
	../g2c/audioplayer.cpp \
	../g2c/bank.cpp \
	../g2c/texture.cpp \
	../g2c/sprites.cpp \
	../g2c/fattening.cpp \
    ../g2c/rendererg.cpp \
	../g2c/transforms.cpp \
	../g2c/graphics.cpp \
	../examples/tetrahedron.cpp \
	main.cpp

main.js: $(SOURCE) makefile
	$(CC) -DEMSCRIPTEN -DSTUB_GL1 $(INCLUDES) $(SOURCE) -s FULL_ES2=1 -s EXPORTED_FUNCTIONS=$(EXPORTS) -o main.js

clean:
	rm glcore.js

