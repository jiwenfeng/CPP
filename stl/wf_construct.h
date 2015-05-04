#ifndef __WF_STL_CONSTRUCT_H__
#define __WF_STL_CONSTRUCT_H__

#include "wf_iterator.h"
#include "wf_type_traits.h"

namespace wf
{

	template<class T>
		void _destroy(T *pointer)
		{
			pointer->~T();
		}

	template<class ForwardIterator>
		void Destroy_aux(ForwardIterator first, ForwardIterator last, true_type)
		{
		}

	template<class ForwardIterator>
		void Destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
		{
			for(; first != last; ++first)
			{
				_destroy(&*first);
			}
		}

	template<typename ForwardIterator>
		void Destroy(ForwardIterator first, ForwardIterator last)
		{
			typedef typename iterator_traits<ForwardIterator>::value_type value_type;
			typedef typename type_traits<value_type>::has_trivial_destructor htd;
			Destroy_aux(first, last, htd());
		}
}
#endif
