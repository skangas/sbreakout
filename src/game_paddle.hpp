#ifndef GAME_PADDLE_HPP
#define GAME_PADDLE_HPP

class game_paddle
{
public:
  game_paddle();
  ~game_paddle();

  void add_ball();
  bool has_ball();
  void release_ball();

  int x;
  int y;

private:
  bool ball;
};

#endif

