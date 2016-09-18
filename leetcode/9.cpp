/*
 * Link : https://leetcode.com/problems/palindrome-number/
 */
#include <stdio.h>

class Solution {
public:
    bool isPalindrome(int x) {
        if(x < 0)
        {
            return false;
        }
        int n = 1;
        int tmp = x;
        while(tmp >= 10)
        {
            tmp /= 10;
            n *= 10;
        }
        while(x != 0)
        {
            if((x % 10) != (x / n))
            {
                return false;
            }
            x %= n;
            x /= 10;
            n /= 100;
        }
        return true;
    }
};

int main()
{
  Solution s;
  s.isPalindrome(1001);
  return 0;
}
