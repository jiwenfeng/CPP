#include <algorithm>
#include <vector>

class Solution {
	public:
		int maximumSwap(int num) {
			vector<int> v;
			int n = num;
			while(n > 0)
			{
				v.push_back(n % 10);
				n /= 10;
			}
			vector<int> v1 = v;
			sort(v.begin(), v.end());
			int sz = v.size();
			for(int i = sz - 1; i >= 0; --i)
			{
				if(v1[i] > v[i])
				{
					return num;
				}
				if(v1[i] < v[i])
				{
					int  k = 0;
					for(int j = i - 1; j >= 0; --j)
					{
						if(v1[j] == v[i])
						{
							k = j;
						}
					}
					v1[k] = v1[i];
					v1[i] = v[i];
					num = 0;
					for(int j = sz - 1; j >= 0; --j)
					{
						num = num * 10 + v1[j];
					}
					return num;
				}
			}
			return num;
		}
};
