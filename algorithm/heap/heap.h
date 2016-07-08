#ifndef __HEAP_H__
#define __HEAP_H__
#include <iostream>

template<class T>
class MaxHeap
{
private:
	int _n;
	int _cap;
	T *_array;
private:
	void heap_shift_up(int pos, const T &value);
	void heap_shift_down();
	void expand_heap();
public:
	MaxHeap(T arr[], int n);
	MaxHeap();
	void push(const T &value);
	T pop();
	~MaxHeap();

public:
	void debug();
};

#endif
