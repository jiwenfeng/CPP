#ifndef __HEAP_H__
#define __HEAP_H__

#include <iterator>

template<class T>
class HeapIterator
{

public:
	typedef typename std::iterator_traits<T>::value_type value_type;
	typedef typename std::iterator_traits<T>::pointer pointer;
	typedef typename std::iterator_traits<T>::reference reference;
	typedef typename std::iterator_traits<T>::iterator_category iterator_category;
	typedef typename std::iterator_traits<T>::difference_type difference_type;

public:
	HeapIterator(const T &value = T()) : _value(value)
	{
	}

	~HeapIterator() {}

	value_type operator++(int)
	{
		T v = *_value;
		_value++;
		return v;
	}

	reference operator++()
	{
		++_value;
		return *_value;
	}

	const reference operator*()
	{
		return *_value;
	}

	bool operator != (const HeapIterator<T> &rhs)
	{
		return _value != rhs._value;
	}

	bool operator == (const HeapIterator<T> &rhs)
	{
		 return _value == rhs._value;
	}

private:
	T		_value;
};

template<class T, class Alloc = std::allocator<T> >
class MaxHeap
{
public:

	typedef T				value_type;
	typedef T*				pointer;
	typedef T&				reference;
	typedef const T*		const_pointer;
	typedef const T&		const_reference;

	typedef HeapIterator<const_pointer> iterator;
	typedef HeapIterator<const_pointer> const_iterator;

public:
	MaxHeap(T arr[], int n);
	MaxHeap();
	MaxHeap(const MaxHeap<T, Alloc> &rhs);
	MaxHeap &operator=(const MaxHeap<T, Alloc> &rhs);
#if __cplusplus >= 201103L
	MaxHeap(MaxHeap &&rhs);
#endif
	~MaxHeap();

public:
	void push(const T &value);
	T pop();
	void debug();

public:
	iterator begin()
	{
		return iterator(_array);
	}

	iterator end()
	{
		return iterator(_array + _n);
	}

	const_iterator begin() const
	{
		return const_iterator(_array);
	}

	const_iterator end() const
	{
		return const_iterator(_array + _n);
	}

private:
	void heap_shift_up(int pos, const T &value);
	void heap_shift_down(int pos, const T &value);
	void expand_heap();
	void rebuild_heap();
	Alloc &get_allocator() { return _allocator; }

private:
	int _n;
	int _cap;
	pointer _array;
	Alloc _allocator;
};

#endif
