#include <cmath>
#include <iostream>

#include "game_lib.hpp"
#include "sprites.hpp"

bool
collision_detection(game_ball* ball, const SDL_Rect* rect, int& coll_x, int& coll_y)
{
  int new_x = ball->get_new_x();
  int new_y = ball->get_new_y();

  int left   = rect->x;
  int top    = rect->y;
  int right  = rect->x + rect->w;
  int bottom = rect->y + rect->h; 
  
  // Find colliding x coordinate
  if (new_x < left)
    coll_x = left;
  else if (new_x > right)
    coll_x = right;
  else
    coll_x = new_x;

  // Find colliding y coordinate
  if (new_y < top)
    coll_y = top;
  else if (new_y > bottom)
    coll_y = bottom;
  else
    coll_y = new_y;

  // No collision if the ball is too far from rectangle
  int dist = distance(new_x, new_y, coll_x, coll_y);
  if (dist > BALL_RADIUS)
    return false;

  return true;
}

/**
 * Calculate distance between two points
 */
float
distance(int x1, int y1, int x2, int y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void
debug_print(const char* function, const char* format, ...)
{
  char buffer[1024];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  printf("%s: ", function);
  printf("%s\n", buffer);
  va_end(args);
}

SDL_Surface *load_image(std::string filename)
{
  SDL_Surface* loaded = NULL;
  SDL_Surface* optimized = NULL;

  loaded = IMG_Load(filename.c_str());

  if (loaded == NULL)
    return NULL;

  optimized = SDL_DisplayFormat(loaded);
  SDL_FreeSurface(loaded);

  return optimized;
}

SDL_Surface *load_image(std::string filename, int r, int g, int b)
{
  SDL_Surface* loaded = NULL;
  SDL_Surface* optimized = NULL;

  loaded = IMG_Load(filename.c_str());

  if (loaded == NULL)
    return NULL;

  if (r != -1)
    {
      Uint32 colorkey = SDL_MapRGB(loaded->format, r, g, b);
      SDL_SetColorKey(loaded, SDL_SRCCOLORKEY, colorkey);
    }

  optimized = SDL_DisplayFormat(loaded);
  SDL_FreeSurface(loaded);

  return optimized;
}
