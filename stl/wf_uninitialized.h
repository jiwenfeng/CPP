#ifndef __WF_UNINILIALIZED_H__
#define __WF_UNINILIALIZED_H__

#include "wf_iterator_traits.h"
#include "wf_type_traits.h"
#include "wf_construct.h"
#include "wf_algorithm.h"
#include <typeinfo>
#include <iostream>

template<class InputIterator, class T>
void
wf_uninitialized_fill_aux(InputIterator first, InputIterator last, const T& value, true_type)
{
	wf::fill(first, last, value);
}

template<class InputIterator, class T>
void
wf_uninitialized_fill_aux(InputIterator first, InputIterator last, const T& value, false_type)
{
	for(; first != last; ++first)
	{
		wf::Construct(first, value);
	}
}

template<class InputIterator, class T>
void
wf_uninitialized_fill(InputIterator first, InputIterator last, const T& value)
{
	typedef typename type_traits<T>::is_POD is_POD;
	wf_uninitialized_fill_aux(first, last, value, is_POD());
}

template<class InputIterator, class T>
void
wf_uninitialized_fill_n(InputIterator first, size_t n, const T& value)
{
}

template<bool>
struct uninit_copy
{
	template<class InputIterator, class OutputIterator>
		static OutputIterator
		uninit_copy_aux(InputIterator first, InputIterator last, OutputIterator result)
		{
			for(; first != last; ++first, ++result)
			{
				std::cout<<typeid(result).name()<<std::endl;
				std::cout<<typeid(first).name()<<std::endl;
				wf::Construct(result, *first);
			}
			return result;
		}
};

template<>
struct uninit_copy<true>
{
	template<class InputIterator, class OutputIterator>
		static OutputIterator
		uninit_copy_aux(InputIterator first, InputIterator last, OutputIterator result)
		{
			return wf::copy(first, last, result);
		}
};

template<class InputIterator, class OutputIterator>
OutputIterator
wf_uninitialized_copy(InputIterator first, InputIterator last, OutputIterator result)
{
	typedef typename iterator_traits<InputIterator>::value_type ValueType1;
	typedef typename iterator_traits<InputIterator>::value_type ValueType2;
	return uninit_copy<is_pod<ValueType1>::value && is_pod<ValueType2>::value>::uninit_copy_aux(first, last, result);
}

#endif
