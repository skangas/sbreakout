#ifndef GAME_LIB_HPP
#define GAME_LIB_HPP

#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "game_ball.hpp"
#include "vect.hpp"

#ifndef NDEBUG
#define LOG( args... ) debug_print(__FUNCTION__, args)
#else
#define LOG( args... ) ((void)(0))
#endif

const float PI = 3.14159265358979;

const int BASE_PIXEL_PER_SECOND = 250;

// int check_block_collision(const game_ball* ball, const SDL_Rect *block);
void debug_print(const char* function, const char* format, ...);
float distance(int x1, int y1, int x2, int y2);
SDL_Surface *load_image(std::string filename);
SDL_Surface *load_image(std::string filename, int r, int g, int b);



#endif
