#ifndef __WF_ALGORITHM_H__
#define __WF_ALGORITHM_H__

namespace wf
{
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

#if 0

	template<class ForwardIterator, class Size, class T>
		inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T&x)
		{
			return __uninitialize_fill_n(first, n, x, value_type(first));
		}

	template<class InputIterator, class OutputIterator>
		inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
		{
			return copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
		}

	inline char *copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
	{
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	template<class InputIterator, class OutputIterator>
		struct copy_dispatch
		{
			OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
			{
				return __copy(first, last, result, iterator_category(first));
			}
		};

	template<class T>
		struct copy_dispatch
		{
			T *operator()(T *first, T *last, T *result)
			{
				typedef typename type_traits<T>::has_trivial_assignment_operator t;
				return copy_t(first, last, result, t());
			}
		};

	template<class T>
		struct copy_dispatch<T *, T *>
		{
			T *operator()(const T *first, const T *last, T *result)
			{
				typedef typename type_traits<T>::has_trivial_assignment_operator t;
				return copy_t(first, last, result, t());
			}
		};

	template<class InputIterator, class OutputIterator>
		inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
		{
			for(; first != last; ++result, ++first)
			{
				*result = *first;
			}
			return result;
		}

	template<class RandomAccessIterator, class OutputIterator>
		inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
		{
			return copy_d(first, last, result, distance_type(first));
		}

	template<class RandomAccessIterator, class OutputIterator, class Distance>
		inline OutputIterator copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *)
		{
			for(Distance n = last - first; n > 0; --n, ++result, ++first)
			{
				*result = *first;
			}
			return result;
		}

	template<class T>
		inline T* copy_t(const T* first, const T* last, T *result, __true_type)
		{
			memmove(result, first, sizeof(T) * (last - first));
			return result + (last - first);
		}

	template<calss T>
		inline T* copy_t(const T* first, const T* last, T *result, __false_type)
		{
			return copy_d(first, last, result, (ptrdiff_t *)0);
		}
#endif
};

#endif
