#include <iostream>
#include <vector>
using namespace std;

/*
 * input [2, 7, 11, 15, 19] 9
 * output [1, 2]
 * */

class Solution
{
public:
	vector<int> addTwoNumber(vector<int> &nums, int target)
	{
		size_t sz = nums.size();
		vector<int> result;
		for(int i = 0; i < sz; ++i)
		{
			for(int j = i + 1; j < sz; ++j)
			{
				if(nums[i] + nums[j] == target)
				{
					result.push_back(i);
					result.push_back(j);
				}
			}
		}
		return result;
	}
};
