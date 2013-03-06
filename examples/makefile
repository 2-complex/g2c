
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

OBJ_DIR = obj

CURRENT = test

ifneq ($(MAKECMDGOALS), clean)

CURRENT = $(MAKECMDGOALS)

current: $(CURRENT)

run: current
	./$(CURRENT) $(CURRENTARGS)
endif

G2C_ROOT_DIR = ..

G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
OBJ_DIR = obj

FLAGS = -DGLUT
GLFLAGS = -framework GLUT -framework OpenGL
GRAPHICSFLAGS = -framework Cocoa -framework QuartzCore
AUDIOFLAGS = -framework AudioToolbox -framework Foundation -framework OpenAL

ENVIRONMENTS =\
	$(G2C_SOURCE_DIR)/lin/lin.h\
	$(OBJ_DIR)/environment.o\
	$(OBJ_DIR)/spriteenvironment.o

LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a


$(CURRENT): $(ENVIRONMENTS) $(SPRITES) $(CURRENT).cpp
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) $(APPS) $(SPRITES) \
		$(GLFLAGS) $(AUDIOFLAGS) $(GRAPHICSFLAGS) \
		$(LIBG2C) \
		$(CURRENT).cpp -o $(CURRENT)

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR)\
		-c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/spriteenvironment.o: $(ENVIRONMENTS_DIR)/spriteenvironment.cpp\
		$(ENVIRONMENTS_DIR)/spriteenvironment.h
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR)\
		-c $(ENVIRONMENTS_DIR)/spriteenvironment.cpp -o $(OBJ_DIR)/spriteenvironment.o

force_look:
	true

clean:
	-rm $(OBJ_DIR)/*.o

