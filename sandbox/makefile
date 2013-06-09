
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)
SOURCE_DIR = ../g2c
ENVIRONMENTS_DIR = ../environments
OBJ_DIR = obj

CURRENT = test

ifneq ($(MAKECMDGOALS), clean)

CURRENT = $(MAKECMDGOALS)

current: $(CURRENT)

run: current
	./$(CURRENT) $(CURRENTARGS)
endif


FLAGS = -DGLUT
GLFLAGS = -framework GLUT -framework OpenGL
GRAPHICSFLAGS = -framework Cocoa -framework QuartzCore
AUDIOFLAGS = -framework AudioToolbox -framework Foundation -framework OpenAL

GRAPHICS = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/bank.o $(OBJ_DIR)/macbank.o $(OBJ_DIR)/graphics.o
SPRITES = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/bank.o $(OBJ_DIR)/macbank.o $(OBJ_DIR)/wave.o $(OBJ_DIR)/sprites.o $(OBJ_DIR)/animations.o
GAMES = $(SPRITES) $(WEBCLIENT) $(OBJ_DIR)/graphics.o

ENVIRONMENT = $(SOURCE_DIR)/lin/lin.h $(OBJ_DIR)/util.o $(OBJ_DIR)/listener.o $(OBJ_DIR)/environment.o
ENVIRONMENTS = $(ENVIRONMENT) $(OBJ_DIR)/spriteenvironment.o $(OBJ_DIR)/scrollenvironment.o $(OBJ_DIR)/panenvironment.o $(OBJ_DIR)/appenvironment.o
APPS = $(OBJ_DIR)/app.o


$(OBJ_DIR)/usenamespace.h:
	echo "namespace g2c {} using namespace g2c;" >> $(OBJ_DIR)/usenamespace.h

$(CURRENT): $(ENVIRONMENTS) $(SPRITES) $(CURRENT).cpp $(OBJ_DIR) $(OBJ_DIR)/usenamespace.h
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(ENVIRONMENTS) $(SPRITES) \
		$(GLFLAGS) $(AUDIOFLAGS) $(GRAPHICSFLAGS) \
		$(CURRENT).cpp -include $(OBJ_DIR)/usenamespace.h -o $(CURRENT)


$(OBJ_DIR)/listener.o: $(SOURCE_DIR)/listener.cpp $(SOURCE_DIR)/listener.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -c $(SOURCE_DIR)/listener.cpp -o $(OBJ_DIR)/listener.o -o $(OBJ_DIR)/listener.o

$(OBJ_DIR)/environment.o: $(ENVIRONMENTS_DIR)/environment.cpp $(ENVIRONMENTS_DIR)/environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/scrollenvironment.o: $(ENVIRONMENTS_DIR)/scrollenvironment.cpp $(ENVIRONMENTS_DIR)/scrollenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/scrollenvironment.cpp -o $(OBJ_DIR)/scrollenvironment.o

$(OBJ_DIR)/panenvironment.o: $(ENVIRONMENTS_DIR)/panenvironment.cpp $(ENVIRONMENTS_DIR)/panenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/panenvironment.cpp -o $(OBJ_DIR)/panenvironment.o

$(OBJ_DIR)/spriteenvironment.o: $(ENVIRONMENTS_DIR)/spriteenvironment.cpp $(ENVIRONMENTS_DIR)/spriteenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/spriteenvironment.cpp -o $(OBJ_DIR)/spriteenvironment.o

$(OBJ_DIR)/appenvironment.o: $(ENVIRONMENTS_DIR)/appenvironment.cpp $(ENVIRONMENTS_DIR)/appenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -I$(ENVIRONMENTS_DIR) -c $(ENVIRONMENTS_DIR)/appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o


$(OBJ_DIR)/util.o: $(SOURCE_DIR)/util.cpp $(SOURCE_DIR)/util.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/util.cpp -o $(OBJ_DIR)/util.o

$(OBJ_DIR)/sound.o: $(SOURCE_DIR)/sound.cpp $(SOURCE_DIR)/sound.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/sound.cpp -o $(OBJ_DIR)/sound.o

$(OBJ_DIR)/wave.o: $(SOURCE_DIR)/wave.cpp $(SOURCE_DIR)/wave.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/wave.cpp -o $(OBJ_DIR)/wave.o

$(OBJ_DIR)/rendertarget.o: $(SOURCE_DIR)/rendertarget.cpp $(SOURCE_DIR)/rendertarget.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/rendertarget.cpp -o $(OBJ_DIR)/rendertarget.o

$(OBJ_DIR)/graphics.o: $(SOURCE_DIR)/graphics.cpp $(SOURCE_DIR)/graphics.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c graphics.cpp -o $(OBJ_DIR)/graphics.o

$(OBJ_DIR)/texture.o: $(SOURCE_DIR)/texture.cpp $(SOURCE_DIR)/texture.h $(SOURCE_DIR)/element.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/texture.cpp -o $(OBJ_DIR)/texture.o

$(OBJ_DIR)/app.o: $(SOURCE_DIR)/app.cpp $(SOURCE_DIR)/app.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/app.cpp -o $(OBJ_DIR)/app.o

$(OBJ_DIR)/spriteapp.o: $(SOURCE_DIR)/spriteapp.cpp $(SOURCE_DIR)/spriteapp.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/spriteapp.cpp -o $(OBJ_DIR)/spriteapp.o

$(OBJ_DIR)/bank.o: $(SOURCE_DIR)/bank.cpp $(SOURCE_DIR)/bank.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/bank.cpp -o $(OBJ_DIR)/bank.o

$(OBJ_DIR)/macbank.o: $(SOURCE_DIR)/macbank.cpp $(SOURCE_DIR)/macbank.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/macbank.cpp -o $(OBJ_DIR)/macbank.o

$(OBJ_DIR)/parse.o: $(SOURCE_DIR)/parse.cpp $(SOURCE_DIR)/parse.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/parse.cpp -o $(OBJ_DIR)/parse.o

$(OBJ_DIR)/serializable.o: $(SOURCE_DIR)/serializable.cpp $(SOURCE_DIR)/serializable.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/serializable.cpp -o $(OBJ_DIR)/serializable.o

$(OBJ_DIR)/sprites.o: $(SOURCE_DIR)/sprites.cpp $(SOURCE_DIR)/sprites.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/sprites.cpp -o $(OBJ_DIR)/sprites.o

$(OBJ_DIR)/animations.o: $(SOURCE_DIR)/animations.cpp $(SOURCE_DIR)/animations.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/animations.cpp -o $(OBJ_DIR)/animations.o

clean:
	-rm $(OBJ_DIR)/*.o

