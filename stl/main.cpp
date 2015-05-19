#include <iostream>
#include "wf_vector.h"
#include <sys/time.h>
#include <vector>

using namespace wf;

class Test
{
public:
	Test(int v) : _v(v) {}
	friend std::ostream &operator<<(std::ostream &os, const Test &t)
	{
		os<<t._v;
		return os;
	}

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
#ifdef __TEST_MINE
	vector<Test> v;
	for(int i = 0; i < 10; ++i)
	{
		v.push_back(Test(i));
	}
	vector<Test> v1(v);
	vector<Test>::iterator i;
	for(i = v1.begin(); i != v1.end(); ++i)
	{
		std::cout<<*i<<std::endl;
	}
#else
	std::vector<Test> v;
	for(int i = 0; i < 10; ++i)
	{
		v.push_back(Test(i));
	}
#endif
	return 0;
}
