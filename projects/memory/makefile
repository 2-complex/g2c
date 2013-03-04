
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ../..

G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
OBJ_DIR = obj

CURRENT = test

current: $(CURRENT)

run: current
	./$(CURRENT) $(CURRENTARGS)

FLAGS = -DGLUT
GLFLAGS = -framework GLUT -framework OpenGL
GRAPHICSFLAGS = -framework Cocoa -framework QuartzCore
AUDIOFLAGS = -framework AudioToolbox -framework Foundation -framework OpenAL

ENVIRONMENTS =\
	$(G2C_SOURCE_DIR)/lin/lin.h\
	$(OBJ_DIR)/environment.o\
	$(OBJ_DIR)/appenvironment.o

LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

test: $(ENVIRONMENTS) $(APPS) $(SPRITES) $(OBJ_DIR)/memoryapp.o test.cpp $(LIBG2C)
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) $(APPS) $(SPRITES) \
		$(GLFLAGS) $(AUDIOFLAGS) $(GRAPHICSFLAGS) \
		$(LIBG2C) \
		$(OBJ_DIR)/memoryapp.o test.cpp -o test

$(OBJ_DIR)/memoryapp.o: memoryapp.cpp memoryapp.h
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR)\
		-c memoryapp.cpp -o $(OBJ_DIR)/memoryapp.o

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR)\
		-c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/appenvironment.o: $(ENVIRONMENTS_DIR)/appenvironment.cpp\
		$(ENVIRONMENTS_DIR)/appenvironment.h
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION)\
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR)\
		-c $(ENVIRONMENTS_DIR)/appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o

force_look:
	true

clean:
	-rm $(OBJ_DIR)/*.o

