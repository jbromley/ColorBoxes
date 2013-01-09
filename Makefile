SRC_DIRS = ColorBoxes
VPATH = ${SRC_DIRS}
OBJS =	GameEngine.o \
	ColorBoxesEngine.o \
	Box.o \
	GLColor.o \
	Utilities.o \
	Wall.o \
	Edge.o \
	main.o
OUTPUT = colorboxes

CXX = g++
CXXFLAGS = -ggdb -D_GNU_SOURCE=1 -D_REENTRANT -IColorBoxes -I/usr/include
LDFLAGS = -L/usr/lib -l SDL_ttf -lSDL -lpthread -lGL -lGLU -lBox2D

${OUTPUT}: ${OBJS}
	${CXX} ${LDFLAGS} ${OBJS} -o ${OUTPUT}

clean:
	rm -f ${OUTPUT} ${OBJS}
