
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

LIN = $(G2C_SOURCE_DIR)/lin/lin.h
ENVIRONMENTS =\
	$(OBJ_DIR)/environment.o \
	$(OBJ_DIR)/spriteenvironment.o \
	$(OBJ_DIR)/scrollenvironment.o \
	$(OBJ_DIR)/panenvironment.o \
	$(OBJ_DIR)/appenvironment.o
	
APPS = $(OBJ_DIR)/app.o

LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

force_look:
	true

$(OBJ_DIR)/usenamespace.h:
	echo "namespace g2c {} using namespace g2c;" >> $(OBJ_DIR)/usenamespace.h


$(CURRENT): $(LIBG2C) $(LIN) $(ENVIRONMENTS) $(CURRENT).cpp $(OBJ_DIR) $(OBJ_DIR)/usenamespace.h
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GRAPHICS_FRAMEWORKS) $(AUDIO_FRAMEWORKS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) $(SPRITES) \
		$(LIBG2C) \
		$(CURRENT).cpp -include $(OBJ_DIR)/usenamespace.h -o $(CURRENT)


$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/scrollenvironment.o: $(ENVIRONMENTS_DIR)/scrollenvironment.cpp $(ENVIRONMENTS_DIR)/scrollenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/scrollenvironment.cpp -o $(OBJ_DIR)/scrollenvironment.o

$(OBJ_DIR)/panenvironment.o: $(ENVIRONMENTS_DIR)/panenvironment.cpp $(ENVIRONMENTS_DIR)/panenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/panenvironment.cpp -o $(OBJ_DIR)/panenvironment.o

$(OBJ_DIR)/spriteenvironment.o: $(ENVIRONMENTS_DIR)/spriteenvironment.cpp $(ENVIRONMENTS_DIR)/spriteenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/spriteenvironment.cpp -o $(OBJ_DIR)/spriteenvironment.o

$(OBJ_DIR)/appenvironment.o: $(ENVIRONMENTS_DIR)/appenvironment.cpp $(ENVIRONMENTS_DIR)/appenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o


clean:
	-rm $(OBJ_DIR)/*.o
	-rm -rf *.dSYM

