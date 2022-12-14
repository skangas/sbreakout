#ifndef SPRITES_HPP
#define SPRITES_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <vector>

using std::vector;

const int BALL_WIDTH = 12;
const int BALL_HEIGHT = 12;
const int BALL_RADIUS = BALL_WIDTH / 2;

const int BRICK_WIDTH = 40;
const int BRICK_HEIGHT = 20;

const int PADDLE_WIDTH = 53;
const int PADDLE_HEIGHT = 17;

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
  
