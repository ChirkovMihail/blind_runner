#pragma once

void start_buttons_animation(int index)
{
	int pace = 10, i;

	while (g_start_buttons[index].get_y() + g_start_buttons[index].get_height() > 0) {
		g_start_buttons[index].set_pos(g_start_buttons[index].get_x(), g_start_buttons[index].get_y() - pace);

		for (i = 0; i < START_BUTTONS_TOTAL; ++i)
			if (i != index) {
				if (g_start_buttons[i].get_y() + pace < SCREEN_HEIGHT + pace + 1)
					g_start_buttons[i].set_pos(g_start_buttons[i].get_x(), g_start_buttons[i].get_y() + pace);
			}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		g_inf_background.render(0, 0);

		for (i = 0; i < START_BUTTONS_TOTAL; ++i)
			g_start_buttons[i].render(g_start_buttons[i].get_x(), g_start_buttons[i].get_y());

		SDL_RenderPresent(g_renderer);
	}
}
