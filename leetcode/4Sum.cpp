#include <iostream>
#include <algorithm>
#include <vector>
#include <sys/time.h>
#include <iterator>

using namespace std;


class Solution
{
	public:
		vector<vector<int> > FourSum(vector<int> &nums, int target)
		{
			vector<vector<int> >v;
			v.reserve(nums.size());
			if(nums.size() < 4)
			{
				return v;
			}
			sort(nums.begin(), nums.end());
			size_t n = nums.size();
			for(size_t i = 0; i < n; ++i)
			{
				if(i > 0 && nums[i] == nums[i - 1])
				{
					continue;
				}
				if(nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target)
				{
					break;
				}
				if(nums[i] + nums[n - 3] + nums[n - 2] + nums[n - 1] < target)
				{
					continue;
				}
				for(size_t j = i + 1; j < n - 2; ++j)
				{

					if(j > i + 1 && nums[j] == nums[j - 1])
					{
						continue;
					}
					if(nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target)
					{
						break;
					}
					if(nums[i] + nums[j] + nums[n - 2] + nums[n - 1] < target)
					{
						continue;
					}
					size_t left = j + 1;
					size_t right = n - 1;
					while(left < right)
					{
						int sum = nums[i] + nums[j] + nums[left] + nums[right];
						if(sum > target)
						{
							right--;
						}
						else if(sum < target)
						{
							left++;
						}
						else
						{
							v.push_back(vector<int>{nums[i], nums[j], nums[left], nums[right]});
							do{
								left++;
							} while(nums[left] == nums[left - 1] && left < right);
							do {
								right--;
							} while(nums[right] == nums[right + 1] && left < right);
						}
					}
				}
			}
			return v;
		}
};

int main()
{
	vector<int> v = {0,0,0,0};
	sort(v.begin(), v.end());
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	cout<<endl;

	Solution s;
	vector<vector<int> > ret = s.FourSum(v, 0);
	for(int i = 0; i < ret.size(); ++i)
	{
		cout<<"[";
		for(int j = 0; j < ret[i].size(); ++j)
		{
			cout<<ret[i][j]<<",";
		}
		cout<<"]"<<endl;
	}

	return 0;
}
