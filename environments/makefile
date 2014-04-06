
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

OBJ_DIR = obj
ENVIRONMENTS_DIR = .

CURRENT = libenvironments.a
current: $(CURRENT)

G2C_ROOT = ..
G2C_SOURCE_DIR = $(G2C_ROOT)/g2c

include $(G2C_SOURCE_DIR)/platform.mk

OFILES = \
	$(OBJ_DIR)/environment.o \
	$(OBJ_DIR)/spriteenvironment.o \
	$(OBJ_DIR)/panenvironment.o \
	$(OBJ_DIR)/scrollenvironment.o \
	$(OBJ_DIR)/appenvironment.o


$(OBJ_DIR)/environment.o: environment.cpp environment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c environment.cpp -o $(OBJ_DIR)/environment.o

$(OBJ_DIR)/spriteenvironment.o: spriteenvironment.cpp spriteenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c spriteenvironment.cpp -o $(OBJ_DIR)/spriteenvironment.o

$(OBJ_DIR)/panenvironment.o: panenvironment.cpp panenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c panenvironment.cpp -o $(OBJ_DIR)/panenvironment.o

$(OBJ_DIR)/scrollenvironment.o: scrollenvironment.cpp scrollenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c scrollenvironment.cpp -o $(OBJ_DIR)/scrollenvironment.o

$(OBJ_DIR)/appenvironment.o: appenvironment.cpp appenvironment.h
	@mkdir -p $(@D)
	c++ $(FLAGS) $(ODEBUG) $(OPTIMIZATION) \
		-I$(G2C_SOURCE_DIR) \
		-c appenvironment.cpp -o $(OBJ_DIR)/appenvironment.o

libenvironments.a: $(OFILES)
	ar rcs libenvironments.a $(OFILES)


clean:
	-rm -f obj/*
	-rm -f libenvironments.a
