#ifndef __ILLEGAL_WORD_H__
#define __ILLEGAL_WORD_H__

#include "define.h"
#include "comm_define.h"


class illegal_word_mgr
{
public:
	illegal_word_mgr();
	~illegal_word_mgr();
public:
	void add_illegal_word(const string &str);
	void remove_illegal_word(const string &str);
	void add_illegal_words(vector<string> &v);
	void clear_illegal_words();
	bool has_illegal_word(const string &str);
	void replace_illegal_word(string &str, char rep = '*');

private:
	std::vector<string> _words;

};
#endif

