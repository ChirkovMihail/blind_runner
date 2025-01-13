#pragma once

enum LOOP_RETURNS
{
	LOOP_EXIT = 0,
	TO_START_MENU = 1
};

bool intersect(int a, int b, int c, int d)
{
	if (c <= a && a <= d || c <= b && b <= d)
		return true;
	if (a <= c && c <= b || a <= d && d <= b)
		return true;
	return false;
}

LOOP_RETURNS game_loop()
{
	int pace = 5;
	int small_box_x = 0, small_box_y = 0;
	int big_box_x = 0, big_box_y = 0;
	bool game_loop_flag = false, small_box_created = false, big_box_created = false;
	SDL_Event e;
	SDL_Rect clip;

	clip.x = 0;
	clip.y = 0;
	clip.w = SCREEN_WIDTH;
	clip.h = SCREEN_HEIGHT;

	g_to_start_menu_button.set_pos(470, 470);

	srand(time(NULL));

	while (!game_loop_flag)
	{
		while (SDL_PollEvent(&e) != 0) 
		{
			if (e.type == SDL_QUIT) {
				game_loop_flag = true;
				return LOOP_EXIT;
			}
			g_to_start_menu_button.handle_event(&e);
		}

		if (g_to_start_menu_button.get_curr_sprite() == BUTTON_SPRITE_MOUSE_DOWN) {
			return TO_START_MENU;
		}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		g_inf_background.render(0, 0, &clip);
		g_to_start_menu_button.render(g_to_start_menu_button.get_x(), g_to_start_menu_button.get_y());

		if (!small_box_created) {
			small_box_created = true;
			small_box_x = 960 + (rand() * rand() + rand()) % 960;
			if (big_box_created) {
				if (intersect(small_box_x, small_box_x + g_small_box.get_width(), big_box_x, big_box_x + g_big_box.get_width())) {
					small_box_x = big_box_x + g_big_box.get_width() + 1;
				}
			}
			small_box_y = 280;
		}
		if (!big_box_created) {
			big_box_created = true;
			big_box_x = 960 + (rand() * rand() + rand()) % 960;
			if (small_box_created) {
				if (intersect(small_box_x, small_box_x + g_small_box.get_width(), big_box_x, big_box_x + g_big_box.get_width())) {
					big_box_x = small_box_x + g_small_box.get_width() + 1;
				}
			}
			big_box_y = 200;
		}

		g_small_box.render(small_box_x, small_box_y);
		g_big_box.render(big_box_x, big_box_y);

		clip.x += pace;
		small_box_x -= pace;
		big_box_x -= pace;

		if (clip.x + clip.w > 2 * SCREEN_WIDTH)
			clip.x = 0;
		if (small_box_x + g_small_box.get_width() < 0) {
			small_box_created = false;
		}
		if (big_box_x + g_big_box.get_width() < 0) {
			big_box_created = false;
		}

		SDL_RenderPresent(g_renderer);
	}
	return LOOP_EXIT;
}

void main_loop()
{
	int i, j;
	bool main_loop_flag = false;
	SDL_Event e;
	LOOP_RETURNS game_loop_res;

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
			game_loop_res = game_loop();
			if (game_loop_res == LOOP_EXIT) {
				break;
			}
			if (game_loop_res == TO_START_MENU) {
				start_buttons_init_pos();
				for (i = 0; i < START_BUTTONS_TOTAL; ++i) {
					g_start_buttons[i].set_pressed(false);
					g_start_buttons[i].set_curr_sprite(BUTTON_SPRITE_MOUSE_OUT);
				}
			}
		}

		for (i = 0; i < START_BUTTONS_TOTAL; ++i)
			g_start_buttons[i].render(g_start_buttons[i].get_x(), g_start_buttons[i].get_y());

		SDL_RenderPresent(g_renderer);
	}
}