#ifndef SPRITES_HPP
#define SPRITES_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <vector>

using std::vector;

class sprites
{
public:
  sprites();
  ~sprites();

  void blit_sprite(int type, int index, int x, int y);
  void load_sprites();
  void set_surface(SDL_Surface* surface);

  enum type { BALL, BRICK, BONUS, PADDLE };
private:
  SDL_Surface* surface;
  SDL_Surface* charts[4];
};

#endif
  
