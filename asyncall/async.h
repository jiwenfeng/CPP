#include <functional>
#include <typeinfo>
#include <iostream>
#include <map>

static int gIndex = 0;

class Base
{};

template<class T>
class Function : public Base
{
public:
	Function(const T &f)
	{
		m_func = f;
	}

	template<typename ...Args>
	void operator()(Args ...args)
	{
		m_func(std::forward<Args>(args)...);
	}

private:
	T m_func;
};

class Async
{
public:
	Async() {}
	Async(const Async &rhs) = delete;
	Async(Async &&rhs) = delete;
	Async &operator=(const Async &rhs) = delete;
	Async &operator=(Async &&rhs) = delete;

public:
	static Async &getInstance()
	{
		static Async inst;
		return inst;
	}

public:
	template<class T>
	int reg(const T &func)
	{
		Base *f = new Function<T>(func);
		m_funcs.insert(std::make_pair(gIndex, f));
		return gIndex++;
	}

	template<typename ...Args>
	void call(int id, Args ...args)
	{
		std::map<int, Base *>::iterator i = m_funcs.find(id);
		if(i == m_funcs.end())
		{
			return;
		}

		Function<std::function<void(Args...)> > *f = static_cast<Function<std::function<void(Args ...)> >*>(i->second);

		if(nullptr == f)
		{
			return;
		}

		(*f)(std::forward<Args>(args)...);
	}

private:
	std::map<int, Base *> m_funcs;
};

