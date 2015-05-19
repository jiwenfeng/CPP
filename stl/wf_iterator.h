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
				normal_iterator() { _cur = 0; }

				normal_iterator(_Iterator i) { _cur = i; }

				normal_iterator(const normal_iterator &i) { _cur = i._cur; }

				normal_iterator& operator=(const normal_iterator& i)
				{
					if(this == &i)
					{
						return *this;
					}
					_cur = i._cur;
					return *this;
				}

				normal_iterator& operator++()
				{
					++_cur;
					return *this;
				}

				normal_iterator operator++(int)
				{
					return normal_iterator(_cur++);
				}

				normal_iterator& operator--()
				{
					--_cur;
					return *this;
				}

				normal_iterator operator--(int)
				{
					return normal_iterator(_cur++);
				}

				normal_iterator operator+=(int n)
				{
					return normal_iterator(_cur + n);
				}

				normal_iterator operator-=(int n)
				{
					return normal_iterator(_cur - n);
				}

				reference operator*()
				{
					return *_cur;
				}

				pointer operator->()
				{
					return _cur;
				}

				reference operator[](size_t n)
				{
					return _cur[n];
				}

				normal_iterator operator+(int n)
				{
					return normal_iterator(_cur + n);
				}

				normal_iterator operator-(int n)
				{
					return normal_iterator(_cur - n);
				}

				_Iterator base() const { return _cur; }

			public:
				_Iterator _cur;
		};

	template<class T, class Container>
		bool operator !=(const normal_iterator<T, Container> &lhs, const normal_iterator<T, Container> &rhs)
		{
			return lhs.base() != rhs.base();
		}

};
#endif
