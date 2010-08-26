#include <cmath>
#include <iostream>

#include "game_lib.hpp"

/**
 * Calculate distance between two
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

  loaded = IMG_Load( filename.c_str() );

  if( loaded != NULL )
    {
      optimized = SDL_DisplayFormat(loaded);
      SDL_FreeSurface(loaded);

      if (optimized != NULL)
        {
          Uint32 colorkey = SDL_MapRGB( optimized->format, 0, 0x00, 0x00 );
          SDL_SetColorKey(optimized, SDL_SRCCOLORKEY, colorkey);
        }
    }
  return optimized;
}
