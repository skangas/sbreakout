#include <iostream>
#include <string>

#include "game.hpp"

game::game()
  : running(false),
    frame(0),
    loader(SCREEN_WIDTH, SCREEN_HEIGHT)
{
}

game::~game()
{
}

void
game::run()
{
  // Prepare background
  SDL_Surface *background = NULL;
  background = load_image("background.jpg");

  // Prepare text
  TTF_Font* font = TTF_OpenFont("fonts/ProggyCleanSZBP.ttf", 16);
  SDL_Color text_color = { 255, 0, 0 };
  status = TTF_RenderText_Solid(font, "The quick brown fox jumps over the lazy dog", text_color);
  board.set_level(loader.load_level("level001.txt"));

  running = true;
  while (running)
    {
      int frame_start = SDL_GetTicks();

      SDL_FillRect(screen, NULL, 0);

      SDL_BlitSurface(background, NULL, screen, NULL);

      handle_events();

      board.update();
      board.blit();

      SDL_BlitSurface(status, NULL, screen, NULL);

      SDL_Flip(screen);

      // Limit the frame rate to FRAME_PER_SECOND
      int frame_ticks = SDL_GetTicks() - frame_start;
      if (frame_ticks < 1000 / FRAMES_PER_SECOND)
        SDL_Delay((1000/FRAMES_PER_SECOND) - frame_ticks);

      frame++;
      std::cout << frame << std::endl;
    }
}

bool
game::init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    return false;

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if (screen == NULL)
    return false;

  if (TTF_Init() == -1)
    return false;

  board.set_surface(screen);

  // Hide mouse cursor
  SDL_ShowCursor(SDL_DISABLE); 

  // Set window title
  SDL_WM_SetCaption( "Stefan's Breakout", NULL );

  return true;
}

bool
game::quit()
{
  SDL_FreeSurface(screen);
  SDL_Quit();
} 

// private methods

void
game::handle_events()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_MOUSEMOTION)
          board.set_paddle(event.motion.x);
      
      if (event.type == SDL_QUIT)
        running = false;
    }      
}
