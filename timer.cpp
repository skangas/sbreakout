#include "timer.hpp"

timer::timer()
  : running(false)
{
}

Uint32
timer::get_ticks()
{
  if (!running)
    return 0;
  Uint32 now   = SDL_GetTicks();
  Uint32 ticks = now - last_read;
  last_read    = now;
  return ticks;
}

bool
timer::is_running() const
{
  return running;
}

void
timer::start()
{
  start_time = SDL_GetTicks();
  last_read = start_time;
  running = true;
}

void
timer::toggle_start_pause()
{
  if (running)
    running = false;
  else
    start();
}
