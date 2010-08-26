#include <fstream>
#include <iostream>
#include <sstream>

#include "level_loader.hpp"

level_loader::level_loader(int width, int height)
  : width(width),
    height(height)
{
}

level_loader::~level_loader() { }

inline std::string slurp(std::ifstream& ifs) {
  std::stringstream sstr;
  sstr << ifs.rdbuf();
  return sstr.str();
}

std::vector<game_brick>
level_loader::load_level(std::string filename)
{
  std::vector<game_brick> bricks;

  std::ifstream file;
  file.open(filename.c_str());

  std::string data = slurp(file);

  // Make some calculations
  int col_width = width / MAX_COLUMNS;
  int row_height = height / MAX_ROWS;

  // Parse the level
  int row = 0;
  int col = 0;

  for (const char* c = data.c_str(); *c != '\0'; c++)
    {
      if (col > MAX_COLUMNS)
        {
          col = 0;
          row++;
        }

      switch (*c)
        {
        case '\n':
          col = 0;
          row++;
          break;
        case ' ':
          col++;
          break;
        default:
          // Assume we have a number
          int type = *c - '0';

          int x = col * col_width;
          int y = row * row_height;

          bricks.push_back(game_brick(x, y, 1));

          col++;
        }
    }
  
  file.close();

  std::cout << "LÄNGD" << bricks.size() << "\n";

  return bricks;
}

