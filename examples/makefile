
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
	bitmap \
	quad \
	serialize \
	serializemore \
	ship \
	sound \
	sprite \
	star \
	steam \
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
		$(1) -o $(2) \
		$(LIBENVIRONMENTS) \
		$(LIBG2C) \
		$(GRAPHICS_LIBRARIES) $(AUDIO_LIBRARIES)
endef

DEPS := $(LIBG2C) $(LIBENVIRONMENTS) $(G2C_SOURCE_DIR)/lin/lin.h

atlas: $(DEPS) atlas.cpp
	$(call build_with_source, launch.o atlas.cpp, atlas)

modelapp: $(DEPS) modelapp.cpp
	$(call build_with_source, launch.o modelapp.cpp, modelapp)

quad: $(DEPS) launch.o quad.cpp
	$(call build_with_source, launch.o quad.cpp, quad)

serialize: $(DEPS) serialize.cpp
	$(call build_with_source, serialize.cpp, serialize)

ship: $(DEPS) launch.o ship.cpp
	$(call build_with_source, launch.o ship.cpp, ship)

bitmap: $(DEPS) launch.o bitmap.cpp
	$(call build_with_source, launch.o bitmap.cpp, bitmap)

sound: $(DEPS) sound.cpp
	$(call build_with_source, launch.o sound.cpp, sound)

sprite: $(DEPS) launch.o sprite.cpp
	$(call build_with_source, launch.o sprite.cpp, sprite)

button: $(DEPS) launch.o button.cpp
	$(call build_with_source, launch.o button.cpp, button)

star: $(DEPS) launch.o star.cpp
	$(call build_with_source, launch.o star.cpp, star)

tetrahedron: $(DEPS) tetrahedron.cpp
	$(call build_with_source, launch.o tetrahedron.cpp, tetrahedron)

steam: $(DEPS) launch.o steam.cpp
	$(call build_with_source, launch.o steam.cpp, steam)

text: $(DEPS) text.cpp
	$(call build_with_source, launch.o text.cpp, text)

triangle: $(DEPS) launch.o triangle.cpp
	$(call build_with_source, launch.o triangle.cpp, triangle)

worldapp: $(DEPS) launch.o worldapp.cpp
	$(call build_with_source, launch.o worldapp.cpp, worldapp)


launch.o: $(launch) DEPS.h launch.cpp
	c++  $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(ENVIRONMENTS_DIR) \
		launch.cpp -c


clean:
	cd $(G2C_SOURCE_DIR) && $(MAKE) clean
	cd $(ENVIRONMENTS_DIR) && $(MAKE) clean
	-rm -f $(EXES)
	-rm -f $(OBJ_DIR)/*.o
	-rm -rf *.dSYM

