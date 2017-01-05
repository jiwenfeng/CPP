#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
	public:
		vector<vector<int>> threeSum(vector<int>& nums) {
			vector<vector<int> > v;
			if(nums.size() < 3)
			{
				return v;
			}
			sort(nums.begin(), nums.end());
			size_t n = nums.size();
			for(size_t i = 0; i < n - 2; ++i)
			{
				if(i > 0 && nums[i] == nums[i - 1])
				{
					continue;
				}
				if(nums[i] + nums[i + 1] + nums[i + 2] > 0)
				{
					 break;
				}
				if(nums[i] + nums[n - 2] + nums[n - 1] < 0)
				{
					continue;
				}
				for(int j = i + 1; i < n - 1; ++j)
				{

					if(j > i + 1 && nums[j] == nums[j - 1])
					{
						continue;
					}
					if(nums[i] + nums[j] + nums[j + 1] > 0)
					{
						break;
					}
					if(nums[i] + nums[j] + nums[n - 1] < 0)
					{
						continue;
					}
					size_t right = n - 1;
					while(right > j)
					{
						int sum = nums[i] + nums[j] + nums[right];
						if(sum < 0)
						{
							j++;
						}
						else if(sum > 0)
						{
							right--;
						}
						else
						{
							v.push_back(vector<int>{nums[i], nums[j], nums[right]});
							do
							{
								right--;
							}while(nums[right] == nums[right + 1] && right > j);
						}
					}
				}
			}
			return v;
		}
};

int main()
{
	vector<int> v = {-4,-1,-4,0,2,-2,-4,-3,2,-3,2,3,3,-4};
	Solution s;
	vector<vector<int>> ret = s.threeSum(v);
	for(int i = 0; i < ret.size(); ++i)
	{
		cout<<"[";
		for(int j = 0; j < 3; ++j)
		{
			cout<<ret[i][j];
			if(j < 2)
			{
				cout<<",";
			}
		}
		cout<<"]"<<endl;
	}
	return 0;
}
