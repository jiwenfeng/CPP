#include <iostream>
#include <vector>
using namespace std;

/*
 * Link : https://leetcode.com/problems/two-sum/ 
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
					return result;
				}
			}
		}
		return result;
	}
};

int main()
{
	return 0;
}
