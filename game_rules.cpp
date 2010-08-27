#include "game_rules.hpp"

const int game_rules::brick_worth[] = {
  25, // type XX
  25, // type XX
  25, // type XX
  25, // type XX
  25, // type XX
  25, // type XX
  25, // type XX
  25, // type XX
};

game_rules::game_rules()
  : lives(3),
    score(0)
{ }

game_rules::~game_rules() { }

void
game_rules::brick_destroyed(int type)
{
  
}

void
game_rules::death()
{
  lives--;
}

int
game_rules::get_lives()
{
  return lives;
}

int
game_rules::get_score()
{
  return score;
}
