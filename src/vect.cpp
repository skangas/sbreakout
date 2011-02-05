#include <cmath>
#include <iostream>

#include "vect.hpp"

/**
 * Default constructor.
 */
vect::vect() { }

vect::vect(float x, float y)
  : x(x),
    y(y)
{ }

/**
 * Copy constructor.
 */ 
vect::vect(const vect& v)
{
  x = v.x;
  y = v.y;
}

vect::~vect() { }

void
vect::bounce_against(vect other)
{
  std::cout << "begin x " << x << " y " << y << "\n";
  
  // Project on other vector
  vect p1 = this->project(other);

  // Project on left hand normal of other vector
  vect p2 = this->project(other.left_hand_normal());

  // Reverse the projection on normal
  p2.x = -p2.x;
  p2.y = -p2.y;

  // Add projections
  x = p1.x + p2.x;
  y = p1.y + p2.y;

  std::cout << x << " y " << y << "\n";
}

float
vect::dot(vect v2)
{
  return ((x * v2.x) + (y * v2.y));
}

/**
 * Return left hand normal.
 */
vect
vect::left_hand_normal()
{
  return vect(y, -x);
}

/**
 * Normalize to unit vector.
 */
void
vect::normalize()
{
  float l = length();
  std::cout << l << "\n";
  if (l > 1)
    {
      x /= l;
      y /= l;
    }
}

vect
vect::project(vect other)
{
  // Make a copy of the vector to project on, and normalize it
  vect copy = vect(other);
  copy.normalize();

  // Dot product with other vector
  float d = this->dot(other);

  std::cout << d << " d \n";

  // Calculate projection using simple formula because we had a unit vector
  // vector... DONE!
  return vect(d * copy.x, d * copy.y);
}

float
vect::length()
{
  return sqrt(pow(x, 2) + pow(y, 2));
}
