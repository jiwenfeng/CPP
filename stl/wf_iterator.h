#ifndef __WF_ITERATOR_H__
#define __WF_ITERATOR_H__

#include "wf_iterator_traits.h"

namespace wf
{
	template<class _Iterator, class Container>
		class normal_iterator
		{
			public:
				typedef typename iterator_traits<_Iterator>::pointer pointer;
				typedef typename iterator_traits<_Iterator>::value_type value_type;
				typedef typename iterator_traits<_Iterator>::reference reference;

			public:
				normal_iterator() { _current = 0; }

				normal_iterator(_Iterator i) { _current = i; }

				normal_iterator(const normal_iterator &i) { _current = i._current; }

				normal_iterator& operator=(const normal_iterator& i)
				{
					if(this == &i)
					{
						return *this;
					}
					_current = i._current;
					return *this;
				}

				normal_iterator& operator++()
				{
					++_current;
					return *this;
				}

				normal_iterator operator++(int)
				{
					return normal_iterator(_current++);
				}

				normal_iterator& operator--()
				{
					--_current;
					return *this;
				}

				normal_iterator operator--(int)
				{
					return normal_iterator(_current++);
				}

				normal_iterator operator+=(int n)
				{
					return normal_iterator(_current + n);
				}

				normal_iterator operator-=(int n)
				{
					return normal_iterator(_current - n);
				}

				reference operator*() const
				{
					return *_current;
				}

				pointer operator->() const
				{
					return _current;
				}

				reference operator[](size_t n) const
				{
					return _current[n];
				}

				normal_iterator operator+(int n)
				{
					return normal_iterator(_current + n);
				}

				normal_iterator operator-(int n)
				{
					return normal_iterator(_current - n);
				}

				_Iterator base() const { return _current; }

			public:
				_Iterator _current;
		};

	template<class T, class Container>
		bool operator !=(const normal_iterator<T, Container> &lhs, const normal_iterator<T, Container> &rhs)
		{
			return lhs.base() != rhs.base();
		}

};
#endif
