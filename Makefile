SRC_DIRS = ColorBoxes
VPATH = ${SRC_DIRS}
OBJS =	GameEngine.o \
	ColorBoxesEngine.o \
	Shape.o \
	Polygon.o \
	Circle.o \
	Box.o \
	GLColor.o \
	OpenGLDraw.o \
	Cursors.o \
	Utilities.o \
	Wall.o \
	Edge.o \
	main.o
OUTPUT = colorboxes

CXX = g++
CXXFLAGS = -ggdb -D_GNU_SOURCE=1 -D_REENTRANT -IColorBoxes -I/usr/include
LDFLAGS = -L/usr/lib -l SDL_ttf -lSDL -lpthread -lGL -lGLU -lglut -lBox2D -lAntTweakBar

${OUTPUT}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${OUTPUT}

clean:
	rm -f ${OUTPUT} ${OBJS}
