CFLAGS=-Wall -Wextra -Wconversion -Wshadow -Wcast-qual -Wwrite-strings

all:
	g++ -O2 $(CFLAGS) \
	-o breakout \
	src/game.cpp \
	src/game_ball.cpp \
	src/game_board.cpp \
	src/game_brick.cpp \
	src/game_lib.cpp \
	src/game_paddle.cpp \
	src/game_rules.cpp \
	src/level_loader.cpp \
	src/main.cpp \
	src/sprites.cpp \
	src/timer.cpp \
	src/vect.cpp \
	-lSDL -lSDL_image -lSDL_ttf \
	$(shell pkg-config --cflags --libs sigc++-2.0)

clean:
	rm -rf *.o breakout

