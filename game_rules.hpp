#ifndef GAME_RULES_HPP
#define GAME_RULES_HPP

class game_rules
{
public:
  game_rules();
  ~game_rules();

  void brick_destroyed(int type);
  void death();
  int get_lives();
  int get_score();

private:
  int lives;
  int score;

  const static int brick_worth[];
};

#endif // RULES_HPP
