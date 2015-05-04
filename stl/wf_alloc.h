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
					return static_cast<pointer>(::operator new (n * sizeof(T)));
				}
				void deallocate(pointer p)
				{
					::operator delete(p);
				}
				void construct(pointer p, const value_type &v)
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
