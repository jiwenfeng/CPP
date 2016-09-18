/*
 * Link : https://leetcode.com/problems/string-to-integer-atoi/
 */
 
class Solution {
public:
    int myAtoi(string str) {
      int sign = 1;
      int i = 0;
      while(str[i] == ' ')
      {
          i++;
      }
      if(str[i] == '-' )
      {
          i++;
          sign = -1;
      }
      else if(str[i] == '+')
      {
          i++;
      }
      double ret = 0;
      for(; i < str.length(); ++i)
      {
          if(str[i] > '9' || str[i] < '0')
          {
              break;
          }
          ret = ret * 10 + str[i] - '0';
      }
      
      ret *= sign;
      if(ret > INT_MAX)
      {
          return INT_MAX;
      }
      if(ret < INT_MIN)
      {
          return INT_MIN;
      }
      return (int)ret;
    }
};

int main()
{
    Solution s;
    printf("%d\n", s.myAtoi("12312"));
}
