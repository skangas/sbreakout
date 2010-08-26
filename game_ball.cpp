#include <cmath>

#include <iostream>

#include "game_ball.hpp"
#include "game_lib.hpp"

using std::cout;
using std::endl;

/**
 * "Stupid" ball class.  Will only calculate the course and position of a ball,
 * never do anything fancy like collision detection etc.
 */

game_ball::game_ball(int x, int y, struct vect direction, float speed)
  : x(x),
    y(y),
    direction(direction),
    speed(speed)
{
  this->direction.normalize();
}

game_ball::~game_ball()
{
}

void
game_ball::bounce_against_bottom()
{
  LOG("enter");
  vect top = vect(1, 0);
  direction.bounce_against(top);
}

void
game_ball::bounce_against_left()
{
  LOG("enter");
  vect top = vect(0, 1);
  direction.bounce_against(top);
}

void
game_ball::bounce_against_right()
{
  LOG("enter");
  vect top = vect(0, -1);
  direction.bounce_against(top);
}

void
game_ball::bounce_against_top()
{
  LOG("enter");
  vect top = vect(-1, 0);
  direction.bounce_against(top);
}

/**
 * Moves the ball to new, assumed free, position.
 */
void
game_ball::move(int ticks)
{
  x = get_new_x(ticks);
  y = get_new_y(ticks);
}

/**
 * Assume there is nothing in the way, and calculate the balls new position.
 * @return the position the ball will be in after specified number of ticks
 */
float
game_ball::get_new_x(int ticks)
{
  float pixels = BASE_PIXEL_PER_SECOND * ticks / 1000.0;
  return x + direction.x * pixels * speed;
}

/**
 * Assume there is nothing in the way, and calculate the balls new position.
 * @return the position the ball will be in after specified number of ticks
 */
float
game_ball::get_new_y(int ticks)
{
  float pixels = BASE_PIXEL_PER_SECOND * ticks / 1000.0;
  return y + direction.y * pixels * speed;
}
