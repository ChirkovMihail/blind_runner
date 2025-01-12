#pragma once

void game_loop()
{
	int pace = 5;
	bool game_loop_flag = false;
	SDL_Event e;
	SDL_Rect clip;

	clip.x = 0;
	clip.y = 0;
	clip.w = SCREEN_WIDTH;
	clip.h = SCREEN_HEIGHT;

	while (!game_loop_flag)
	{
		while (SDL_PollEvent(&e) != 0) 
		{
			if (e.type == SDL_QUIT)
				game_loop_flag = true;
		}

		SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(g_renderer);

		g_inf_background.render(0, 0, &clip);

		clip.x += pace;
		if (clip.x + clip.w > 2 * SCREEN_WIDTH)
			clip.x = 0;

		SDL_RenderPresent(g_renderer);
	}
}