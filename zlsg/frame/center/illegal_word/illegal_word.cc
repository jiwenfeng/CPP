#include "illegal_word.h"
#include "utf8.h"

illegal_word_mgr::illegal_word_mgr()
{
}

illegal_word_mgr::~illegal_word_mgr()
{
	clear_illegal_words();
}

void illegal_word_mgr::add_illegal_word(const string &str)
{
	if(find(_words.begin(), _words.end(), str) != _words.end())
	{
		return ; 
	}
	_words.push_back(str);
}

void illegal_word_mgr::remove_illegal_word(const string &str)
{
	_words.erase(remove(_words.begin(), _words.end(), str), _words.end());
}

void illegal_word_mgr::add_illegal_words(vector<string> &v)
{
	for(size_t i = 0; i < v.size(); ++i)
	{
		add_illegal_word(v[i]);
	}
}

void illegal_word_mgr::clear_illegal_words()
{
	_words.clear();
}

bool illegal_word_mgr::has_illegal_word(const string &str)
{
	return find(_words.begin(), _words.end(), str) != _words.end();
}

void illegal_word_mgr::replace_illegal_word(string &str, char rep)
{
	while(1)
	{
		bool flag = false;
		for(size_t i = 0; i < _words.size(); ++i)
		{
			size_t n = str.find(_words[i]);
			if(n != string::npos)
			{
				int len = utf_len(_words[i].c_str());
				str.erase(n, _words[i].length());
				str.insert(n, len, rep);
				flag = true;
			}
		}
		if(!flag)
		{
			break;
		}
	}
}
