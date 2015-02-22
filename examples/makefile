
OPTIMIZATION = -O0
DEBUG = -g3
ODEBUG = $(DEBUG)

G2C_ROOT_DIR = ..
G2C_SOURCE_DIR = $(G2C_ROOT_DIR)/g2c
GLUT_TRAMPOLINE_DIR = $(G2C_ROOT_DIR)/trampolines/glut
OBJ_DIR = obj

include $(G2C_SOURCE_DIR)/platform.mk

EXES = \
	atlas \
	bitmap \
	quad \
	serialize \
	ship \
	sound \
	star \
	steam \
	tetrahedron \
	text \
	triangle \
	worldapp

all: $(EXES)


LIBGLUTTRAMPOLINE = $(GLUT_TRAMPOLINE_DIR)/libgluttrampoline.a
LIBG2C = $(G2C_SOURCE_DIR)/libg2c.a

$(LIBG2C): force_look
	cd $(G2C_SOURCE_DIR) && $(MAKE) libg2c.a

$(LIBGLUTTRAMPOLINE): force_look
	cd $(GLUT_TRAMPOLINE_DIR) && $(MAKE) libgluttrampoline.a

force_look:
	true

define build_with_source
	c++ $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(GLUT_TRAMPOLINE_DIR) \
		$(1) -o $(2) \
		$(LIBGLUTTRAMPOLINE) \
		$(LIBG2C) \
		$(GRAPHICS_LIBRARIES) $(AUDIO_LIBRARIES)
endef

DEPS := $(LIBG2C) $(LIBGLUTTRAMPOLINE) $(G2C_SOURCE_DIR)/lin/lin.h

atlas: $(DEPS) atlas.cpp
	$(call build_with_source, atlas.cpp, atlas)

modelapp: $(DEPS) modelapp.cpp
	$(call build_with_source, modelapp.cpp, modelapp)

quad: $(DEPS) quad.cpp
	$(call build_with_source, quad.cpp, quad)

serialize: $(DEPS) serialize.cpp
	$(call build_with_source, serialize.cpp, serialize)

ship: $(DEPS) ship.cpp
	$(call build_with_source, ship.cpp, ship)

bitmap: $(DEPS) bitmap.cpp
	$(call build_with_source, bitmap.cpp, bitmap)

sound: $(DEPS) sound.cpp
	$(call build_with_source, sound.cpp, sound)

button: $(DEPS) button.cpp
	$(call build_with_source, button.cpp, button)

star: $(DEPS) star.cpp
	$(call build_with_source, star.cpp, star)

tetrahedron: $(DEPS) tetrahedron.cpp
	$(call build_with_source, tetrahedron.cpp, tetrahedron)

steam: $(DEPS) steam.cpp
	$(call build_with_source, steam.cpp, steam)

text: $(DEPS) text.cpp
	$(call build_with_source, text.cpp, text)

triangle: $(DEPS) triangle.cpp
	$(call build_with_source, triangle.cpp, triangle)

worldapp: $(DEPS) worldapp.cpp
	$(call build_with_source, worldapp.cpp, worldapp)


launch.o: $(DEPS) launch.cpp
	c++  $(FLAGS) $(DEBUG) $(OPTIMIZATION) $(GLFLAGS) \
		-I$(G2C_SOURCE_DIR) -I$(GLUT_TRAMPOLINE_DIR) \
		launch.cpp -c


clean:
	cd $(G2C_SOURCE_DIR) && $(MAKE) clean
	cd $(GLUT_TRAMPOLINE_DIR) && $(MAKE) clean
	-rm -f $(EXES)
	-rm -f *.o
	-rm -f $(OBJ_DIR)/*.o
	-rm -rf *.dSYM

