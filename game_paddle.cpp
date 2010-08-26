#include "game_paddle.hpp"

game_paddle::game_paddle()
  : x(0),
    y(0)
{
}

game_paddle::~game_paddle()
{
}

void
game_paddle::blit(SDL_Surface* surface)
{
  
}

void
game_paddle::set_x(int x)
{
  this->x = x;
}
