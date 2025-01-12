#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

SDL_Renderer* g_renderer = NULL;
SDL_Window* g_window = NULL;

#include "texture.h"
#include "button.h"

TTexture g_start_menu_background;
TTexture g_inf_background;

const int START_BUTTONS_TOTAL = 4;
TButton g_start_buttons[START_BUTTONS_TOTAL];

#include "animations.h"
#include "loops.h"

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	g_window = SDL_CreateWindow("BlindRunner", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (g_window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

void start_buttons_init_pos()
{
	g_start_buttons[0].set_pos(205, 470);
	g_start_buttons[1].set_pos(g_start_buttons[0].get_x() + g_start_buttons[0].get_width() + 10, g_start_buttons[0].get_y());
	g_start_buttons[2].set_pos(g_start_buttons[1].get_x() + g_start_buttons[1].get_width() + 10, g_start_buttons[0].get_y());
	g_start_buttons[3].set_pos(g_start_buttons[2].get_x() + g_start_buttons[2].get_width() + 10, g_start_buttons[0].get_y());
}

bool load_media()
{
	if (!g_inf_background.load_from_file("images/infinite_background/test_background3.bmp")) {
		printf("Failed to load start menu background texture!\n");
		return false;
	}

	if (!g_start_menu_background.load_from_file("images/start_menu_background2.bmp")) {
		printf("Failed to load start menu background texture! \n");
		return false;
	}

	if (!g_start_buttons[0].load_from_file("images/start_button1.bmp")) {
		printf("Failed to load start button texture!\n");
		return false;
	}	

	if (!g_start_buttons[1].load_from_file("images/settings_button1.bmp")) {
		printf("Failed to load settings button texture!\n");
		return false;
	}	

	if (!g_start_buttons[2].load_from_file("images/stat_button1.bmp")) {
		printf("Failed to load statistics button texture!\n");
		return false;
	}	

	if (!g_start_buttons[3].load_from_file("images/tutorial_button1.bmp")) {
		printf("Failed to load tutorial button texture!\n");
		return false;
	}
	start_buttons_init_pos();
}

void close()
{
	g_start_menu_background.free();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	g_renderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init()) {
		printf("Failed to initialize!\n");
		return 1;
	}

	if (!load_media()) {
		printf("Failed to load media!\n");
		return 2;
	}

	int i, j;
	bool main_loop_flag = false, start_animation_on_process = false;
	SDL_Event e;

	while (!main_loop_flag)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				main_loop_flag = true;

			for (i = 0; i < START_BUTTONS_TOTAL; ++i)
				g_start_buttons[i].handle_event(&e);
		}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		g_start_menu_background.render(0, 0);

		for (i = 0; i < START_BUTTONS_TOTAL; ++i) {
			if (g_start_buttons[i].get_curr_sprite() == BUTTON_SPRITE_MOUSE_DOWN && g_start_buttons[i].get_pressed() == false) {
				for (j = 0; j < START_BUTTONS_TOTAL; ++j)
					g_start_buttons[j].set_pressed(true);
				start_buttons_animation(i);
				break;
			}
		}
		
		if (g_start_buttons[0].get_curr_sprite() == BUTTON_SPRITE_MOUSE_DOWN) {
			game_loop();
			break;
		}

		for (i = 0; i < START_BUTTONS_TOTAL; ++i)
			g_start_buttons[i].render(g_start_buttons[i].get_x(), g_start_buttons[i].get_y());

		SDL_RenderPresent(g_renderer);
	}

	close();

	return 0;
}
