
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ..
G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
OBJ_DIR = obj

include $(G2C_SOURCE_DIR)/platform.mk

ifneq ($(MAKECMDGOALS), clean)
CURRENT = $(MAKECMDGOALS)
current: $(CURRENT)
endif

EXES = \
	modelfile \
	serialize \
	ship \
	sound \
	sprite \
	text \
	triangle \
	worldfile


LIBENVIRONMENTS = $(ENVIRONMENTS_DIR)/libenvironments.a
LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

$(LIBENVIRONMENTS): force_look
	cd $(ENVIRONMENTS_DIR) && $(MAKE) libenvironments.a

force_look:
	true

$(CURRENT): $(LIBG2C) $(LIBENVIRONMENTS) $(G2C_SOURCE_DIR)/lin/lin.h $(CURRENT).cpp
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(CURRENT).cpp -o $(CURRENT) \
		$(LIBENVIRONMENTS) \
		$(LIBG2C) \
		$(GRAPHICS_LIBRARIES) $(AUDIO_LIBRARIES)

clean:
	-rm -f $(EXES)
	-rm -f $(OBJ_DIR)/*.o
	-rm -rf *.dSYM

