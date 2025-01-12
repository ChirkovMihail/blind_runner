#pragma once

enum TButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class TButton : public TTexture
{
public:
	TButton();

	void set_pos(int x, int y);
	void handle_event(SDL_Event* e);
	void set_pressed(bool flag);
	void set_curr_sprite(TButtonSprite sp);

	int get_x();
	int get_y();
	TButtonSprite get_curr_sprite();
	bool get_pressed();

private:
	SDL_Point pos;
	TButtonSprite curr_sprite;
	bool pressed;
};

TButton::TButton()
{
	pos.x = 0;
	pos.y = 0;
	curr_sprite = BUTTON_SPRITE_MOUSE_OUT;
	pressed = false;
}

void TButton::set_curr_sprite(TButtonSprite sp)
{
	curr_sprite = sp;
}

void TButton::set_pressed(bool flag)
{
	pressed = flag;
}

bool TButton::get_pressed()
{
	return pressed;
}

void TButton::set_pos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

int TButton::get_x()
{
	return pos.x;
}

int TButton::get_y()
{
	return pos.y;
}

TButtonSprite TButton::get_curr_sprite()
{
	return curr_sprite;
}

void TButton::handle_event(SDL_Event * e)
{
	if (e->type != SDL_MOUSEBUTTONDOWN) {
		curr_sprite = BUTTON_SPRITE_MOUSE_OUT;
		return;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);

	if (pos.x < x && x < pos.x + get_width() && pos.y < y && y < pos.y + get_height()) {
		curr_sprite = BUTTON_SPRITE_MOUSE_DOWN;
	}
	else {
		curr_sprite = BUTTON_SPRITE_MOUSE_OUT;
	}
}