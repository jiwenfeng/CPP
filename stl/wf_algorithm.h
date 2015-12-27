#ifndef __WF_ALGORITHM_H__
#define __WF_ALGORITHM_H__

#include "wf_type_traits.h"
#include "wf_iterator.h"
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

namespace wf
{
	template<class I>
		struct iter_base
		{
			//typename I
		};

	template<class _I, class Container>
		struct iter_base<normal_iterator<_I, Container> >
		{};

	template<class T>
		const T&
		max(const T &_T1, const T &_T2)
		{
			return _T1 < _T2 ? _T2 : _T1;
		}
	
	template<class T, class compare>
		const T&
		max(const T &_T1, const T&_T2, compare cmp)
		{
			return cmp(_T1, _T2) ? _T1 : _T2;
		}

	template<class T>
		const T&
		min(const T &_T1, const T &_T2)
		{
			return _T1 < _T2 ? _T1 : _T2;
		}

	template<class T, class compare>
		const T&
		min(const T &_T1, const T &_T2, compare cmp)
		{
			return cmp(_T1, _T2) ? _T1 : _T2;
		}

	template<class T>
		T *
		copy_m(T *first, T *last, T *result, true_type)
		{
			memmove(result, last - first, sizeof(T) * (last - first));
			return result + sizeof(T) * (last - first);
		}

	template<class T>
		T *
		copy_m(T *first, T *last, T *result, false_type)
		{
			size_t n = last - first;
			for(size_t i = 0; i < n; ++i)
			{
				*result++ = *first++;
			}
			return result;
		}

	template<class RandomAccessIterator, class OutputIterator>
		OutputIterator
		copy_t(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
		{
			for(size_t i = 0; i < last - first; ++i, ++first, ++result)
			{
				*result = *first;
			}
			return result;
		}

	template<class InputIterator, class OutputIterator>
		OutputIterator
		copy_t(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
		{
			for(; first != last; ++first, ++result)
			{
				*result = *first;
			}
			return result;
		}


	template<class InputIterator, class OutputIterator>
		struct copy_dispatch
		{
			static OutputIterator
				copy_d(InputIterator first, InputIterator last, OutputIterator result)
				{
					for(; first != last; ++first, ++result)
					{
						*result = *first;
					}
					return result;
				}
		};

	template<class T>
		struct copy_dispatch<T *, T *>
		{
			static T *
				copy_d(T *first, T *last, T *result)
				{
					typedef typename type_traits<T>::has_trivial_assignment_operator type;
					return copy_m(first, last, result, type());
				}
		};

	template<class T>
		struct copy_dispatch<const T*, T *>
		{
			static T *
				copy_d(const T *first, const T *last, T *result)
				{
					typedef typename type_traits<T>::has_trivial_assignment_operator type;
					return copy_m(first, last, result, type());
				}
		};


	template<class InputIterator, class OutputIterator>
		OutputIterator
		copy(InputIterator first, InputIterator last, OutputIterator result)
		{
			return copy_dispatch<InputIterator, OutputIterator>::copy_d(first, last, result);
		}

		char *
		copy(char *first, char *last, char *result)
		{
			memmove(result, first, last - first);
			return result + (last - first);
		}

		wchar_t *
		copy(wchar_t *first, wchar_t *last, wchar_t *result)
		{
			memmove(result, first, sizeof(wchar_t) * (last - first));
			return result + (last - first) * sizeof(wchar_t);
		}

		template<class InputIterator, class T>
			void
			fill_aux(InputIterator first, InputIterator last, const T& value, true_type)
			{
				
			}

		template<class InputIterator, class T>
			void
			fill_aux(InputIterator first, InputIterator last, const T& value, false_type)
			{
				for(; first != last; ++first)
				{
					*first = value;
				}
			}

		template<class InputIterator, class T>
			void
			fill(InputIterator first, InputIterator last, const T &value)
			{
			}

		template<class InputIterator, class T>
			void 
			fill_n(InputIterator first, size_t n, const T &value)
			{
			}
};

#endif
