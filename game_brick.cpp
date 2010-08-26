#include "game_brick.hpp"

game_brick::game_brick(int x, int y, int type, int life)
  : x(x),
    y(y),
    type(type),
    life(life)
{ }

game_brick::~game_brick() { }

int
game_brick::get_type() const
{
  return type;
}

int
game_brick::get_x() const
{
  return x;
}

int
game_brick::get_y() const
{
  return y;
}

void
game_brick::hit()
{
  life--;
}

bool
game_brick::in_play() const
{
  return life > 0;
}
