#include <iostream>
#include "wf_vector.h"
#include <sys/time.h>
#include <vector>

using namespace wf;

class Test
{
public:
	Test() {}
	Test(int v) : _v(v) {}
private:
	int _v;
};

int get_tick()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


int main()
{
	vector<int> v;
	int t1 = get_tick();
	for(int i = 0; i < 10000000; ++i)
	{
		v.push_back(i);
	}
	int t2 = get_tick();

	printf("t2 - t1 = %d\n", t2 - t1);

	std::vector<int> std_v;
	int t3 = get_tick();
	for(int i = 0; i < 10000000; ++i)
	{
		std_v.push_back(i);
	}
	int t4 = get_tick();
	printf("t4 - t3 = %d\n", t4 - t3);
#if 0
	vector<int>::iterator it;
	for(it = v.begin(); it != v.end(); ++it)
	{
		std::cout<<*it<<std::endl;
	}
#endif
	return 0;
}
