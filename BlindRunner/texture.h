#pragma once

class TTexture
{
public:
	TTexture();
	~TTexture();

	bool load_from_file(std::string path, int r, int g, int b);
	void free();
	void set_color(Uint8 red, Uint8 green, Uint8 blue);
	void set_blendmode(SDL_BlendMode blending);
	void set_alpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0,
		SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int get_width();
	int get_height();

private:
	SDL_Texture* texture;
	int width;
	int height;
};

TTexture::TTexture()
{
	texture = NULL;
	width = 0;
	height = 0;
}

TTexture::~TTexture()
{
	free();
}

bool TTexture::load_from_file(std::string path, int r = 0, int g = 255, int b = 255)
{
	//delete preexisting
	free();

	if (!(r >= 0 && r < 256 && g >= 0 && g < 256 && b >= 0 && b < 256)) {
		std::cout << "Wrong mapRBG colors " << path << ' ' << '\n';
		return false;
	}

	SDL_Texture* new_texture = NULL;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == NULL) {
		std::cout << "Unable to load image:  SDL_image Error: " << path << ' ' << IMG_GetError() << '\n';
		return false;
	}

	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, r, g, b));
	new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
	if (new_texture == NULL) {
		std::cout << "Unable to create texture from surface: SDL_Error : " << path << ' ' << SDL_GetError() << '\n';
		return false;
	}

	width = loaded_surface->w;
	height = loaded_surface->h;
	SDL_FreeSurface(loaded_surface);
	texture = new_texture;
	return texture != NULL;
}

void TTexture::free()
{
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void TTexture::set_color(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void TTexture::set_blendmode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture, blending);
}

void TTexture::set_alpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

void TTexture::render(int x, int y, SDL_Rect* clip, double angle,
	SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect render_quad = { x, y, width, height };
	if (clip != NULL) {
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}
	SDL_RenderCopyEx(g_renderer, texture, clip, &render_quad, angle, center, flip);
}

int TTexture::get_width()
{
	return width;
}

int TTexture::get_height()
{
	return height;
}