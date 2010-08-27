#ifndef GAME_BRICK_HPP
#define GAME_BRICK_HPP

/**
 * This class represents a brick.
 */
class game_brick
{
public:
  game_brick(int x, int y, int type, int life);
  ~game_brick();

  int get_type() const;
  int get_x() const;
  int get_y() const;
  void hit();
  bool in_play() const;

private:
  int type;
  int invisible;
  int life;
  int x;
  int y;

  const static int INVINCIBLE = -1;
};

#endif
