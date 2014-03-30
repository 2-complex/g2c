
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_DIRECTORY = ..

SOURCE_DIR = $(G2C_DIRECTORY)/g2c
OBJ_DIR = obj

CURRENT = libg2c.a

current: $(CURRENT)

run: current
	./$(CURRENT) $(CURRENTARGS)

FLAGS = -DGLUT
#GLFLAGS = -framework GLUT -framework OpenGL
#GRAPHICSFLAGS = -framework Cocoa -framework QuartzCore
#AUDIOFLAGS = -framework AudioToolbox -framework Foundation -framework OpenAL

GLFLAGS += -L/usr/lib -lglut -lGLU -lGL
GLINCLUDES += /usr/include ../external

#PLATFORM_BANK = $(OBJ_DIR)/macbank.o
PLATFORM_BANK = $(OBJ_DIR)/unixbank.o

GRAPHICS = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/bank.o $(PLATFORM_BANK) $(OBJ_DIR)/graphics.o
SPRITES = $(OBJ_DIR)/parse.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/player.o $(OBJ_DIR)/wave.o $(OBJ_DIR)/bank.o $(PLATFORM_BANK) $(OBJ_DIR)/sprites.o $(OBJ_DIR)/animations.o
GAMES = $(SPRITES) $(OBJ_DIR)/graphics.o $(OBJ_DIR)/transforms.o

APPS = $(OBJ_DIR)/app.o

libg2c.a: $(GAMES) $(APPS)
	ar rcs libg2c.a $(OBJ_DIR)/listener.o $(OBJ_DIR)/util.o $(OBJ_DIR)/serializable.o $(OBJ_DIR)/parse.o $(OBJ_DIR)/sprites.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/player.o $(OBJ_DIR)/bank.o $(OBJ_DIR)/unixbank.o $(OBJ_DIR)/texture.o $(OBJ_DIR)/wave.o


$(OBJ_DIR)/listener.o: $(SOURCE_DIR)/listener.cpp $(SOURCE_DIR)/listener.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -I$(SOURCE_DIR) -c $(SOURCE_DIR)/listener.cpp -o $(OBJ_DIR)/listener.o -o $(OBJ_DIR)/listener.o


$(OBJ_DIR)/util.o: $(SOURCE_DIR)/util.cpp $(SOURCE_DIR)/util.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/util.cpp -o $(OBJ_DIR)/util.o

$(OBJ_DIR)/sound.o: $(SOURCE_DIR)/sound.cpp $(SOURCE_DIR)/sound.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/sound.cpp -o $(OBJ_DIR)/sound.o

$(OBJ_DIR)/player.o: $(SOURCE_DIR)/player.cpp $(SOURCE_DIR)/player.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/player.cpp -o $(OBJ_DIR)/player.o

$(OBJ_DIR)/openalplayer.o: $(SOURCE_DIR)/openalplayer.cpp $(SOURCE_DIR)/openalplayer.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/openalplayer.cpp -o $(OBJ_DIR)/openalplayer.o

$(OBJ_DIR)/wave.o: $(SOURCE_DIR)/wave.cpp $(SOURCE_DIR)/wave.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/wave.cpp -o $(OBJ_DIR)/wave.o

$(OBJ_DIR)/rendertarget.o: $(SOURCE_DIR)/rendertarget.cpp $(SOURCE_DIR)/rendertarget.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/rendertarget.cpp -o $(OBJ_DIR)/rendertarget.o

$(OBJ_DIR)/transforms.o: $(SOURCE_DIR)/transforms.cpp $(SOURCE_DIR)/transforms.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/transforms.cpp -o $(OBJ_DIR)/transforms.o

$(OBJ_DIR)/graphics.o: $(SOURCE_DIR)/graphics.cpp $(SOURCE_DIR)/graphics.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c graphics.cpp -o $(OBJ_DIR)/graphics.o

$(OBJ_DIR)/texture.o: $(SOURCE_DIR)/texture.cpp $(SOURCE_DIR)/texture.h $(SOURCE_DIR)/element.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/texture.cpp -o $(OBJ_DIR)/texture.o

$(OBJ_DIR)/app.o: $(SOURCE_DIR)/app.cpp $(SOURCE_DIR)/app.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/app.cpp -o $(OBJ_DIR)/app.o

$(OBJ_DIR)/bank.o: $(SOURCE_DIR)/bank.cpp $(SOURCE_DIR)/bank.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/bank.cpp -o $(OBJ_DIR)/bank.o

$(OBJ_DIR)/macbank.o: $(SOURCE_DIR)/macbank.cpp $(SOURCE_DIR)/macbank.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/macbank.cpp -o $(OBJ_DIR)/macbank.o

$(OBJ_DIR)/unixbank.o: $(SOURCE_DIR)/unixbank.cpp $(SOURCE_DIR)/unixbank.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/unixbank.cpp -o $(OBJ_DIR)/unixbank.o

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

