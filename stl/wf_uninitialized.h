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

template<class InputIterator, class OutputIterator>
OutputIterator
wf_uninitialized_copy_aux(InputIterator first, InputIterator last, OutputIterator result, true_type)
{
	return wf::copy(first, last, result);
}

template<class InputIterator, class OutputIterator>
OutputIterator
wf_uninitialized_copy_aux(InputIterator first, InputIterator last, OutputIterator result, false_type)
{
	for(; first != last; ++first, ++result)
	{
		wf::Construct(result, *first);
	}
	return result;
}

template<class InputIterator, class OutputIterator>
OutputIterator
wf_uninitialized_copy(InputIterator first, InputIterator last, OutputIterator result)
{
	typedef typename type_traits<InputIterator>::is_POD is_POD;
	return wf_uninitialized_copy_aux(first, last, result, is_POD());
}

#endif
