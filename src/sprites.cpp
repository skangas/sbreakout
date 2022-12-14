#include <string>

#include "game_lib.hpp"
#include "sprites.hpp"

// Below are loads of constants that are hard-coded, since they depend on the
// sprite-sheet anyways.  This sucks but is kind of hard to avoid.

const std::string filenames[4] = {
  "graphics/ball.png",
  "graphics/bricks.png",
  "graphics/extras.png",
  "graphics/paddle.png"
};

const int alpha[4][3] = {
  { 0, 0, 0 },    // BALL
  { -1, -1, -1 }, // BRICK
  { -1, -1, -1 }, // BONUS
  { 0, 0, 0 }     // PADDLE
};

// Offsets for images in sprite sheets
const int ball_offsets[] = {
  0, 0,
  20, 0,
  40, 0,
  60, 0,
};

const int brick_offsets[] = {
  0, 0,
  40, 0,
  80, 0,
  120, 0,
  160, 0,
  200, 0,
  240, 0,
  280, 0,
  320, 0,
  360, 0,
  400, 0,
  440, 0,
  480, 0,
  520, 0,
  560, 0,
  600, 0,
  640, 0,
  680, 0,
  720, 0,
  760, 0,
  800, 0,
};
const int bonus_offsets[] = {
  0, 0,
};
const int paddle_offsets[] = {
  0, 0,
};

const vector<int> offsets[4] = {
  vector<int>(ball_offsets, ball_offsets + sizeof(ball_offsets)/sizeof(int)),
  vector<int>(brick_offsets, brick_offsets + sizeof(brick_offsets)/sizeof(int)),
  vector<int>(bonus_offsets, bonus_offsets + sizeof(bonus_offsets)/sizeof(int)),
  vector<int>(paddle_offsets, paddle_offsets + sizeof(paddle_offsets)/sizeof(int)),
};

const static int sizes[] = {
  12, 12, // BALL
  40, 20, // BRICK (39?)
  40, 20, // BONUS (39?)
  53, 17, // PADDLE
};

sprites::sprites() { }

sprites::~sprites() { }

/**
 * Blit a sprite onto the surface.
 * @args type the sprite chart to use, see enum type for possible values
 * @args index the number of the sprite on chart
 */
void
sprites::blit_sprite(int type, int index, int x, int y)
{
  SDL_Rect clip;
  clip.x = offsets[type][index*2];
  clip.y = offsets[type][index*2+1];
  clip.w = sizes[type*2];
  clip.h = sizes[type*2+1];

  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  pos.w = clip.w;
  pos.h = clip.h;

  SDL_BlitSurface(charts[type], &clip, surface, &pos);
}

void
sprites::load_sprites()
{
  for (int i = 0; i < 4; i++)
    {
      charts[i] = load_image(filenames[i], alpha[i][0], alpha[i][1], alpha[i][2]);
    }
}

void
sprites::set_surface(SDL_Surface* surface)
{
  this->surface = surface;
}

