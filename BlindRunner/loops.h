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

int random(int a, int b)
{
	if (b <= a)
		return a;
	return a + (rand() + rand() * rand()) % (b - a);
}

const int BOX_TOTAL = 52;
TBox boxes[BOX_TOTAL];

const int CURR_BOX_TOTAL = 3;
TBox curr_boxes[CURR_BOX_TOTAL];

void init_boxes()
{
	int i;
	char d, c;
	std::string s;
	for (i = 0, d = 'a'; i < 26; ++i, d++) {
		boxes[i] = TBox();
		s = "";
		s += d;
		if (!('a' <= d && d <= 'z'))
			s += '1';
		s = "images/alphabet/" + s + ".bmp";
		boxes[i].load_from_file(s);
	}

	for (i = 26, d = 'A'; i < BOX_TOTAL; ++i, d++) {
		boxes[i] = TBox();
		s = "";
		s += d;
		if (!('a' <= d && d <= 'z'))
			s += '1';
		s = "images/alphabet/" + s + ".bmp";
		boxes[i].load_from_file(s);
	}
}

void create_box(int index)
{
	int i;
	bool flag;

	curr_boxes[index] = boxes[random(0, BOX_TOTAL)];
	curr_boxes[index].set_active(true);

	while (1)
	{
		curr_boxes[index].set_x(random(SCREEN_WIDTH, SCREEN_WIDTH + 2 * SCREEN_WIDTH));
		flag = true;
		for (i = 0; i < CURR_BOX_TOTAL && flag; ++i) {
			if (i == index)
				continue;
			if (intersect(curr_boxes[i].get_x(), curr_boxes[i].get_x() + curr_boxes[i].get_width(),
				curr_boxes[index].get_x(), curr_boxes[index].get_x() + curr_boxes[index].get_width()))
				flag = false;
		}
		if (flag)
			break;
	}

	if (curr_boxes[index].get_height() == 200)
		curr_boxes[index].set_y(200);
	else
		curr_boxes[index].set_y(280);
}

LOOP_RETURNS game_loop()
{
	int pace = 5, i;
	std::pair<int, int> att_range;
	bool game_loop_flag = false, do_attack = false;
	SDL_Event e;
	SDL_Rect clip;

	clip.x = 0;
	clip.y = 0;
	clip.w = SCREEN_WIDTH;
	clip.h = SCREEN_HEIGHT;

	att_range.first = 100;
	att_range.second = 300;
	g_to_start_menu_button.set_pos(470, 470);	
	init_boxes();
	srand(time(NULL));
	for (i = 0; i < CURR_BOX_TOTAL; ++i) {
		create_box(i);
	}

	while (!game_loop_flag)
	{
		do_attack = false;
		while (SDL_PollEvent(&e) != 0) 
		{
			if (e.type == SDL_QUIT) {
				game_loop_flag = true;
				return LOOP_EXIT;
			}
			else {
				if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						do_attack = true;
						break;
					}
				}
			}
			g_to_start_menu_button.handle_event(&e);
		}

		if (g_to_start_menu_button.get_curr_sprite() == BUTTON_SPRITE_MOUSE_DOWN) {
			return TO_START_MENU;
		}

		if (do_attack) {
			for (i = 0; i < CURR_BOX_TOTAL; ++i) {
				if (att_range.first <= curr_boxes[i].get_x() && curr_boxes[i].get_x() <= att_range.second)
					curr_boxes[i].set_active(false);
			}
		}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		g_inf_background.render(0, 0, &clip);
		g_to_start_menu_button.render(g_to_start_menu_button.get_x(), g_to_start_menu_button.get_y());
		for (i = 0; i < CURR_BOX_TOTAL; ++i)
			curr_boxes[i].render(curr_boxes[i].get_x(), curr_boxes[i].get_y());

		clip.x += pace;
		for (i = 0; i < CURR_BOX_TOTAL; ++i)
			curr_boxes[i].set_x(curr_boxes[i].get_x() - pace);

		if (clip.x + clip.w > 2 * SCREEN_WIDTH)
			clip.x = 0;
		for (i = 0; i < CURR_BOX_TOTAL; ++i) {
			if (curr_boxes[i].get_x() < att_range.first && curr_boxes[i].get_active()) {
				return TO_START_MENU;
			}
			if (curr_boxes[i].get_x() + curr_boxes[i].get_width() < 0) {
				create_box(i);
			}
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