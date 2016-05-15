#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
	string longestPalindrome(string s) {
		size_t sz = s.length();
		if(sz == 1)
		{
			return s;
		}
		for(size_t i = 0; i < sz; ++i)
		{
			int flag = 0;
			size_t n = (sz - i) / 2;
			for(size_t j = i; j < n; ++j)
			{
				if(s[j] != s[sz - i - j - 1])
				{
					flag = 1;
					break;
				}
				flag = 2;
			}
			if(flag == 2)
			{
				return s.substr(i, sz - i + 1);
			}
		}
		return "";
	}
};

int main()
{
	const char *str = "abcdcba";
	Solution s;
	cout<<str<<"==>"<<s.longestPalindrome(str)<<endl;
	return 0;
}
