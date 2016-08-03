#ifndef __RECT_H__
#define __RECT_H__

#include "postion.h"
#include <stdio.h>

class rect
{
public:
    rect();

    rect(const rect& rhs);

    rect(int x1, int y1, int x2, int y2);

    virtual ~rect();

    void set(int x1, int y1, int x2, int y2);

    bool operator==(const rect& rhs) const;

    bool operator!=(const rect& rhs) const;
    
    bool operator<(const rect& rhs) const;

    postion& get_top_left();

    postion& get_bot_right();


	bool is_in_range(const postion &p) const;
	bool is_in_range(int x, int y) const;

    const postion& get_top_left() const;

    const postion& get_bot_right() const;

    int width() const;

    int height() const;

    int size() const;

    const void print() const 
    {
        //printf("[(%4d %4d)(%4d %4d)] ", top_left.x, top_left.y, bot_right.x, bot_right.y);
    }

public:
    postion top_left;
    postion bot_right;
};

#endif
