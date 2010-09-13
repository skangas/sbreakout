#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "game_board.hpp"
#include "game_rules.hpp"
#include "level_loader.hpp"
#include "timer.hpp"

class game
{
public:
  game();
  ~game();

  bool init();
  void run();
  bool quit();
  
private:
  int show_big_message(int ticks);
  void handle_input_events();
  void maybe_next_level();
  void set_big_message(int ticks, std::string fmt, ...);

  int level;
  bool running;

  game_board board;
  game_rules rules;
  level_loader loader;
  timer clock;

  int frame;
  int frame_start;

  std::string big_message;
  int big_message_ticks;
  
  static const int LAST_LEVEL = 10;
  static const int FRAMES_PER_SECOND = 60; // This is what most LCDs can handle
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_BPP = 32;
  static const int GAME_AREA_X = SCREEN_WIDTH * 0.05;
  static const int GAME_AREA_Y = SCREEN_HEIGHT * 0.05;
  static const int GAME_AREA_W = SCREEN_WIDTH * 0.8;
  static const int GAME_AREA_H = SCREEN_HEIGHT * 0.9;
  
  SDL_Surface* screen;
  SDL_Surface* game_area;
  SDL_Surface* status;
};

#endif
