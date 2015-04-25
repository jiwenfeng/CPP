#ifndef __WF_VECTOR_H__
#define __WF_VECTOR_H__

#include "wf_alloc.h"
#include "wf_iterator.h"
#include <stdio.h>

namespace wf
{
	template<class T, class _Alloc = wf::allocator<T> >
		class vector
		{
			public:
				typedef T	value_type;
				typedef T*	pointer;
				typedef T&	reference;
				typedef const T* const_pointer;
				typedef const T& const_reference;
			public:
				typedef normal_iterator<pointer, vector> iterator;
				typedef _Alloc alloc_data;
			public:
				vector() : _start(0), _finish(0), _cap(0)
				{
				}

				vector(size_t n)
				{

				}

				vector(const vector &v)
				{
				}

				vector& operator=(const vector &v)
				{
					if(this == &v)
					{
						return *this;
					}
					return *this;
				}

				value_type& operator[](size_t n)
				{

				}

				~vector() {}

			public:
				void push_back(value_type &v)
				{}

				value_type pop_back()
				{}

				iterator begin() { return iterator(_start); }

				iterator end() { return iterator(_finish); }

				size_t size() { return _finish - _start; }


			private:
				pointer _start;
				pointer _finish;
				pointer _cap;
		};
};

#endif
