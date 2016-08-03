#include "utf8.h"
#include <string.h>

int utf_len(const char *s)
{
	size_t len = 0;
	char c = '\0';
	while('\0' != (c = *s))
	{
		if(0 == (0x80 & c))
		{
			len++;
			s++;
			continue;
		}
		else
		{
			if(0 == (0x40 &c))
			{
				s++;
				continue;
			}
			unsigned char l = ((0xF0 & c) >> 4);
			switch(l)
			{
				case 0xF:
					s += 4;
					break;
				case 0xE:
					s += 3;
					break;
				case 0xC:
					s += 2;
					break;
			}
			len++;
		}
	}
	return len;
}

// chinese 是否都是中文
int utf_len(const char *s, bool &chinese)
{
	size_t len = 0;
	char c = '\0';
	while('\0' != (c = *s))
	{
		if(0 == (0x80 & c))
		{
			len++;
			s++;
			chinese = false;
			return 0;
			//continue;
		}
		else
		{
			if(0 == (0x40 &c))
			{
				s++;
				chinese = false;
				return 0;
//				continue;
			}
			unsigned char l = ((0xF0 & c) >> 4);
			switch(l)
			{
				case 0xF:
					s += 4;
					break;
				case 0xE:
					s += 3;
					break;
				case 0xC:
					s += 2;
					break;
			}
			len++;
		}
	}
	chinese = true;
	return len;
}
