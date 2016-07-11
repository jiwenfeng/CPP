#include "heap.h"
#include <bits/stl_construct.h>
#include <bits/stl_iterator.h>
#include <iostream>

#ifdef __TEST_STRING
#include <string>
#endif

#define FATHER(n) (((n) - 1) / 2)
#define LCHILD(n) (2 * (n) + 1)
#define RCHILD(n) (2 * (n) + 2)

template<class T, class Alloc>
MaxHeap<T, Alloc>::MaxHeap(T arr[], int n) : _cap(n), _n(0), _array(NULL)
{
	_array = _allocator.allocate(n);
	for(int i = 0; i < n; ++i)
	{
		push(arr[i]);
	}
}

template<class T, class Alloc>
MaxHeap<T, Alloc>::MaxHeap() : _cap(0), _n(0), _array(NULL)
{}

template<class T, class Alloc>
MaxHeap<T, Alloc>::MaxHeap(const MaxHeap<T, Alloc> &rhs)
{
	this->operator=(rhs);
}

template<class T, class Alloc>
MaxHeap<T, Alloc> &
MaxHeap<T, Alloc>::operator=(const MaxHeap<T, Alloc> &rhs)
{
	if(this == &rhs)
	{
		return *this;
	}
	for(int i = 0; i < rhs._n; ++i)
	{
		push(rhs._array[i]);
	}
	return *this;
}

template<class T, class Alloc>
void
MaxHeap<T, Alloc>::expand_heap()
{
	int cap = _cap < 8 ? 8 : _cap * 2;
	T *new_array = _allocator.allocate(cap);
	for(int i = 0; i < _n; ++i)
	{
		_allocator.construct(&new_array[i], _array[i]);
	}
	_cap = cap;
	_array = new_array;
}

template<class T, class Alloc>
void 
rebuild_heap()
{
}

template<class T, class Alloc>
void
MaxHeap<T, Alloc>::push(const T &value)
{
	if(_n == _cap)
	{
		expand_heap();
	}
	heap_shift_up(_n++, value);
}

template<class T, class Alloc>
T
MaxHeap<T, Alloc>::pop()
{
	if(0 == _n)
	{
		return T();
	}
	T value = _array[0];
	heap_shift_down(0, _array[--_n]);
	return value;
}

template<class T, class Alloc>
MaxHeap<T, Alloc>::~MaxHeap()
{
	std::_Destroy(begin(), end(), get_allocator());
	_allocator.deallocate(_array, _cap);
}

template<class T, class Alloc>
void
MaxHeap<T, Alloc>::heap_shift_up(int idx, const T &value)
{
	int pos = FATHER(idx);
	while(idx)
	{
		if(_array[pos] > value)
		{
			break;
		}
		std::swap(_array[pos], _array[idx]);
		idx = pos;
		pos = FATHER(pos);
	}
	_allocator.construct(&_array[idx], value);
}

template<class T, class Alloc>
void 
MaxHeap<T, Alloc>::heap_shift_down(int pos, const T &value)
{
	int max_child = RCHILD(pos);
	while(max_child <= _n)
	{
		max_child -= max_child == _n || _array[max_child] < _array[max_child - 1];
		if(_array[max_child] < value)
		{
			break;
		}
		_array[pos] = _array[max_child];
		pos = max_child;
		max_child = RCHILD(pos);
	}
	_array[pos] = value;
}

template<class T, class Alloc>
void
MaxHeap<T, Alloc>::debug()
{
	std::cout<<"---------------------"<<std::endl;
	for(iterator i = begin(); i != end(); ++i)
	{
		std::cout<<(*i)<<std::endl;
	}
}

#if __cplusplus >= 201103L
template<class T, class Alloc>
MaxHeap<T, Alloc>::MaxHeap(MaxHeap &&rhs)
{
	std::_Destroy(begin(), end(), get_allocator());
	_allocator.deallocate(_array, _cap);
	this->_n = rhs._n;
	this->_cap = rhs._cap;
	this->_array = rhs._array;
	rhs._array = NULL;
	rhs._n = rhs._cap = 0;
}
#endif


int main()
{
#ifdef __TEST_INT__
	int arr[] = {12, 32, 39, 80, 79, 73, 90, 10, 54, 87};
#define N (sizeof(arr) / sizeof(arr[0]))
	MaxHeap<int> iHeap(arr, N);
//	iHeap.debug();
//
//	iHeap.pop();
//	iHeap.debug();
//
//	iHeap.pop();
//	iHeap.debug();

	MaxHeap<int>::iterator itr = iHeap.begin();
	iHeap.debug();


#endif

#ifdef __TEST_STRING__
	std::string arr[] = {"ff", "aa", "ss", "bb", "cc", "dd", "hh", "ii", "kk"};
#define N (sizeof(arr) / sizeof(arr[0]))
	MaxHeap<std::string> sHeap(arr, N);
	sHeap.debug();
#endif
	return 0;
}
