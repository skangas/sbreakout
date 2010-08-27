#include "game_paddle.hpp"

game_paddle::game_paddle()
  : x(0),
    y(0),
    ball(true)
{ }

game_paddle::~game_paddle() { }

void
game_paddle::add_ball()
{
  ball = true;
}

bool
game_paddle::has_ball()
{
  return ball;
}

void
game_paddle::release_ball()
{
  ball = false;
}
