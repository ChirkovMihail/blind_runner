#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;

SDL_Renderer* g_renderer = NULL;
SDL_Window* g_window = NULL;

#include "texture.h"
#include "button.h"
#include "box.h"

TTexture g_start_menu_background;
TTexture g_inf_background;
const int MAN_FRAMES_TOTAL = 6;
TTexture g_man[MAN_FRAMES_TOTAL];

const int START_BUTTONS_TOTAL = 4;
TButton g_start_buttons[START_BUTTONS_TOTAL];
TButton g_to_start_menu_button;

void start_buttons_init_pos();

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
	std::string path;
	for (int i = 0; i < MAN_FRAMES_TOTAL; ++i) {
		path = "images/man/running_man_frame" + std::to_string(i + 1) + ".bmp";
		if (!g_man[i].load_from_file(path, 255, 255, 255)) {
			printf("Failed to load man texture!\n");
			return false;
		}
	}

	if (!g_to_start_menu_button.load_from_file("images/buttons/to_start_menu_button1.bmp")) {
		printf("Failed to load to_start_menu_button texture!\n");
		return false;
	}

	if (!g_inf_background.load_from_file("images/infinite_background/test_background5.bmp")) {
		printf("Failed to load infinite_background texture!\n");
		return false;
	}

	if (!g_start_menu_background.load_from_file("images/start_menu_background/start_menu_background2.bmp")) {
		printf("Failed to load start menu background texture! \n");
		return false;
	}

	if (!g_start_buttons[0].load_from_file("images/buttons/start_button1.bmp")) {
		printf("Failed to load start button texture!\n");
		return false;
	}	

	if (!g_start_buttons[1].load_from_file("images/buttons/settings_button1.bmp")) {
		printf("Failed to load settings button texture!\n");
		return false;
	}	

	if (!g_start_buttons[2].load_from_file("images/buttons/stat_button1.bmp")) {
		printf("Failed to load statistics button texture!\n");
		return false;
	}	

	if (!g_start_buttons[3].load_from_file("images/buttons/tutorial_button1.bmp")) {
		printf("Failed to load tutorial button texture!\n");
		return false;
	}
	start_buttons_init_pos();
	return true;
}

void close()
{
	g_start_menu_background.free();
	g_inf_background.free();

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

	main_loop();

	close();

	return 0;
}
