#ifndef SPRITES_HPP
#define SPRITES_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class sprites
{
public:
  sprites(SDL_surface surface);
  ~sprites();

  void blit_sprite(int num, int x, int y);



private:
  enum {
    PADDLE;
  }
  // 19 bred, 14 hög

  // paddle
  

  static const SDL_Rect clip[2];

  SDL_Surface surface;
};

#endif
  
