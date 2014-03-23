
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ../..
G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
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

LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

force_look:
	true


bugworldapp.o: bugworldapp.cpp bugworldapp.h
	c++ $(ODEBUG) $(OPTIMIZATION) $(FLAGS) -I$(G2C_SOURCE_DIR) -c bugworldapp.cpp

bugworldapptest: \
		$(LIBG2C) \
		$(ENVIRONMENTS) \
		bugworldapp.o \
		bugworldapptest.cpp
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) $(GRAPHICS_FRAMEWORKS) $(AUDIO_FRAMEWORKS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) \
		$(LIBG2C) \
		bugworldapp.o \
		bugworldapptest.cpp -o bugworldapptest

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/appenvironment.o: $(ENVIRONMENTS_DIR)/appenvironment.cpp\
		$(ENVIRONMENTS_DIR)/appenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o


clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f triangle
	-rm -rf *.dSYM


