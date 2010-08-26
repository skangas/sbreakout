#ifndef GAME_PADDLE_HPP
#define GAME_PADDLE_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class game_paddle
{
public:
  game_paddle();
  ~game_paddle();

  void blit(SDL_Surface* surface);
  void set_x(int x);

private:
  int x;
  int y;
};

#endif

