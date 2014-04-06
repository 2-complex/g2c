
OPTIMIZATION = -O0
DEBUG = -g3

G2C_ROOT = ..
GLUT_INCLUDES = 
GLUT_LIBS = -lglut -lGL -lGLU

environment.o: environment.h environment.cpp
	c++ -DGLUT -I$(G2C_ROOT)/g2c environment.cpp -c $(GLUT_LIBS)

appenvironment.o: appenvironment.h appenvironment.cpp
	c++ -DGLUT -I$(G2C_ROOT)/g2c appenvironment.cpp -c $(GLUT_LIBS)


