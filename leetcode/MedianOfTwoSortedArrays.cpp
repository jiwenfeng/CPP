#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

/*
 * Link : https://leetcode.com/problems/median-of-two-sorted-arrays/
 */

class Solution
{
public:
	double findMedianSortedArrays(vector<int> &n1, vector<int> &n2)
	{
		vector<int> v;
		copy(n1.begin(), n1.end(), back_inserter(v));
		copy(n2.begin(), n2.end(), back_inserter(v));
		sort(v.begin(), v.end());
		size_t sz = v.size();
		if(sz % 2 == 0)
		{
			return double((v[sz / 2] + v[sz / 2 - 1]) / 2.0);
		}
		else
		{
			return double(v[sz / 2]);
		}
	}
};

int main()
{
	Solution s;
	vector<int> v1 = {}, v2 = {2, 3};
	cout<<s.findMedianSortedArrays(v1, v2)<<endl;
	return 0;
}
