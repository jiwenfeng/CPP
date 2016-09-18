/*
 * Link : https://leetcode.com/problems/reverse-integer/
 */

#include <iostream>

class Solution {
public:
    int reverse(int x) {
        double ret = 0;
        while(x != 0)
        {
            ret = ret * 10 + x % 10;
            x /= 10;
        }

        if(ret > INT_MAX || ret < INT_MIN)
        {
            return 0;
        }
        return (int)ret;
};

int main()
{
    	Solution s;
	printf("-123:%d\n", s.reverse(-123));
	printf("123:%d\n", s.reverse(123));
	printf("-1234:%d\n", s.reverse(-1234));
	printf("1534236469:%d\n", s.reverse(1534236469));
        return 0;
}
