OBJECTS = main.o graphics.o machine.o screen_matrix.o spriteengine.o

INCLUDES = -I/opt/SDL/include
LIBRARIES = -lSDL2 -lrt

all: ${OBJECTS}
	gcc $^ -o demo ${LIBRARIES}

main.o: main.c
	gcc ${INCLUDES} -c $^ -o $@

graphics.o: graphics.c
	gcc ${INCLUDES} -c $^ -o $@

machine.o: machine.c
	gcc ${INCLUDES} -c $^ -o $@

screen_matrix.o: screen_matrix.c
	gcc ${INCLUDES} -c $^ -o $@

spriteengine.o: spriteengine.c
	gcc ${INCLUDES} -c $^ -o $@

clean:
	rm -rf demo ${OBJECTS}
