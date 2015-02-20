
CC=emcc
LDFLAGS := -O2 --llvm-opts 2
EXPORTS := "['_initGL', '_drawTriangle', '_init', '_resize', '_step', '_draw', '_mouseDown', '_mouseDragged', '_mouseUp']"


SOURCE := \
	main.cpp

main.js: $(SOURCE) makefile
	$(CC) $(SOURCE) -s FULL_ES2=1 -s EXPORTED_FUNCTIONS=$(EXPORTS) -o main.js

clean:
	rm glcore.js

