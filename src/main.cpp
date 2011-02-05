#include "game.hpp"

int main(int argc, char* args[])
{
  game g;

  if (g.init() == false)
    return 1;

  g.run();

  if (g.quit() == false)
    return 1;
  
  return 0;
}

