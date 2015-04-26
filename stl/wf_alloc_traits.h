#ifndef __WF_ALLOC_TRAITS_H__
#define __WF_ALLOC_TRAITS_H__

#include <stdio.h>

namespace wf
{
	template<class T>
		struct alloc_traits
		{
			typedef typename T::pointer pointer;
			typedef typename T::value_type value_type;

			static pointer allocate(T &_alloc, size_t n)
			{
				return _alloc.allocate(n);
			}

			static void deallocate(T &_alloc, pointer p)
			{
				_alloc.deallocate(p);
			}

			static void constuct(T &_alloc, pointer p, const value_type &value)
			{
				_alloc.construct(p, value);
			}

			static void destroy(T &_alloc, pointer p)
			{
				_alloc.destroy(p);
			}

			static size_t max_size(T &_alloc)
			{
				return _alloc.max();
			}
		};
};

#endif
