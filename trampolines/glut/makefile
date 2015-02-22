
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

OBJ_DIR = obj

CURRENT = libgluttrampoline.a
current: $(CURRENT)

G2C_ROOT = ../../
G2C_SOURCE_DIR = $(G2C_ROOT)/g2c

include $(G2C_SOURCE_DIR)/platform.mk

OFILES = \
        $(OBJ_DIR)/glut.o \
	$(OBJ_DIR)/gluttrampoline.o \
        $(OBJ_DIR)/glutlistener.o


$(OBJ_DIR)/glutlistener.o: glutlistener.cpp glutlistener.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c glutlistener.cpp -o $(OBJ_DIR)/glutlistener.o

$(OBJ_DIR)/glut.o: glut.cpp glut.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c glut.cpp -o $(OBJ_DIR)/glut.o

$(OBJ_DIR)/gluttrampoline.o: gluttrampoline.cpp gluttrampoline.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c gluttrampoline.cpp -o $(OBJ_DIR)/gluttrampoline.o

libgluttrampoline.a: $(OFILES)
	ar rcs libgluttrampoline.a $(OFILES)


clean:
	-rm -f obj/*
	-rm -f libgluttrampoline.a

