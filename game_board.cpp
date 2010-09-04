#include <assert.h>
#include <cmath>
#include <iostream>

#include "game_board.hpp"
#include "game_brick.hpp"
#include "game_lib.hpp"
#include "game_paddle.hpp"
#include "sprites.hpp"

game_board::game_board()
  : last_update(0),
    paused(false)
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
  int closest_distance;
  game_brick *closest_brick = NULL;

  // Look for the closest brick
  typedef vector<game_brick>::iterator BI;
  for (BI brick = bricks.begin(); brick != bricks.end(); brick++)
    {
      if (!brick->in_play())
        continue;

      SDL_Rect r;
      r.x = brick->get_x();
      r.y = brick->get_y();
      r.w = BRICK_WIDTH;
      r.h = BRICK_HEIGHT;
      
      int coll_x, coll_y;
      bool collision = collision_detection(&ball, &r, coll_x, coll_y);

      if (!collision)
        continue;
  
      // Keep this brick unless we have already seen a brick that was closer to the ball
      int dist = distance(new_x, new_y, coll_x, coll_y);
      if (closest_brick == NULL || dist < closest_distance)
        {
          closest_brick = &(*brick);
          closest_distance = dist;
        }
    }

  // If we did find a brick, execute the necessary actions
  if (closest_brick)
    {
      closest_brick->hit();

      ball.bounce_on_rect(closest_brick->get_x(), closest_brick->get_y(),
                          BRICK_WIDTH, BRICK_HEIGHT);

      signal_brick_destroyed(closest_brick->get_type());
    }
}

void
game_board::handle_paddle_collision(game_ball& ball, int new_x, int new_y)
{
  SDL_Rect r;
  r.x = paddle.x;
  r.y = paddle.y;
  r.w = PADDLE_WIDTH;
  r.h = PADDLE_HEIGHT;

  int coll_x, coll_y;
  bool collision = collision_detection(&ball, &r, coll_x, coll_y);

  if (!collision)
    return;

  // Ok, ball should be bounced.  Determine and set new direction.
  // ball.bounce_on_rect(paddle.x, paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT);

  // If we bounced on the top of the paddle, adjust ball angle slightly
  int left_of  = paddle.x - ball.x;
  int right_of = ball.x - (paddle.x + PADDLE_WIDTH);
  int above    = paddle.y - ball.y;
  if (above > left_of && above > right_of)
    {
      ball.bounce_against_bottom();

      float percent_from_left = float(coll_x - paddle.x) / PADDLE_WIDTH;
      float angle = PI * 1.2 + (PI * 0.6 ) * percent_from_left;

      ball.direction.x += cos(angle);
      ball.direction.y += sin(angle);
      ball.direction.normalize();
    }

}

void
game_board::handle_wall_collision(game_ball& ball, int new_x, int new_y)
{
  if (new_x + BALL_RADIUS > surface->w)
    ball.bounce_against_right();
  else if (new_x - BALL_RADIUS < 0)
    ball.bounce_against_left();
  else if (new_y - BALL_RADIUS < 0)
    ball.bounce_against_top();
}

bool
game_board::is_clear()
{
  typedef vector<game_brick>::iterator BI;

  if (bricks.size() > 0)
    for (BI brick = bricks.begin(); brick != bricks.end(); brick++)
      if (!brick->is_invincible())
        return 0;
  return 1;
}

void
game_board::set_level(vector<game_brick> new_bricks)
{
  bricks = new_bricks;
}

void
game_board::set_paddle(int x)
{
  if (x < 0)
    paddle.x = 0;
  else if (x > surface->w - PADDLE_WIDTH)
    paddle.x = surface->w - PADDLE_WIDTH;
  else
    paddle.x = x;
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
game_board::update(int ticks)
{
  if (!ticks)
    return;

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

      ball->set_ticks(ticks);

      int new_x = ball->get_new_x(ticks);
      int new_y = ball->get_new_y(ticks);

      // Check for brick collisions
      handle_brick_collision(*ball, new_x, new_y);

      // Check for paddle collision
      handle_paddle_collision(*ball, new_x, new_y);

      // Collision detection against the walls
      handle_wall_collision(*ball, new_x, new_y);

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
}
