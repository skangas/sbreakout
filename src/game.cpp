#include "assert.h"
#include <iostream>
#include <string>

#include "game.hpp"
#include "game_rules.hpp"

#include "SDL/SDL_ttf.h"

game::game()
  : running(false),
    level(0),
    frame(0)
{
  board.signal_death.connect(sigc::mem_fun(rules, &game_rules::death));
  board.signal_brick_destroyed.connect(sigc::mem_fun(rules, &game_rules::brick_destroyed));
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
  game_area = SDL_CreateRGBSurface(SDL_SWSURFACE, GAME_AREA_W, GAME_AREA_H,
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

  Uint32 start = SDL_GetTicks();
  clock.start();
  running = true;
  timer frame_clock;
  while (running)
    {
      frame_clock.start();
      frame++;

      int ticks = clock.get_ticks();

      // Change to next level if needed
      maybe_next_level();

      // Clear surfaces
      SDL_FillRect(screen, NULL, 0);
      SDL_FillRect(game_area, NULL, 0);

      // Handle user input events
      handle_input_events();

      // Update and blit the game area
      board.update(ticks);
      board.blit();

      // Blit background
      SDL_BlitSurface(background, NULL, screen, NULL);

      // Show player life
      sprintf(buffer, "LIFE   %d", rules.get_lives());
      status = TTF_RenderText_Solid(font, buffer, status_color);
      SDL_Rect status_loc = { GAME_AREA_X + game_area->w + 25, GAME_AREA_Y };
      SDL_BlitSurface(status, NULL, screen, &status_loc);

      // Show player score
      sprintf(buffer, "SCORE   %d", rules.get_score());
      status = TTF_RenderText_Solid(font, buffer, status_color);
      status_loc.y += 35;
      SDL_BlitSurface(status, NULL, screen, &status_loc);

      // Show FPS counter
      SDL_Surface* fps_surface;
      int fps = calculate_fps();
      sprintf(buffer, "%d FPS", fps);
      fps_surface = TTF_RenderText_Solid(fps_font, buffer, fps_color);
      SDL_Rect fps_loc = { SCREEN_WIDTH - 50, 5, 100, 100 };
      SDL_BlitSurface(fps_surface, NULL, screen, &fps_loc);

      // Show big message if there is one
      show_big_message(ticks);

      // Blit main game area -- has to be done after showing big message
      SDL_Rect target = { GAME_AREA_X, GAME_AREA_Y };
      SDL_BlitSurface(game_area, NULL, screen, &target);

      SDL_Flip(screen);

      // Limit the frame rate to FRAME_PER_SECOND
      Uint32 frame_time = frame_clock.get_ticks();
      if (frame_time < 1000 / FRAMES_PER_SECOND)
        SDL_Delay((1000/FRAMES_PER_SECOND) - frame_time);
    }
}

int
game::title_screen()
{
  char buffer[1024];
  assert(screen != NULL);

  SDL_ShowCursor(SDL_ENABLE);

  // Prepare background
  SDL_Surface *background = NULL;
  background = load_image("titlescreen-1024.png");

  // Prepare text
  TTF_Font* title_font = TTF_OpenFont("fonts/LiberationSerif-Bold.ttf", 30);
  TTF_Font* font = TTF_OpenFont("fonts/LiberationSerif-Bold.ttf", 24);

  while (true)
    {
      SDL_FillRect(screen, NULL, 0);
      SDL_FillRect(game_area, NULL, 0);

      // Blit background
      SDL_BlitSurface(background, NULL, screen, NULL);
      SDL_Flip(screen);

      highlight_button(NEW_GAME_X, NEW_GAME_Y, NEW_GAME_W, NEW_GAME_H);
      // draw_button("Foo", x, y);

      SDL_Event event;

      while (SDL_PollEvent(&event))
        {
          switch (event.type)
            {
            case SDL_KEYDOWN:
              switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                  return -1;
                }
              break;
            case SDL_MOUSEBUTTONDOWN:
              if (event.button.button == SDL_BUTTON_LEFT)
                board.handle_event_mouse_left();
              break;
            case SDL_QUIT:
              return -1;
            }
        }
    }
}

bool
game::quit()
{
  TTF_Quit();
  SDL_FreeSurface(screen);
  SDL_Quit();
} 

// private methods

int game::calculate_fps()
{
  static int fps = 0;
	static float last = 0.0f;
	float now = SDL_GetTicks();
	fps = fps*0.9 + (100.0f/(now - last));
	last = now;
  return fps;
}

int
game::show_big_message(int ticks)
{
  if (big_message_ticks > 0)
    {
      big_message_ticks -= ticks;

      /* Set up the text. */
      TTF_Font* bm_font = TTF_OpenFont("fonts/LiberationSerif-Bold.ttf", 32);
      SDL_Color bm_color = { 0xFF, 0x00, 0x00 };
      SDL_Surface* bm_surface;
      bm_surface = TTF_RenderText_Blended(bm_font, big_message.c_str(), bm_color);

      /* We want to center the text. */
      SDL_Rect bm_rect =
        {
          (game_area->w/2) - (bm_surface->w/2),
          (game_area->h/2) - (bm_surface->h/2),
          bm_surface->w, bm_surface->h
        };

      const int VERTICAL_PADDING = 15;
      const int HORIZONTAL_PADDING = 30;
      const int BORDER_WIDTH = 2;
      const Uint32 border_color = SDL_MapRGB(game_area->format, 0xFF, 0xFF, 0xFF);
      const Uint32 bg_color = SDL_MapRGB(game_area->format, 0x00, 0x00, 0x00);

      /* We will draw two rectangles on top of each other to emulate having a
       * border.  It will surely look just fine.  We begin with the border. */
      bm_rect.x -= VERTICAL_PADDING + BORDER_WIDTH;
      bm_rect.y -= HORIZONTAL_PADDING + BORDER_WIDTH;
      bm_rect.w += 2 * (HORIZONTAL_PADDING + BORDER_WIDTH);
      bm_rect.h += 2 * (VERTICAL_PADDING + BORDER_WIDTH);
      SDL_FillRect(game_area, &bm_rect, border_color);

      /* Second, inner rectangle. */
      bm_rect.x += BORDER_WIDTH;
      bm_rect.y += BORDER_WIDTH;
      bm_rect.w -= 2 * BORDER_WIDTH;
      bm_rect.h -= 2 * BORDER_WIDTH;
      SDL_FillRect(game_area, &bm_rect, bg_color);

      /* Finally, show the text. */
      bm_rect.x += HORIZONTAL_PADDING;
      bm_rect.y += VERTICAL_PADDING;
      bm_rect.w -= 2 * HORIZONTAL_PADDING;
      bm_rect.h -= 2 * VERTICAL_PADDING;
      SDL_BlitSurface(bm_surface, NULL, game_area, &bm_rect);
      return 1;
    }
  return 0;
}

/**
 * Handle all input events.
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
              x -= GAME_AREA_X;
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

void
game::maybe_next_level()
{
  if (board.is_clear())
    {
      level++;

      if (level > LAST_LEVEL)
        {
          set_big_message(10000, "You have won!");
        }

      // Load level
      char buffer[1024];
      sprintf(buffer, "data/levels/level%03d.txt", level);
      board.set_level(loader.load_level(buffer));

      // Give a message
      set_big_message(4000, "Level %d", level);
    }
}

void
game::set_big_message(int ticks, std::string format, ...)
{
  // Set ticks
  big_message_ticks = ticks;

  // Set message
  char buffer[1024];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format.c_str(), args);
  va_end(args);

  big_message = buffer;
}

void
game::highlight_button(int x, int y, int w, int h)
{
  SDL_Renderer* renderer = NULL;

  renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);

  SDL_Rect r = { NEW_GAME_X, NEW_GAME_Y, NEW_GAME_W, NEW_GAME_H };

  SDL_RenderDrawRect(renderer, &r);

  SDL_RenderPresent(renderer);

  return;
}
