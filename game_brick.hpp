#ifndef GAME_BRICK_HPP
#define GAME_BRICK_HPP

/**
 * This class represents a brick.
 */

class game_brick
{
public:
  game_brick(int x, int y, int life);
  ~game_brick();

  int get_x() const;
  int get_y() const;
  void hit();
  bool in_play() const;

private:
  int life;
  int x;
  int y;
  // add type here?
};

#endif
