#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <stdlib.h>

namespace base_lib{

template<typename T>
class auto_ptr
{
public:
	auto_ptr() : m_auto_ptr(0) {}
    auto_ptr(T* ptr) : m_auto_ptr(ptr) {}
    auto_ptr(const auto_ptr& obj) : m_auto_ptr(obj.get()) {}
	~auto_ptr() { reset (0); }

    auto_ptr& operator= (const auto_ptr& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

	T* get() const { return m_auto_ptr; }
    T* operator() () { return get(); }

	void reset(T* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            delete m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
	T* m_auto_ptr;
};


template<typename T>
class auto_array
{
public:
    auto_array() : m_auto_ptr(0) {}
    auto_array(T* ptr) : m_auto_ptr(ptr) {}
    auto_array(const auto_array<T>& obj) : m_auto_ptr(obj.m_auto_ptr) {}
    ~auto_array() { reset (0); }

    auto_array& operator= (const auto_array<T>& obj)
    {
        m_auto_ptr = obj.get();
        return *this;
    }

    T* get() const { return m_auto_ptr; }
    T* operator()() const { return get(); }

    void reset(T* ptr)
    {
        if (m_auto_ptr == ptr){
            return;
        }

        if (m_auto_ptr){
            delete [] m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
    T* m_auto_ptr;
};

class auto_free
{
public:
    auto_free() : m_auto_ptr(0) {}
    auto_free(void* ptr) : m_auto_ptr(ptr) {}
    auto_free(const auto_free& obj) : m_auto_ptr(obj.get()) {}
    ~auto_free() { reset (0); }

    auto_free& operator= (const auto_free& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

    void* get() const { return m_auto_ptr; }
    void* operator() () { return get(); }

    void reset(void* ptr)
    {
        if (m_auto_ptr == ptr){
            return;
        }

        if (m_auto_ptr){
            free(m_auto_ptr);
        }

        m_auto_ptr = ptr;
    }

private:
    void* m_auto_ptr;
};

typedef auto_array<char>         auto_char_A;
typedef auto_array<wchar_t>      auto_char_W;

};


template<typename T, int X = 0> class singleton 
{
private:
	singleton() {}
	singleton(const singleton&) {}
	~singleton() {}
	singleton& operator= (const singleton &) { return *this; } 

public:
	static T* instance()
	{
		static base_lib::auto_ptr<T> autoptr;
 		if(autoptr.get() == 0)
		{
 			autoptr.reset(new T);
		}
 		return autoptr.get();
	}
};


#endif
