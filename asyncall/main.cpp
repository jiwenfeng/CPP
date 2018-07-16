#include "async.h"

void T1(int a, int b, int c)
{
	std::cout<<__func__<<":"<<"a:"<<a<<", b:"<<b<<", c:"<<c<<std::endl;
}

void T2(int a, int b)
{
	std::cout<<__func__<<":"<<"a:"<<a<<", b:"<<std::endl;
}

int main()
{
	int id1 = Async::getInstance().reg<std::function<void(int, int, int)>>(std::bind(T1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	
	Async::getInstance().call(id1, 1, 2, 3);

	int id2 = Async::getInstance().reg<std::function<void()>>(std::bind(T1, 1, 2, 3));
	Async::getInstance().call(id2);


	int id3 = Async::getInstance().reg<std::function<void(int)>>(std::bind(T1, 1, 2, std::placeholders::_1));
	Async::getInstance().call(id3, 3);
	return 0;
}
