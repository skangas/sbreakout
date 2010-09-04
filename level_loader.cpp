#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "level_loader.hpp"

level_loader::level_loader() { }

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

  // Make some calculations
  int col_width  = ceilf(width / float(MAX_COLUMNS));
  int row_height = ceilf(height / float(MAX_ROWS));

  // Initialize count
  int row = 0;
  int col = 0;

  // Read the level file
  std::ifstream file;
  file.open(filename.c_str());
  std::string data = slurp(file);

  // Parse the level
  std::stringstream ss(data);
  while (!ss.eof())
    {
      int type;
      // If there is an error, warn and stop parsing
      if ((ss >> type).fail())
        {
          std::cerr << "There was an error in the level file " << filename
                    << " while parsing level row " << row << " col " << col << "\n";
          continue;
        }

      // EOF marker
      if (type == 99)
        break;

      // Wrap on last column
      if (col >= MAX_COLUMNS)
        {
          col = 0;
          row++;
        }

      // In level files, we take 0 to mean no brick, but the internal brick
      // type indexing in fact begins at 0, not 1.
      if (type != 0)
        {
          type--;
          // Create the brick
          int x = col * col_width;
          int y = row * row_height;
          bricks.push_back(game_brick(x, y, type));
        }

      col++;
    }
  
  file.close();

  std::cout << "Number of bricks on level: " << bricks.size() << "\n";

  return bricks;
}

