#ifndef __WF_ALLOC_H__
#define __WF_ALLOC_H__

#include <new>
#include <stdio.h>

namespace wf
{
	template<class T>
		class allocator
		{
			public:
				typedef T	value_type;
				typedef T*	pointer;
				typedef T&	reference;
				typedef const T* const_pointer;
				typedef const T& const_reference;
			public:
				pointer allocate(size_t n)
				{
					return new T[sizeof(T) * n];
				}
				void deallocate(pointer p)
				{
					delete[] p;
				}
				void construct(pointer p, value_type &v)
				{
					new(p) T(v);
				}
				void destroy(pointer p)
				{
					p->~T();
				}
				size_t max()
				{
					return (unsigned int)(-1) / sizeof(T);
				}
		};
};

#endif
