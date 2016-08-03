#include "rect.h"

rect::rect()
{
}

rect::rect(int x1, int y1, int x2, int y2)
{
    set(x1, y1, x2, y2);
}

rect::~rect()
{
}

rect::rect(const rect &rhs)
{
	set(rhs.top_left.x, rhs.top_left.y, rhs.bot_right.x, rhs.bot_right.y);
}

void rect::set(int x1, int y1, int x2, int y2)
{
    top_left.x = x1;
    top_left.y = y1;
    bot_right.x = x2;
    bot_right.y = y2;
}

bool rect::operator==(const rect& rhs) const
{
    return !operator != (rhs);
}

bool rect::operator!=(const rect& rhs) const
{
    if (this == &rhs){
        return false;
    }

    if (top_left != rhs.top_left || bot_right != rhs.bot_right) {
        return true;
    }

    return false;
}
    
bool rect::operator<(const rect& rhs) const
{
    if (this == &rhs){
        return false;
    }

    if (top_left < rhs.top_left){
        return true;
    } else if (rhs.top_left < top_left){
        return false;
    }

    if (bot_right < rhs.bot_right){
        return true;
    } else if (rhs.bot_right < bot_right){
        return false;
    }

    return false;
}

postion& rect::get_top_left()
{
    return top_left;
}

postion& rect::get_bot_right()
{
    return bot_right;
}

int rect::width() const
{
    return bot_right.x - top_left.x;
}

int rect::height() const
{
    return bot_right.y - top_left.y;
}

int rect::size() const
{
    return width() * height();
}

bool rect::is_in_range(int x, int y) const
{
	return (top_left.x <= x && x <= bot_right.x) && (top_left.y <= y && y <= bot_right.y);
}

bool rect::is_in_range(const postion &p) const
{
	return is_in_range(p.x, p.y);
}
