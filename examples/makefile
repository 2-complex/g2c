
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

GRAPHICS = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/bank.o $(OBJ_DIR)/macbank.o $(OBJ_DIR)/graphics.o
SPRITES = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/bank.o $(OBJ_DIR)/macbank.o $(OBJ_DIR)/renderer.o $(OBJ_DIR)/sprites.o $(OBJ_DIR)/animations.o
GAMES = $(SPRITES) $(WEBCLIENT) $(OBJ_DIR)/graphics.o

ENVIRONMENTS =\
	$(G2C_SOURCE_DIR)/lin/lin.h\
	$(OBJ_DIR)/environment.o\
	$(OBJ_DIR)/appenvironment.o

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

force_look:
	true

clean:
	-rm $(OBJ_DIR)/*.o

