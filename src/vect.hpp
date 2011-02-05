#ifndef VECT_HPP
#define VECT_HPP

// Class representing a two-dimensional vector

class vect
{
public:
  vect();
  vect(const vect&);
  vect(float x, float y);
  ~vect();

  float x;
  float y;

  void bounce_against(vect);
  float dot(vect);
  vect left_hand_normal();
  void normalize();
  vect project(vect);
  float length();
};

#endif
