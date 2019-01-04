SRC_DIRS = ColorBoxes
VPATH = ${SRC_DIRS}
OBJS =	GameEngine.o \
	ColorBoxesEngine.o \
	PhysicsEntity.o \
	Polygon.o \
	Circle.o \
	Box.o \
	Bomb.o \
	ExplosionCallback.o \
	GLColor.o \
	OpenGLDraw.o \
	Cursors.o \
	Utilities.o \
	Wall.o \
	Edge.o \
	main.o
OUTPUT = colorboxes

CXX = g++
CXXFLAGS = -std=c++11 -ggdb -D_GNU_SOURCE=1 -D_REENTRANT -IColorBoxes -I/usr/include -I/usr/local/include
LDFLAGS = -L/usr/lib -L/usr/lib/x86_64-linux-gnu -L/usr/local/lib -lSDL -lSDL_ttf -lpthread -lGL -lGLU -lglut -lBox2D -lAntTweakBar

${OUTPUT}: ${OBJS}
	${CXX} ${OBJS} ${LDFLAGS} -o ${OUTPUT}

clean:
	rm -f ${OUTPUT} ${OBJS}
