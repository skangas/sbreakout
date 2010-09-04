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
    speed(speed),
    ticks(0)
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
  vect edge = vect(1, 0);
  direction.bounce_against(edge);
}

void
game_ball::bounce_against_left()
{
  LOG("enter");
  vect edge = vect(0, 1);
  direction.bounce_against(edge);
}

void
game_ball::bounce_against_right()
{
  LOG("enter");
  vect edge = vect(0, -1);
  direction.bounce_against(edge);
}

void
game_ball::bounce_against_top()
{
  LOG("enter");
  vect edge = vect(-1, 0);
  direction.bounce_against(edge);
}

/**
 * Bounce the ball on "best" edge of specified rect.
 * @return 0-5 depending on which edge, in this order: top - right - bottom - left - none
 */
void
game_ball::bounce_on_rect(int ex, int ey, int ew, int eh)
{
  // Decide which edge to bounce against.
  int left_of  = ex - x;
  int right_of = x - (ex + ew);
  int above    = ey - y;
  int below    = y - (ey + eh);

  // The numbering below corresponds to CSS ordering of attributes, heh
  if (left_of > 0 && left_of > below && left_of > above)
      bounce_against_right();
  else if (right_of > 0 && right_of > below && right_of > above)
      bounce_against_left();
  else if (above > 0)
      bounce_against_bottom();
  else if (below > 0)
      bounce_against_top();
  else
    ; // INSIDE -- do not bounce for now
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

float
game_ball::get_new_x()
{
  return get_new_x(ticks);
}

/**
 * Assume there is nothing in the way, and calculate the balls new position.
 * @return the position the ball will be in after specified number of ticks
 */
float
game_ball::get_new_x(int ticks)
{
  this->ticks = ticks;
  float pixels = BASE_PIXEL_PER_SECOND * ticks / 1000.0;
  return x + direction.x * pixels * speed;
}

float
game_ball::get_new_y()
{
  return get_new_y(ticks);
}

/**
 * Assume there is nothing in the way, and calculate the balls new position.
 * @return the position the ball will be in after specified number of ticks
 */
float
game_ball::get_new_y(int ticks)
{
  this->ticks = ticks;
  float pixels = BASE_PIXEL_PER_SECOND * ticks / 1000.0;
  return y + direction.y * pixels * speed;
}

void
game_ball::set_ticks(int ticks)
{
  this->ticks = ticks;
}
