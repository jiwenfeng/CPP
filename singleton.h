#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <mutex>
#include <iostream>
#include <chrono>

using namespace std;

template<typename T>
class CSingleton
{
public:
	CSingleton() = delete;
	CSingleton(const CSingleton &rhs) = delete;
	CSingleton &operator=(const CSingleton &rhs) = delete;
public:
	static T& GetInstance()
	{
		if(NULL == m_data)
		{
			std::unique_lock<std::mutex> lock(m_lock);
			if(NULL == m_data)
			{
				m_data = new T;
			}
		}
		return *m_data;
	}

private:
	static T *m_data;
	static std::mutex m_lock;
};

template<typename T> T* CSingleton<T>::m_data = NULL;
template<typename T> std::mutex CSingleton<T>::m_lock;

#endif
