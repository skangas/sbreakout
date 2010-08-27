#include <assert.h>
#include <cmath>
#include <iostream>

#include "game_board.hpp"
#include "game_brick.hpp"
#include "game_lib.hpp"
#include "game_paddle.hpp"
#include "sprites.hpp"

game_board::game_board()
  : last_update(0)
{
  surface = NULL;
}

game_board::~game_board()
{
}

void
game_board::blit()
{
  assert(surface != NULL);

  // Blit all balls
  typedef vector<game_ball>::const_iterator BAI;
  for (BAI ball = balls.begin(); ball != balls.end(); ball++)
    {
      graphics.blit_sprite(sprites::BALL, 0, ball->x - BALL_RADIUS, ball->y - BALL_RADIUS);
    }  

  // Blit all bricks
  typedef vector<game_brick>::const_iterator BRI;
  for (BRI brick = bricks.begin(); brick != bricks.end(); brick++)
    {
      if (!brick->in_play())
        continue;

      graphics.blit_sprite(sprites::BRICK, brick->get_type(),
                           brick->get_x(), brick->get_y());
    }

  // Blit paddle
  graphics.blit_sprite(sprites::PADDLE, 0, paddle.x, paddle.y);

  // Blit ball on paddle
  if (paddle.has_ball())
    {
      int x = paddle.x + PADDLE_WIDTH * 0.5 - BALL_RADIUS;
      int y = paddle.y - BALL_HEIGHT;
      graphics.blit_sprite(sprites::BALL, 0, x, y);
    }
}

void
game_board::handle_event_mouse_left()
{
  // Release paddle ball
  if (paddle.has_ball())
    {
      int x = paddle.x + PADDLE_WIDTH * 0.5;
      int y = paddle.y - BALL_HEIGHT;
      balls.push_back(game_ball(x, y, vect(0, -1), 1.0));
      paddle.release_ball();
    }

  // More to come, e.g. shooting laser
}

void
game_board::handle_brick_collision(game_ball& ball, int new_x, int new_y)
{
  typedef vector<game_brick>::iterator BI;

  int closest_distance = -1;
  game_brick *closest_brick = NULL;

  enum direction { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 4, INSIDE = 8 };
  direction ball_hit;

  // Look for the closest brick
  for (BI brick = bricks.begin(); brick != bricks.end(); brick++)
    {
      if (!brick->in_play())
        continue;

      int left   = brick->get_x();
      int right  = brick->get_x() + BRICK_WIDTH;
      int top    = brick->get_y();
      int bottom = brick->get_y() + BRICK_HEIGHT;
      
      int x_collision, y_collision;
  
      // Find colliding x coordinate
      
      // NOTE: We use the _new_ X/Y to decide collision, but the _old_ X to
      // decide which direction the ball was coming from.  This is to avoid
      // using the inside of the brick to decide on direction.
      if (new_x < left)
          x_collision = left;
      else if (new_x > right)
          x_collision = right;
      else
        x_collision = new_x;

      // Find colliding y coordinate
      if (new_y < top)
          y_collision = top;
      else if (new_y > bottom)
          y_collision = bottom;
      else
        y_collision = new_y;

      int dist = distance(new_x, new_y, x_collision, y_collision);

      // No collision if the ball is too far from this brick
      if (dist > BALL_RADIUS)
        continue;

      // Cancel if we have already seen a brick that was closer to the ball
      if (closest_distance != -1 && dist > closest_distance)
        continue;
      else
        {
          closest_distance = dist;
          closest_brick = &(*brick);
        }

      // TODO:
      // if (ball.invincible) continue;

      // Bounce the ball depending on its position.  Here we need to decide
      // which border it makes the most sense to bounce against.  We want to
      // bounce the ball against the border closest to the collision.

      int above = top - ball.y;
      int below = ball.y - bottom;
      int left_of = left - ball.x;
      int right_of = ball.x - right;

      ball_hit = INSIDE;

      if (left_of > 0)
        {
          if (left_of > below && left_of > above)
            {
              ball_hit = RIGHT;
              above = below = 0;
            }
        }
      else if (right_of > 0)
        {
          if (right_of > below && right_of > above)
            {
              ball_hit = LEFT;
              above = below = 0;
            }
        }

      if (above > 0)
        ball_hit = BOTTOM;
      else if (below > 0)
        ball_hit = TOP;
    }

  // If we did find a brick, execute the necessary actions
  if (closest_brick)
    {
      closest_brick->hit();

      switch (ball_hit)
        {
        case RIGHT:  ball.bounce_against_right();  break;
        case LEFT:   ball.bounce_against_left();   break;
        case TOP:    ball.bounce_against_top();    break;
        case BOTTOM: ball.bounce_against_bottom(); break;
        default: break; // we were inside the brick
        }
    }
}

void
game_board::handle_paddle_collision(game_ball& ball, int new_x, int new_y)
{
  // Ball is too far from paddle
  if (ball.y + BALL_RADIUS < paddle.y)
    return;
  // Ball center has passed paddle, so it can not be bounced.
  if (ball.y > paddle.y)
    return;

  // Find colliding x coordinate
  int left  = paddle.x;
  int right = paddle.x + PADDLE_WIDTH;

  int collide_x;

  if (ball.x < left)
    collide_x = left;
  else if (ball.x > right)
    collide_x = right;
  else
    collide_x = ball.x;

  // See if ball is close enough to bounce
  if (distance(ball.x, ball.y, collide_x, paddle.y) > BALL_RADIUS)
    return;

  // Ok, ball should be bounced.  Determine and set new direction.
  float hit_paddle = float(collide_x - paddle.x) / PADDLE_WIDTH;
  float angle      = PI + 0.2 + (PI - 0.4) * hit_paddle;

   LOG("angle %f hit_paddle %f", angle, hit_paddle);

  ball.direction.x = cos(angle);
  ball.direction.y = sin(angle);
  ball.direction.normalize();
}

void
game_board::handle_wall_collision(vector<game_ball>::iterator ball, int new_x, int new_y)
{
  if (new_x + BALL_RADIUS > surface->w)
    ball->bounce_against_right();
  else if (new_x - BALL_RADIUS < 0)
    ball->bounce_against_left();
  else if (new_y - BALL_RADIUS < 0)
    ball->bounce_against_top();
}

void
game_board::set_level(vector<game_brick> new_bricks)
{
  bricks = new_bricks;
}

void
game_board::set_paddle(int x)
{
  if (x < surface->w - PADDLE_WIDTH)
    paddle.x = x;
  else
    paddle.x = surface->w - PADDLE_WIDTH;
}

void
game_board::set_surface(SDL_Surface* surface)
{
  this->surface = surface;
  paddle.y = surface->h * 0.95;
  graphics.set_surface(surface);
  graphics.load_sprites();
}

void
game_board::update()
{
  int this_update = SDL_GetTicks();
  int ticks = this_update - last_update;

  // If there are no balls, this means we have recently died, and since update()
  // has been called, someone has decided we should continue playing.  Add ball.
  if (balls.size() == 0)
    paddle.add_ball();

  typedef vector<game_ball>::iterator BAI;

  // Holds a ball that has fallen through floor -- see below
  BAI ball_to_erase = balls.end();

  // Iterate and try to move all balls while checking for collisions
  for (BAI ball = balls.begin(); ball != balls.end(); ball++)
    {
      // Handle ball falling through floor
      if (ball->y > surface->h)
        {
          // Signal death if we are now out of balls
          if (balls.size() == 1)
              signal_death.emit();
          // We erase the ball after the loop is done
          ball_to_erase = ball;
          continue;
        }

      int new_x = ball->get_new_x(ticks);
      int new_y = ball->get_new_y(ticks);

      // Check for brick collision
      handle_brick_collision(*ball, new_x, new_y);

      // Check for paddle collision
      handle_paddle_collision(*ball, new_x, new_y);

      // Collision detection against the walls
      handle_wall_collision(ball, new_x, new_y);

      // There was a choice between looping here, and simply assuming there are
      // no more collisions this frame, which means we can set the ball off in
      // the new direction.  This might bite us later though, but whatever...
      new_x = ball->get_new_x(ticks);
      new_y = ball->get_new_y(ticks);

      // std::cout << ball->x << " " << add_x << " " << pixels_to_move << std::endl;
      // std::cout << ball->y << " " << add_y << " " << pixels_to_move << std::endl;
      
      // Verify the new position
      ball->move(ticks);
    }

  // OK, we have looped through all balls.  It is now safe to erase ball.
  if (ball_to_erase != balls.end())
    balls.erase(ball_to_erase);

  last_update = this_update;
}
