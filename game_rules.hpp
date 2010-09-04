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
  static const int brick_values[];

  int lives;
  int score;
};

#endif // RULES_HPP
