#include "heap.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

#define FATHER(n) (((n) - 1) / 2)
#define LCHILD(n) (2 * (n) + 1)
#define RCHILD(n) (2 * (n) + 2)

template<class T>
MaxHeap<T>::MaxHeap(T arr[], int n) : _cap(n), _n(0), _array(NULL)
{
	_array = new T[_cap];
	for(int i = 0; i < n; ++i)
	{
		push(arr[i]);
	}
}

template<class T>
MaxHeap<T>::MaxHeap() : _cap(0), _n(0), _array(NULL)
{}

template<class T>
void
MaxHeap<T>::expand_heap()
{
	int cap = _cap < 8 ? 8 : _cap * 2;
	T *new_array = new T[cap];
	memcpy(new_array, _array, _n * sizeof(T));
	_cap = cap;
	delete[] _array;
	_array = new_array;
}

template<class T>
void
MaxHeap<T>::push(const T &value)
{
	if(_n >= _cap - 1)
	{
		expand_heap();
	}
	heap_shift_up(_n++, value);
}

template<class T>
T
MaxHeap<T>::pop()
{
	if(_n == 0)
	{
		return T();
	}
	heap_shift_down();
}

template<class T>
MaxHeap<T>::~MaxHeap()
{
	for(int i = 0; i < _n; ++i)
	{
		_array[i].T::~T();
	}
	delete [] _array;
	_array = NULL;
}

template<class T>
void
MaxHeap<T>::heap_shift_up(int idx, const T &value)
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
	_array[idx] = value;
}

template<class T>
void 
MaxHeap<T>::heap_shift_down()
{}

template<class T>
void
MaxHeap<T>::debug()
{
	for(int i = 0; i < _n; ++i)
	{
		cout<<_array[i]<<endl;
	}
}

int main()
{
	int arr[] = {12, 32, 39, 80, 79, 73, 90, 10, 54, 87};
#define N (sizeof(arr) / sizeof(arr[0]))
	MaxHeap<int> iHeap(arr, N);
	iHeap.debug();
	return 0;
}
