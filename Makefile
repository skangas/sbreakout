all : 
	g++ -O2 -lSDL -lSDL_image -lSDL_ttf \
	-o breakout \
	game.cpp \
	game_ball.cpp \
	game_board.cpp \
	game_brick.cpp \
	game_lib.cpp \
	game_paddle.cpp \
	level_loader.cpp \
	main.cpp \
	sprites.cpp \
	vect.cpp

clean :
	rm -rf *.o breakout

