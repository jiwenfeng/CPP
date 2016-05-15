#include <iostream>
#include <string>

using namespace std;

/*
 * Given a string, find the length of the longest substring without repeating characters
 *
 * Given "abcabcbb" ==> "abc", length is 3
 * Given "bbbbbbbb" ==> "b", length is 1
 *
 * */

class Solution
{
public:
	int lenghtOfLongestSubstring(string s)
	{
		int sz = s.size();
		if(0 == sz)
		{
			return 0;
		}
		int j, p, len = 1;
		for(int i = 1; i < sz; ++i)
		{
			int j = p;
			for(; j < i; ++j)
			{
				if(s[i] == s[j])
				{
					p = j + 1;
					break;
				}
			}
			len = max(len, i - p + 1);
		}
		return len;
	}
};
