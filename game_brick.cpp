#include "game_brick.hpp"

game_brick::game_brick(int x, int y, int type)
  : x(x),
    y(y),
    type(type)
{
  life = brick_lives[type];
}

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
  if (is_invincible())
    return;

  life--;

  // This brick might transform to another brick type on death
  const int& new_type = brick_transforms_to[type];
  if (life == 0 && new_type != -1)
    {
      type = new_type;
      life = brick_lives[type];
    }
}

bool
game_brick::in_play() const
{
  return life > 0 || is_invincible();
}

bool
game_brick::is_invincible() const
{
  return life == INVINCIBLE;
}

const int game_brick::brick_lives[] = {
  2,
  3,
  -1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
};

const int game_brick::brick_transforms_to[] = {
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
};
