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

LOOP_RETURNS game_loop()
{
	int pace = 5, i;
	bool game_loop_flag = false;
	SDL_Event e;
	SDL_Rect clip;

	clip.x = 0;
	clip.y = 0;
	clip.w = SCREEN_WIDTH;
	clip.h = SCREEN_HEIGHT;

	const int BOX_TOTAL = 52;
	TBox boxes[BOX_TOTAL];
	TBox box1, box2;
	bool box1_created = false, box2_created = false;

	g_to_start_menu_button.set_pos(470, 470);

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

	srand(time(NULL));

	TBox test = boxes[0];
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

		if (!box1_created) {
			box1_created = true;
			box1 = boxes[random(0, BOX_TOTAL)];
			box1.set_x(random(960, 960 + 960));
			if (box2_created) {
				if (intersect(box1.get_x(), box1.get_x() + box1.get_width(), box2.get_x(), box2.get_x() + box2.get_width())) {
					box1.set_x(box2.get_x() + box2.get_width() + 5);
				}
			}
			if (box1.get_height() == 200)
				box1.set_y(200);
			else
				box1.set_y(280);
		}

		if (!box2_created) {
			box2_created = true;
			box2 = boxes[random(0, BOX_TOTAL)];
			box2.set_x(random(960, 960 + 960));
			if (box1_created) {
				if (intersect(box1.get_x(), box1.get_x() + box1.get_width(), box2.get_x(), box2.get_x() + box2.get_width())) {
					box2.set_x(box1.get_x() + box1.get_width() + 5);
				}
			}
			if (box2.get_height() == 200)
				box2.set_y(200);
			else
				box2.set_y(280);
		}

		box1.render(box1.get_x(), box1.get_y());
		box2.render(box2.get_x(), box2.get_y());

		clip.x += pace;
		box1.set_x(box1.get_x() - pace);
		box2.set_x(box2.get_x() - pace);

		if (clip.x + clip.w > 2 * SCREEN_WIDTH)
			clip.x = 0;
		if (box1.get_x() + box1.get_width() < 0) {
			box1_created = false;
		}
		if (box2.get_x() + box2.get_width() < 0) {
			box2_created = false;
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