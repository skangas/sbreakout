#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>

#include <sigc++/sigc++.h>

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
  bool is_clear();
  void set_level(vector<game_brick> new_bricks);
  void set_paddle(int x);
  void set_surface(SDL_Surface* surface);
  void update(int ticks);

  vector<game_ball> balls;
  vector<game_brick> bricks;
  game_paddle paddle;

  void handle_event_mouse_left();

  sigc::signal<void> signal_death;
  sigc::signal<void, int> signal_brick_destroyed;

private:
  void handle_brick_collision(game_ball& ball, int new_x, int new_y);
  void handle_paddle_collision(game_ball& ball, int new_x, int new_y);
  void handle_wall_collision(game_ball& ball, int new_x, int new_y);

  sprites graphics;

  int last_update;
  bool paused;

  int paddle_x;
  int paddle_y;
  SDL_Surface* surface;
};

#endif
