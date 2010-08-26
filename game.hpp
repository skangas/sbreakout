#ifndef GAME_HPP
#define GAME_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "game_board.hpp"
#include "level_loader.hpp"

class game {
public:
  game();
  ~game();

  bool init();
  void run();
  bool quit();
  
private:
  bool running;

  game_board board;
  int paddle_location;

  int frame;
  int frame_start;

  static const int FRAMES_PER_SECOND = 100;
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_BPP = 32;

  SDL_Surface* screen;
  SDL_Surface* game_area;
  SDL_Surface* status;

  level_loader loader;

  void blit_paddle();
  void handle_events();
};

#endif
