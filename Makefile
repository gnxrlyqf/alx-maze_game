CFLAGS =  -Wall -Werror -Wextra -pedantic
LIBS = -lSDL2 -lSDL2_image -lm -lpng
WLIBS = -I dependencies/SDL2-2.30.3/i686-w64-mingw32/include/SDL2 \
-I dependencies/SDL2-2.30.3/i686-w64-mingw32/include \
-L dependencies/SDL2-2.30.3/i686-w64-mingw32/lib \
-I dependencies/SDL2_image-2.6.0/i686-w64-mingw32/include \
-L dependencies/SDL2_image-2.6.0/i686-w64-mingw32/lib \
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

build:
	gcc ./src/files/* -o game ${LIBS} ${CFLAGS};

build-w:
	i686-w64-mingw32-gcc ./src/files/* -o game.exe ${WLIBS} ${CFLAGS};

run:
	./game;

experimental:
	./game exp;

clean:
	rm game;