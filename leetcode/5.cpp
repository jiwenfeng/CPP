#include <iostream>
#include <string>

/*
 * Link : https://leetcode.com/problems/longest-palindromic-substring/
 */


class Solution
{
public:
	std::string longestPalindrome(std::string s)
	{
		std::string ret;
		int sz = s.length();
		for(size_t i = 0; i < sz; ++i)
		{
			std::string tmp = GetPalindrome(s, i, sz);
			if(tmp.empty())
			{
				continue;
			}
			if(ret.length() < tmp.length())
			{
				ret = tmp;
			}
		}
		return ret;
	}

private:
	std::string GetPalindrome(const std::string &str, int s, int sz)
	{
		int i = 0;
		int j = sz - 1;
		for(; i <= (sz - s) / 2;)
		{
			if(str[s + i] != str[j])
			{
				i = 0;
				--sz;
				j = sz - 1;
			}
			else
			{
				i++;
				j--;
			}
		}
		return str.substr(s, sz - s);
	}
};

int main()
{
	Solution s;
	std::cout<<s.longestPalindrome("a")<<std::endl;
	std::cout<<s.longestPalindrome("bb")<<std::endl;
	std::cout<<s.longestPalindrome("abb")<<std::endl;
	std::cout<<s.longestPalindrome("aaabaaaa")<<std::endl;


	std::cout<<s.longestPalindrome("civilwartestingwhetherthatnaptionoranynartionsoconceivedandsodedicatedcanlongendureWeareqmetonagreatbattlefiemldoftzhatwarWehavecometodedicpateaportionofthatfieldasafinalrestingplaceforthosewhoheregavetheirlivesthatthatnationmightliveItisaltogetherfangandproperthatweshoulddothisButinalargersensewecannotdedicatewecannotconsecratewecannothallowthisgroundThebravelmenlivinganddeadwhostruggledherehaveconsecrateditfaraboveourpoorponwertoaddordetractTgheworldadswfilllittlenotlenorlongrememberwhatwesayherebutitcanneverforgetwhattheydidhereItisforusthelivingrathertobededicatedheretotheulnfinishedworkwhichtheywhofoughtherehavethusfarsonoblyadvancedItisratherforustobeherededicatedtothegreattdafskremainingbeforeusthatfromthesehonoreddeadwetakeincreaseddevotiontothatcauseforwhichtheygavethelastpfullmeasureofdevotionthatweherehighlyresolvethatthesedeadshallnothavediedinvainthatthisnationunsderGodshallhaveanewbirthoffreedomandthatgovernmentofthepeoplebythepeopleforthepeopleshallnotperishfromtheearth")<<std::endl;

	return 0;
}
