#include <math>

class Solution {
public:
    int fib(int N) {
        return 1 / sqrt(5) * (pow((1 + sqrt(5))/2, N) - pow((1 - sqrt(5))/2, N));
    }
};
