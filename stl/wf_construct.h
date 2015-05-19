#ifndef __WF_STL_CONSTRUCT_H__
#define __WF_STL_CONSTRUCT_H__

#include "wf_type_traits.h"
#include "wf_iterator_traits.h"

namespace wf
{
	template<class T>
		void Construct(T *p)
		{
			new(p) T();
		}

	template<class T1, class T2>
		void Construct(T1 *p, const T2 &value)
		{
			new(p) T1(value);
		}

	template<class T>
		void _Destroy(T *pointer)
		{
			pointer->~T();
		}

	template<class ForwardIterator>
		void _Destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
		{
		}

	template<class ForwardIterator>
		void _Destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
		{
			for(; first != last; ++first)
			{
				_Destroy(&*first);
			}
		}

	template<typename ForwardIterator>
		void Destroy(ForwardIterator first, ForwardIterator last)
		{
			typedef typename iterator_traits<ForwardIterator>::value_type value_type;
			typedef typename type_traits<value_type>::has_trivial_destructor htd;
			_Destroy_aux(first, last, htd());
		}
}
#endif
