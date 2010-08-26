#include "assert.h"
#include <iostream>
#include <string>

#include "game.hpp"

game::game()
  : running(false),
    frame(0)
{
}

game::~game()
{
}

void
game::run()
{
  assert(screen != NULL);

  // Prepare background
  SDL_Surface *background = NULL;
  background = load_image("background.jpg");

  // Prepare text
  TTF_Font* font = TTF_OpenFont("fonts/ProggyCleanSZBP.ttf", 16);
  SDL_Color text_color = { 255, 0, 0 };
  status = TTF_RenderText_Solid(font, "---", text_color);
  board.set_level(loader.load_level("level001.txt"));

  int start = SDL_GetTicks();

  running = true;
  while (running)
    {
      int frame_start = SDL_GetTicks();
      frame++;

      // Clear surfaces
      SDL_FillRect(screen, NULL, 0);
      SDL_FillRect(game_area, NULL, 0);

      // Update and blit the game area
      board.update();
      board.blit();

      // Blit surfaces
      SDL_BlitSurface(background, NULL, screen, NULL);
      SDL_Rect target = { SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.05 };
      SDL_BlitSurface(game_area, NULL, screen, &target);

      handle_events();


      // SDL_BlitSurface(status, NULL, screen, NULL);

      // Limit the frame rate to FRAME_PER_SECOND
      int frame_ticks = SDL_GetTicks() - frame_start;
      if (frame_ticks < 1000 / FRAMES_PER_SECOND)
        SDL_Delay((1000/FRAMES_PER_SECOND) - frame_ticks);

      // Show FPS counter
      SDL_Surface* fps_surface;
      char buffer[256];
      int fps = frame / float((frame_start - start) / 1000.0);
      sprintf(buffer, "%d FPS", fps);
      fps_surface = TTF_RenderText_Solid(font, buffer, text_color);
      SDL_Rect loc = { SCREEN_WIDTH - 50, 5, 100, 100 };
      SDL_BlitSurface(fps_surface, NULL, screen, &loc);

      SDL_Flip(screen);
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

  // Create game area
  game_area = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.9,
                                   SCREEN_BPP, 0, 0, 0, 0);

  board.set_surface(game_area);
  loader = level_loader(game_area->w, game_area->h);

  // Initialize sdl-ttf
  if (TTF_Init() == -1)
    return false;

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
      switch (event.type)
        {
        case SDL_MOUSEMOTION:
          board.set_paddle(event.motion.x);
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            running = false;
          break;
        case SDL_QUIT:
          running = false;
          break;
        default:
          break;
        }
    }      
}
