#ifndef GAME_BALL_HPP
#define GAME_BALL_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "game_lib.hpp"
#include "vect.hpp"

class game_ball
{
public:
  game_ball(int x, int y, vect direction, float speed);
  ~game_ball();

  float x;
  float y;
  vect direction;
  float speed;

  void bounce_against_bottom();
  void bounce_against_left();
  void bounce_against_right();
  void bounce_against_top();

  void move(int ticks);
  float get_new_x(int ticks);
  float get_new_y(int ticks);

private:

  void ensure_minimum_rotation();

  void turn_left();
  void turn_right();
};

#endif
