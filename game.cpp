#include "assert.h"
#include <iostream>
#include <string>

#include "game.hpp"
#include "game_rules.hpp"

game::game()
  : running(false),
    level(0),
    frame(0)
{
  board.signal_death.connect(sigc::mem_fun(rules, &game_rules::death));
}

game::~game() { }

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

  // Create level loader
  loader = level_loader(game_area->w, game_area->h);

  // Initialize sdl-ttf
  if (TTF_Init() == -1)
    return false;

  // Set window title
  SDL_WM_SetCaption("Stefan's Breakout", NULL);

  // Hide the mouse cursor
  SDL_ShowCursor(SDL_DISABLE); 

  return true;
}

void
game::run()
{
  char buffer[1024];
  assert(screen != NULL);

  // Prepare background
  SDL_Surface *background = NULL;
  background = load_image("background.jpg");

  // Prepare text
  TTF_Font* font = TTF_OpenFont("fonts/LiberationSerif-Bold.ttf", 16);
  TTF_Font* fps_font = TTF_OpenFont("fonts/ProggyCleanSZBP.ttf", 16);
  SDL_Color status_color = { 0x7F, 0xDF, 0xFF };
  SDL_Color fps_color = { 0x00, 0x33, 0x00 };

  board.set_level(loader.load_level("level001.txt"));

  Uint32 start = SDL_GetTicks();
  clock.start();

  running = true;
  while (running)
    {
      // maybe_next_level();

      int frame_start = SDL_GetTicks();
      int ticks = clock.get_ticks();

      // Clear surfaces
      SDL_FillRect(screen, NULL, 0);
      SDL_FillRect(game_area, NULL, 0);

      // Update and blit the game area
      board.blit();
      board.update(ticks);

      // Handle user input events
      handle_input_events();

      // Blit main surfaces
      SDL_BlitSurface(background, NULL, screen, NULL);
      SDL_Rect target = { SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.05 };
      SDL_BlitSurface(game_area, NULL, screen, &target);

      // Show game status texts
      sprintf(buffer, "LIFE   %d", rules.get_lives());
      status = TTF_RenderText_Solid(font, buffer, status_color);
      SDL_Rect status_loc = { target.x + game_area->w + 25, target.y,
                              10000, 10000 };
      SDL_BlitSurface(status, NULL, screen, &status_loc);

      sprintf(buffer, "SCORE   %d", rules.get_score());
      status = TTF_RenderText_Solid(font, buffer, status_color);
      status_loc.y += 35;
      SDL_BlitSurface(status, NULL, screen, &status_loc);

      // Limit the frame rate to FRAME_PER_SECOND
      frame++;
      int frame_ticks = SDL_GetTicks() - frame_start;
      if (frame_ticks < 1000 / FRAMES_PER_SECOND)
        SDL_Delay((1000/FRAMES_PER_SECOND) - frame_ticks);

      // Show FPS counter
      SDL_Surface* fps_surface;

      int fps = frame / float((frame_start - start) / 1000.0);
      sprintf(buffer, "%d FPS", fps);
      fps_surface = TTF_RenderText_Solid(fps_font, buffer, fps_color);
      SDL_Rect fps_loc = { SCREEN_WIDTH - 50, 5, 100, 100 };
      SDL_BlitSurface(fps_surface, NULL, screen, &fps_loc);

      SDL_Flip(screen);
    }
}

bool
game::quit()
{
  SDL_FreeSurface(screen);
  SDL_Quit();
} 

// private methods

/**
 * Handle input events.
 */
void
game::handle_input_events()
{
  SDL_Event event;

  while (SDL_PollEvent(&event))
    {
      switch (event.type)
        {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
              running = false;
              break;
            case SDLK_p:
              clock.toggle_start_pause();
              break;
            }
          break;
        case SDL_MOUSEMOTION:
          if (clock.is_running())
            {
              int x = event.motion.x;
              x -= SCREEN_WIDTH * 0.05;
              x -= 26; // center paddle to cursor
              board.set_paddle(x);
            }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (event.button.button == SDL_BUTTON_LEFT)
            board.handle_event_mouse_left();
          break;
        case SDL_QUIT:
          running = false;
          break;
        }
    }      
}
