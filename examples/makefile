
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ..
G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
ENVIRONMENTS_DIR = $(G2C_ROOT_DIR)/environments
OBJ_DIR = obj

include $(G2C_SOURCE_DIR)/platform.mk

EXES = \
	atlas \
	modelfile \
	serialize \
	ship \
	sound \
	sprite \
	tetrahedron \
	text \
	triangle \
	worldfile \
	worldapptest

all: $(EXES)


LIBENVIRONMENTS = $(ENVIRONMENTS_DIR)/libenvironments.a
LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

$(LIBENVIRONMENTS): force_look
	cd $(ENVIRONMENTS_DIR) && $(MAKE) libenvironments.a

force_look:
	true

define build_with_source
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		$(1) -o $(CURRENT) \
		$(LIBENVIRONMENTS) \
		$(LIBG2C) \
		$(GRAPHICS_LIBRARIES) $(AUDIO_LIBRARIES)
endef

DEPS := $(LIBG2C) $(LIBENVIRONMENTS) $(G2C_SOURCE_DIR)/lin/lin.h

atlas: $(DEPS) atlas.cpp
	$(call build_with_source, atlas.cpp)

modelfile: $(DEPS) modelfile.cpp
	$(call build_with_source, modelfile.cpp)

serialize: $(DEPS) serialize.cpp
	$(call build_with_source, serialize.cpp)

ship: $(DEPS) ship.cpp
	$(call build_with_source, ship.cpp)

sound: $(DEPS) sound.cpp
	$(call build_with_source, sound.cpp)

sprite: $(DEPS) sprite.cpp
	$(call build_with_source, sprite.cpp)

tetrahedron: $(DEPS) tetrahedron.cpp
	$(call build_with_source, tetrahedron.cpp)

text: $(DEPS) text.cpp
	$(call build_with_source, text.cpp)

triangle: $(DEPS) triangle.cpp
	$(call build_with_source, triangle.cpp)

worldfile: $(DEPS) worldfile.cpp
	$(call build_with_source, worldfile.cpp)

worldapptest: $(DEPS) worldapptest.cpp worldapp.cpp
	$(call build_with_source, worldapptest.cpp worldapp.cpp)


clean:
	-rm -f $(EXES)
	-rm -f $(OBJ_DIR)/*.o
	-rm -rf *.dSYM



