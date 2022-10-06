#include "game.hpp"

int main(int argc, char* args[])
{
  game g;

  if (g.init() == false)
    return 1;

  // int choice = g.title_screen();
  // switch (choice)
  //   {
  //   case 1:
      g.run();
    //   break;
    // }

  if (g.quit() == false)
    return 1;
  
  return 0;
}

