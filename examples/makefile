
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ..
G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
OBJ_DIR = obj


ifneq ($(MAKECMDGOALS), clean)

CURRENT = $(MAKECMDGOALS)

current: $(CURRENT)

run: current
	./$(CURRENT) $(CURRENTARGS)
endif


FLAGS = -DGLUT
GRAPHICS_FRAMEWORKS = -framework Cocoa -framework QuartzCore -framework GLUT -framework OpenGL
AUDIO_FRAMEWORKS = -framework AudioToolbox -framework Foundation -framework OpenAL

GRAPHICS_LIBRARIES = -lGL -lGLU -lGLEW -lglut -lpng
AUDIO_LIBRARIES = -lopenal

ENVIRONMENTS = \
	$(OBJ_DIR)/environment.o \
	$(OBJ_DIR)/scrollenvironment.o \
	$(OBJ_DIR)/panenvironment.o \
	$(OBJ_DIR)/spriteenvironment.o

LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

force_look:
	true


$(CURRENT): $(LIBG2C) $(ENVIRONMENTS) $(G2C_SOURCE_DIR)/lin/lin.h $(CURRENT).cpp
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) \
		$(LIBG2C) \
		$(CURRENT).cpp -o $(CURRENT) \
		$(GRAPHICS_LIBRARIES) $(AUDIO_LIBRARIES)\

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/spriteenvironment.o: $(ENVIRONMENTS_DIR)/spriteenvironment.cpp\
		$(ENVIRONMENTS_DIR)/spriteenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/spriteenvironment.cpp -o $(OBJ_DIR)/spriteenvironment.o

$(OBJ_DIR)/panenvironment.o: $(ENVIRONMENTS_DIR)/panenvironment.cpp\
		$(ENVIRONMENTS_DIR)/panenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/panenvironment.cpp -o $(OBJ_DIR)/panenvironment.o

$(OBJ_DIR)/scrollenvironment.o: $(ENVIRONMENTS_DIR)/scrollenvironment.cpp\
		$(ENVIRONMENTS_DIR)/scrollenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		-c $(ENVIRONMENTS_DIR)/scrollenvironment.cpp -o $(OBJ_DIR)/scrollenvironment.o


clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f triangle
	-rm -rf *.dSYM

