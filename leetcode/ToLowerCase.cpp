#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    string toLowerCase(string str) {
        size_t sz = str.length();
        for(size_t i = 0; i < sz; ++i)
        {
            if(str[i] >= 'A' && str[i] <= 'Z')
            {
                str[i] += 32;
            }
        }
        return str;
    }
};
