#pragma once

class TBox : public TTexture
{
public:
	TBox();

	void set_pos(int x, int y);
	void set_x(int x);
	void set_y(int y);

	int get_x();
	int get_y();
	char get_char();

private:
	SDL_Point pos;
	char c;
};

char TBox::get_char()
{
	return c;
}

void TBox::set_x(int x)
{
	pos.x = x;
}

void TBox::set_y(int y)
{
	pos.y = y;
}

TBox::TBox()
{
	pos.x = 0;
	pos.y = 0;
	c = 0;
}

void TBox::set_pos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

int TBox::get_x()
{
	return pos.x;
}

int TBox::get_y()
{
	return pos.y;
}