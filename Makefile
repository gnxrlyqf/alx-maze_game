CFLAGS =  -Wall -Werror -Wextra -pedantic
LIBS = -lSDL2 -lSDL2_image -lm -lpng

build:
	gcc ./src/files/* -o game ${LIBS} ${CFLAGS};

build-w:
	i686-w64-mingw32-gcc ./src/files/* -o game ${LIBS} ${CFLAGS};

run:
	./game;

experimental:
	./game exp;

clean:
	rm game;