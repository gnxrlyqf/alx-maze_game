CFLAGS = -lSDL2 -lSDL2_image -lm -lpng

build:
	gcc ./src/files/*.c ./src/files/main.h -o game ${CFLAGS};

run:
	./game;

clean:
	rm game;