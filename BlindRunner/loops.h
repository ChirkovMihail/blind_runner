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
		boxes[i].set_char(d);
	}

	for (i = 26, d = 'A'; i < BOX_TOTAL; ++i, d++) {
		boxes[i] = TBox();
		s = "";
		s += d;
		if (!('a' <= d && d <= 'z'))
			s += '1';
		s = "images/alphabet/" + s + ".bmp";
		boxes[i].load_from_file(s);
		boxes[i].set_char(d);
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
	int pace = 2, i, curr_run_frame = 0, run_pace = 15;
	char att_c;
	std::pair<int, int> att_range;
	bool game_loop_flag = false, do_attack = false;
	SDL_Event e;
	SDL_Rect clip;

	clip.x = 0;
	clip.y = 0;
	clip.w = SCREEN_WIDTH;
	clip.h = SCREEN_HEIGHT;

	att_range.first = 100 + g_man[0].get_width();
	att_range.second = att_range.first + 200;
	g_to_start_menu_button.set_pos(415, 470);	
	init_boxes();
	srand(time(NULL));
	for (i = 0; i < CURR_BOX_TOTAL; ++i) {
		create_box(i);
	}

	while (!game_loop_flag)
	{
		att_c = '0';
		do_attack = false;
		while (SDL_PollEvent(&e) != 0) 
		{
			if (e.type == SDL_QUIT) {
				game_loop_flag = true;
				return LOOP_EXIT;
			}
			else {
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.mod & KMOD_SHIFT) {
						if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) {
							do_attack = true;
							att_c = 'A' + (e.key.keysym.sym - SDLK_a);
						}
					}
					else {
						if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_z) {
							do_attack = true;
							att_c = 'a' + (e.key.keysym.sym - SDLK_a);
						}
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
				if (att_range.first <= curr_boxes[i].get_x() && curr_boxes[i].get_x() <= att_range.second 
					&& att_c == curr_boxes[i].get_char())
					curr_boxes[i].set_active(false);
			}
		}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		//render

		g_inf_background.render(0, 0, &clip);
		g_to_start_menu_button.render(g_to_start_menu_button.get_x(), g_to_start_menu_button.get_y());
		for (i = 0; i < CURR_BOX_TOTAL; ++i) {
			if (curr_boxes[i].get_active())
				curr_boxes[i].render(curr_boxes[i].get_x(), curr_boxes[i].get_y());
		}
		g_man[curr_run_frame / run_pace].render(100, 130);

		//shift

		curr_run_frame = (curr_run_frame + 1) % (MAN_FRAMES_TOTAL * run_pace);
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

		g_inf_background.render(0, 0);

		for (i = 0; i < ACTIVE_BUTTONS; ++i) {
			if (g_start_buttons[i].get_curr_sprite() == BUTTON_SPRITE_MOUSE_DOWN && g_start_buttons[i].get_pressed() == false) {
				for (j = 0; j < ACTIVE_BUTTONS; ++j)
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
				for (i = 0; i < ACTIVE_BUTTONS; ++i) {
					g_start_buttons[i].set_pressed(false);
					g_start_buttons[i].set_curr_sprite(BUTTON_SPRITE_MOUSE_OUT);
				}
			}
		}

		for (i = 0; i < ACTIVE_BUTTONS; ++i)
			g_start_buttons[i].render(g_start_buttons[i].get_x(), g_start_buttons[i].get_y());

		SDL_RenderPresent(g_renderer);
	}
}