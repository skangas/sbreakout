#ifndef TIMER_HPP
#define TIMER_HPP

#include "SDL/SDL.h"

/**
 * Simple timer class.
 */
class timer
{
public:
  timer();

  Uint32 get_ticks();
  bool is_running();
  void start();
  void toggle_start_pause();


private:
  Uint32 start_time;
  Uint32 last_read;

  bool running;
  int ticks;
};


#endif // TIMER_HPP
