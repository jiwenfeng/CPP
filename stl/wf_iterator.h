#ifndef __WF_ITERATOR_H__
#define __WF_ITERATOR_H__

namespace wf
{
	template<class T>
		struct iterator_traits
		{
			typedef typename T::value_type value_type;
			typedef typename T::pointer	pointer;
			typedef typename T::const_pointer const_pointer;
			typedef typename T::reference reference;
			typedef typename T::const_reference const_reference;
		};

	template<class T>
		struct iterator_traits<T *>
		{
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
		};

	template<class T>
		struct iterator_traits<const T*>
		{
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
		};

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
