#ifndef GAME_BRICK_HPP
#define GAME_BRICK_HPP

/**
 * This class represents a brick.
 */
class game_brick
{
public:
  game_brick(int x, int y, int type);
  ~game_brick();

  int get_type() const;
  int get_x() const;
  int get_y() const;
  void hit();
  bool in_play() const;
  bool is_invincible() const;

private:
  static const int INVINCIBLE = -1;
  static const int brick_lives[];
  static const int brick_transforms_to[];

  int type;
  int invisible;
  int life;
  int x;
  int y;
};

#endif
