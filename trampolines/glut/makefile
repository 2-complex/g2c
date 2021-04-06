
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

OBJ_DIR = obj

GLFLAGS = -DGL_SILENCE_DEPRECATION -Wno-deprecated-declarations
ALFLAGS = -DAL_SILENCE_DEPRECATION -Wno-deprecated-declarations

CURRENT = libgluttrampoline.a
current: $(CURRENT)

CELLO_ROOT = ../../
CELLO_ENGINE_DIR = $(CELLO_ROOT)/engine

include $(CELLO_ENGINE_DIR)/platform.mk

OFILES = \
	$(OBJ_DIR)/glut.o \
	$(OBJ_DIR)/gluttrampoline.o \
	$(OBJ_DIR)/glutlistener.o


$(OBJ_DIR)/glutlistener.o: glutlistener.cpp glutlistener.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(CELLO_ENGINE_DIR) \
		-c glutlistener.cpp -o $(OBJ_DIR)/glutlistener.o

$(OBJ_DIR)/glut.o: glut.cpp glut.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(CELLO_ENGINE_DIR) \
		-c glut.cpp -o $(OBJ_DIR)/glut.o

$(OBJ_DIR)/gluttrampoline.o: gluttrampoline.cpp gluttrampoline.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(GLFLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(CELLO_ENGINE_DIR) \
		-c gluttrampoline.cpp -o $(OBJ_DIR)/gluttrampoline.o

libgluttrampoline.a: $(OFILES)
	ar rcs libgluttrampoline.a $(OFILES)


clean:
	-rm -f obj/*
	-rm -f libgluttrampoline.a
