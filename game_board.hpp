#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "game_ball.hpp"
#include "game_brick.hpp"
#include "game_paddle.hpp"
#include "game_lib.hpp"
#include "sprites.hpp"

using std::vector;

class game_board
{
public:
  game_board();
  ~game_board();

  void blit();
  void set_level(vector<game_brick> new_bricks);
  void set_paddle(int x);
  void set_surface(SDL_Surface* surface);
  void update();

  vector<game_ball> balls;
  vector<game_brick> bricks;
  game_paddle paddle;

  void handle_event_mouse_left();

private:
  void handle_brick_collision(game_ball& ball, int new_x, int new_y);
  void handle_paddle_collision(game_ball& ball, int new_x, int new_y);
  void handle_wall_collision(game_ball& ball, int new_x, int new_y);

  sprites graphics;

  int last_update;

  int paddle_x;
  int paddle_y;
  SDL_Surface* surface;

  // Default ball speed
  static const int BALL_WIDTH = 12;
  static const int BALL_HEIGHT = 12;
  static const int BALL_RADIUS = BALL_WIDTH / 2;
  static const int BRICK_WIDTH = 40;
  static const int BRICK_HEIGHT = 20;
  static const int PADDLE_WIDTH = 57;
};

#endif
