OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

GLFLAGS = -DGL_SILENCE_DEPRECATION -Wno-deprecated-declarations
ALFLAGS = -DAL_SILENCE_DEPRECATION -Wno-deprecated-declarations
CC = c++

CELLO_ROOT_DIRECTORY = ..

SOURCE_DIR = $(CELLO_ROOT_DIRECTORY)/engine
OBJ_DIR = obj

CURRENT = libcello.a
current: $(CURRENT)

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	PLATFORM_BANK = $(OBJ_DIR)/macbank.o
endif

ifeq ($(UNAME), Linux)
	PLATFORM_BANK = $(OBJ_DIR)/unixbank.o
endif


OFILES = \
	$(OBJ_DIR)/util.o \
	$(OBJ_DIR)/listener.o \
	$(OBJ_DIR)/app.o \
	$(OBJ_DIR)/parse.o \
	$(OBJ_DIR)/serializable.o \
	$(OBJ_DIR)/sound.o \
	$(OBJ_DIR)/audioplayer.o \
	$(OBJ_DIR)/bank.o \
	$(OBJ_DIR)/texture.o \
	$(OBJ_DIR)/sprites.o \
	$(OBJ_DIR)/fattening.o \
	$(OBJ_DIR)/rendererg.o \
	$(OBJ_DIR)/transforms.o \
	$(OBJ_DIR)/graphics.o \
	$(OBJ_DIR)/wave.o \
	$(PLATFORM_BANK) \
	$(OBJ_DIR)/openalplayer.o

libcello.a: $(OFILES)
	ar rcs libcello.a $(OFILES)


$(OBJ_DIR)/listener.o: $(SOURCE_DIR)/listener.cpp $(SOURCE_DIR)/listener.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/listener.cpp -o $(OBJ_DIR)/listener.o

$(OBJ_DIR)/util.o: $(SOURCE_DIR)/util.cpp $(SOURCE_DIR)/util.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/util.cpp -o $(OBJ_DIR)/util.o

$(OBJ_DIR)/sound.o: $(SOURCE_DIR)/sound.cpp $(SOURCE_DIR)/sound.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/sound.cpp -o $(OBJ_DIR)/sound.o

$(OBJ_DIR)/audioplayer.o: $(SOURCE_DIR)/audioplayer.cpp $(SOURCE_DIR)/audioplayer.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/audioplayer.cpp -o $(OBJ_DIR)/audioplayer.o

$(OBJ_DIR)/openalplayer.o: $(SOURCE_DIR)/openalplayer.cpp $(SOURCE_DIR)/openalplayer.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ALFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/openalplayer.cpp -o $(OBJ_DIR)/openalplayer.o

$(OBJ_DIR)/wave.o: $(SOURCE_DIR)/wave.cpp $(SOURCE_DIR)/wave.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/wave.cpp -o $(OBJ_DIR)/wave.o

$(OBJ_DIR)/rendertarget.o: $(SOURCE_DIR)/rendertarget.cpp $(SOURCE_DIR)/rendertarget.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/rendertarget.cpp -o $(OBJ_DIR)/rendertarget.o

$(OBJ_DIR)/transforms.o: $(SOURCE_DIR)/transforms.cpp $(SOURCE_DIR)/transforms.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/transforms.cpp -o $(OBJ_DIR)/transforms.o

$(OBJ_DIR)/graphics.o: $(SOURCE_DIR)/graphics.cpp $(SOURCE_DIR)/graphics.h $(SOURCE_DIR)/lin/lin.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/graphics.cpp -o $(OBJ_DIR)/graphics.o

$(OBJ_DIR)/texture.o: $(SOURCE_DIR)/texture.cpp $(SOURCE_DIR)/texture.h $(SOURCE_DIR)/element.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/texture.cpp -o $(OBJ_DIR)/texture.o

$(OBJ_DIR)/app.o: $(SOURCE_DIR)/app.cpp $(SOURCE_DIR)/app.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/app.cpp -o $(OBJ_DIR)/app.o

$(OBJ_DIR)/bank.o: $(SOURCE_DIR)/bank.cpp $(SOURCE_DIR)/bank.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/bank.cpp -o $(OBJ_DIR)/bank.o

$(OBJ_DIR)/macbank.o: $(SOURCE_DIR)/macbank.cpp $(SOURCE_DIR)/macbank.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ALFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/macbank.cpp -o $(OBJ_DIR)/macbank.o

$(OBJ_DIR)/unixbank.o: $(SOURCE_DIR)/unixbank.cpp $(SOURCE_DIR)/unixbank.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/unixbank.cpp -o $(OBJ_DIR)/unixbank.o

$(OBJ_DIR)/parse.o: $(SOURCE_DIR)/parse.cpp $(SOURCE_DIR)/parse.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/parse.cpp -o $(OBJ_DIR)/parse.o

$(OBJ_DIR)/serializable.o: $(SOURCE_DIR)/serializable.cpp $(SOURCE_DIR)/serializable.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/serializable.cpp -o $(OBJ_DIR)/serializable.o

$(OBJ_DIR)/sprites.o: $(SOURCE_DIR)/sprites.cpp $(SOURCE_DIR)/sprites.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/sprites.cpp -o $(OBJ_DIR)/sprites.o

$(OBJ_DIR)/rendererg.o: $(SOURCE_DIR)/rendererg.cpp $(SOURCE_DIR)/rendererg.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/rendererg.cpp -o $(OBJ_DIR)/rendererg.o

$(OBJ_DIR)/fattening.o: $(SOURCE_DIR)/fattening.cpp $(SOURCE_DIR)/fattening.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/fattening.cpp -o $(OBJ_DIR)/fattening.o

$(OBJ_DIR)/animations.o: $(SOURCE_DIR)/animations.cpp $(SOURCE_DIR)/animations.h
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(ODEBUG) $(OPTIMIZATION) -c $(SOURCE_DIR)/animations.cpp -o $(OBJ_DIR)/animations.o

clean:
	-rm -f $(OBJ_DIR)/*.o
	-rm -f libcello.a


