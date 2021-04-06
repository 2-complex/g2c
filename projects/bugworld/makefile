
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

CELLO_ROOT_DIR = ../..
CELLO_SOURCE_DIR = $(CELLO_ROOT_DIR)/cello
ENVIRONMENTS_DIR = $(CELLO_ROOT_DIR)/environments
OBJ_DIR = obj


ifneq ($(MAKECMDGOALS), clean)

CURRENT = $(MAKECMDGOALS)

run: bugworldapptest
	./bugworldapptest $(CURRENTARGS)
endif


FLAGS = -DGLUT
GRAPHICS_FRAMEWORKS = -framework Cocoa -framework QuartzCore -framework GLUT -framework OpenGL
AUDIO_FRAMEWORKS = -framework AudioToolbox -framework Foundation -framework OpenAL

ENVIRONMENTS =\
	$(OBJ_DIR)/environment.o \
	$(OBJ_DIR)/appenvironment.o

LIBCELLO = $(CELLO_SOURCE_DIR)/libcello.a

$(LIBCELLO): force_look
	cd $(CELLO_SOURCE_DIR) && $(MAKE) libcello.a

force_look:
	true


bugworldapp.o: bugworldapp.cpp bugworldapp.h
	c++ $(ODEBUG) $(OPTIMIZATION) $(FLAGS) -I$(CELLO_SOURCE_DIR) -c bugworldapp.cpp

bugworldapptest: \
		$(LIBCELLO) \
		$(ENVIRONMENTS) \
		bugworldapp.o \
		bugworldapptest.cpp
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) $(GRAPHICS_FRAMEWORKS) $(AUDIO_FRAMEWORKS) \
		-I$(CELLO_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) \
		$(LIBCELLO) \
		bugworldapp.o \
		bugworldapptest.cpp -o bugworldapptest

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(CELLO_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/appenvironment.o: $(ENVIRONMENTS_DIR)/appenvironment.cpp\
		$(ENVIRONMENTS_DIR)/appenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(CELLO_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o


clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f triangle
	-rm -rf *.dSYM


