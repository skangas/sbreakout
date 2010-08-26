#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "game_ball.hpp"
#include "game_brick.hpp"
#include "game_lib.hpp"

struct brick {
  int x;
  int y;
};

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

private:
  void blit_balls();
  void blit_bricks();
  void blit_paddle();

  void handle_brick_collision(game_ball& ball, int new_x, int new_y);
  void handle_paddle_collision(game_ball& ball, int new_x, int new_y);
  void handle_wall_collision(game_ball& ball, int new_x, int new_y);

  // void check_block_collision(game_ball* ball, const SDL_Rect &block);

  int last_update;

  vector<game_ball> balls;
  vector<game_brick> bricks;

  int paddle_x;
  int paddle_y;
  SDL_Surface* surface;

  // SDL_Pictures
  SDL_Surface* ball_sprite;
  SDL_Surface* brick_sprite;
  SDL_Surface* paddle_sprite;

  // Default ball speed
  static const int BALL_WIDTH = 16;
  static const int BALL_HEIGHT = 16;
  static const int BRICK_WIDTH = 48;
  static const int BRICK_HEIGHT = 16;
  static const int BALL_RADIUS = BALL_WIDTH / 2;
  static const int PADDLE_WIDTH = 65;
};

#endif
