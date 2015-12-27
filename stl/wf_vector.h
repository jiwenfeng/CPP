#ifndef __WF_VECTOR_H__
#define __WF_VECTOR_H__

#include "wf_alloc.h"
#include "wf_alloc_traits.h"
#include "wf_uninitialized.h"
#include "wf_iterator.h"
#include "wf_algorithm.h"
#include "wf_construct.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace wf
{
	template<class T, class _Alloc = wf::allocator<T> >
		class vector
		{
			public:
				typedef T					value_type;
				typedef T*					pointer;
				typedef const T*			const_pointer;
				typedef T&					reference;
				typedef const T&			const_reference;
			public:
				typedef normal_iterator<pointer, vector> iterator;
				typedef normal_iterator<const_pointer, vector> const_iterator;
				typedef _Alloc alloc_type;
				typedef alloc_traits<_Alloc> _Tr;

			public:
				vector() : _start(0), _finish(0), _cap(0)
				{
				}

				vector(size_t n)
				{
					_start = _Tr::allocate(_alloc, n);
					_finish = _start;
					_cap = _start + n;
				}

				vector(const vector &v)
				{
					_start = _Tr::allocate(_alloc, v.size());
					_finish = _start;
					_cap = _start + v.size();
					_finish = wf_uninitialized_copy(v.begin(), v.end(), _start);
				}

				template<class InputIterator>
					vector(InputIterator first, InputIterator last)
					{
					}

				vector& operator=(const vector &v)
				{
					if(this == &v)
					{
						return *this;
					}
					Destroy(_start, _finish);
					_Tr::deallocate(_alloc, _start);
					_start = _Tr::allocate(_alloc, v.size());
					_finish = _start;
					_cap = _start + v.size();
					_finish = wf_uninitialized_copy(v.begin(), v.end(), _start);
					return *this;
				}

				value_type& operator[](size_t n)
				{
					return *(_start + n);
				}

				~vector()
				{
					Destroy(_start, _finish);
					_Tr::deallocate(_alloc, _start);
				}

			public:
				void push_back(const value_type &v)
				{
					if(_finish != _cap)
					{
						_Tr::constuct(_alloc, _finish, v);
						++_finish;
					}
					else
					{
						size_t new_size = calc_new_size();
						pointer new_start = _Tr::allocate(_alloc, new_size);
						pointer new_finish = new_start;
						new_finish = wf_uninitialized_copy(_start, _finish, new_finish);
						_Tr::constuct(_alloc, new_finish, v);
						++new_finish;
						Destroy(_start, _finish);
						_Tr::deallocate(_alloc, _start);
						_start = new_start;
						_finish = new_finish;
						_cap = _start + new_size;
					}
				}

				void pop_back()
				{
					--_finish;
					Destroy(_finish, _finish);
				}

				iterator begin() { return iterator(_start); }
				const_iterator begin() const { return const_iterator(_start); }

				iterator end() { return iterator(_finish); }
				const_iterator end() const { return const_iterator(_finish); }

				size_t size() const { return _finish - _start; }
				
				size_t capacity() const { return _cap - _start; }

				size_t max_size() { return _Tr::max_size(_alloc); }

				void resize(size_t n) 
				{
					if(n < size())
					{
						Destroy(_start + n, _finish);
						_finish = _start + n;
						return;
					}
					if(size() < n && n < capacity())
					{
						// fill n - size()
						return;
					}
					pointer new_start = _Tr::allocate(_alloc, n);
					pointer new_finish = wf_uninitialized_copy(_start, _finish, new_start);
					// fill n - (new_start - new_finish)
					Destroy(_start, _finish);
					_Tr::deallocate(_start);
					_finish = _start + n;
					_cap = _finish;
				}

				void resize(size_t n, const value_type &value)
				{
				}

				iterator erase(iterator begin, iterator end)
				{
					// TODO
					return begin();
				}

				iterator erase(iterator pos)
				{
					// TODO
					return begin();
				}

				void clear()
				{
				}

			private:
				size_t calc_new_size(size_t n = 1)
				{
					if(max_size() - size() < n)
					{
						abort();
					}
					return size() + wf::max(size(), n);
				}

			private:
				pointer _start;
				pointer _finish;
				pointer _cap;
				alloc_type _alloc;
		};
};

#endif
