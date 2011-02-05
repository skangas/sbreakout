#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include <string>
#include <vector>

#include "game_brick.hpp"

class level_loader
{
public:
  level_loader();
  level_loader(int width, int height);
  ~level_loader();

  std::vector<game_brick> load_level(std::string filename);

private:
  int width;
  int height;
  
  const static int MAX_COLUMNS = 20;
  const static int MAX_ROWS = 30;
};

#endif
