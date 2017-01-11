
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <list>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
	auto producer = [](mutex &m, condition_variable &cv, list<int> &v) {
		int i = 0;
		while(true)
		{
			unique_lock<mutex> lock(m);
			while (v.size() == 5)
			{
				cv.notify_all();
				cv.wait(lock);
			}
			++i;
			v.push_back(i);
			cout << "[" << this_thread::get_id() << "] Send:" << i << endl;
			lock.unlock();
			cv.notify_all();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	};
  auto customer = [](mutex &m, condition_variable &cv, list<int> &v) {
		while(true)
		{
			unique_lock<mutex> lock(m);
			while (v.empty())
			{
				cv.notify_all();
				cv.wait(lock);
			}
			cout << "[" << this_thread::get_id() << "] Get:" << *v.begin() << endl;
			v.pop_front();
			lock.unlock();
			cv.notify_all();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	};
  mutex m;
  condition_variable cv;
  vector<thread> v;
  list<int> l;
  v.push_back(thread(producer, std::ref(m), std::ref(cv), std::ref(l)));
  v.push_back(thread(customer, std::ref(m), std::ref(cv), std::ref(l)));
  for (vector<thread>::iterator itr = v.begin(); itr != v.end(); ++itr)
  {
  	itr->join();
  }
  return 0;
}
