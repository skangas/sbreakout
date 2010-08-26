#include <assert.h>
#include <cmath>
#include <iostream>

#include "game_board.hpp"
#include "game_brick.hpp"
#include "game_lib.hpp"

game_board::game_board()
  : last_update(0),
    paddle_x(0),
    paddle_y(0)
{
  surface = NULL;

  // Prepare sprites -- segfault if placed here, why?
  ball_sprite = NULL;
  brick_sprite = NULL;
  paddle_sprite = NULL;
  
  // Add default ball
  game_ball ball = game_ball(100, 700, vect(10, -20), 3.5);
  balls.push_back(ball);

  // Add default level
  game_brick b = game_brick(400, 100, 1);
  bricks.push_back(b);
}

game_board::~game_board()
{
}

void
game_board::blit()
{
  assert(surface != NULL);

  if (paddle_sprite == NULL)
    paddle_sprite = load_image("paddle.bmp");
  if (ball_sprite == NULL)
    ball_sprite = load_image("ball.png");
  if (brick_sprite == NULL)
    brick_sprite = load_image("yellow_brick.png");

  blit_balls();
  blit_bricks();
  blit_paddle();
}

void
game_board::blit_balls()
{
  typedef vector<game_ball>::const_iterator CI;
  
  for (CI ball = balls.begin(); ball != balls.end(); ball++)
    {
      SDL_Rect pos;
      pos.x = ball->x - BALL_RADIUS;
      pos.y = ball->y - BALL_RADIUS;
      pos.w = BALL_WIDTH;
      pos.h = BALL_HEIGHT;

      SDL_BlitSurface(ball_sprite, NULL, surface, &pos);
    }
}

void
game_board::blit_bricks()
{
  typedef vector<game_brick>::const_iterator CI;
  
  for (CI brick = bricks.begin(); brick != bricks.end(); brick++)
    {
      if (!brick->in_play())
        continue;

      SDL_Rect pos;
      pos.x = brick->get_x();
      pos.y = brick->get_y();
      pos.w = BRICK_WIDTH;
      pos.h = BRICK_HEIGHT;

      SDL_BlitSurface(brick_sprite, NULL, surface, &pos);
    }
}

void
game_board::blit_paddle()
{
  SDL_Rect pos;
  pos.x = paddle_x;
  pos.y = paddle_y;
  pos.w = 100;
  pos.h = 100;

  SDL_BlitSurface(paddle_sprite, NULL, surface, &pos);
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
  if (ball.y + BALL_RADIUS < paddle_y)
    return;
  // Ball center has passed paddle, so it can not be bounced.
  if (ball.y > paddle_y)
    return;

  // Find colliding x coordinate
  int left  = paddle_x;
  int right = paddle_x + PADDLE_WIDTH;

  int collide_x;

  if (ball.x < left)
    collide_x = left;
  else if (ball.x > right)
    collide_x = right;
  else
    collide_x = ball.x;

  // See if ball is close enough to bounce
  if (distance(ball.x, ball.y, collide_x, paddle_y) > BALL_RADIUS)
    return;

  // Ok, ball should be bounced.  Determine and set new direction.
  float hit_paddle = float(collide_x - paddle_x) / PADDLE_WIDTH;
  float angle      = PI + 0.2 + (PI - 0.4) * hit_paddle;

   LOG("angle %f hit_paddle %f", angle, hit_paddle);

  ball.direction.x = cos(angle);
  ball.direction.y = sin(angle);
  ball.direction.normalize();
}

void
game_board::handle_wall_collision(game_ball& ball, int new_x, int new_y)
{
  if (new_x + BALL_RADIUS > surface->w)
    ball.bounce_against_right();
  else if (new_x - BALL_RADIUS < 0)
    ball.bounce_against_left();
  else if (new_y + BALL_RADIUS > surface->h)
    ball.bounce_against_bottom();
  else if (new_y - BALL_RADIUS < 0)
    ball.bounce_against_top();
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
    paddle_x = x;
  else
    paddle_x = surface->w - PADDLE_WIDTH;
}

void
game_board::set_surface(SDL_Surface* surface)
{
  this->surface = surface;
  paddle_y = surface->h * 0.95;
}

void
game_board::update()
{
  int this_update = SDL_GetTicks();
  int ticks = this_update - last_update;

  typedef vector<game_ball>::iterator CI;

  // Iterate and try to move all balls while checking for collisions
  for (CI ball = balls.begin(); ball != balls.end(); ball++)
    {
      int new_x = ball->get_new_x(ticks);
      int new_y = ball->get_new_y(ticks);

      // Check for brick collision
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

  last_update = this_update;
}
