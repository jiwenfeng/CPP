#ifndef __WF_ITERATOR_TYPE_H__
#define __WF_ITERATOR_TYPE_H__

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<class T>
struct iterator_traits
{
	typedef typename T::value_type value_type;
	typedef typename T::pointer	pointer;
	typedef typename T::const_pointer const_pointer;
	typedef typename T::reference reference;
	typedef typename T::const_reference const_reference;
	typedef typename T::iterator_category iterator_category;
};

template<class T>
struct iterator_traits<T *>
{
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef random_access_iterator_tag iterator_category;
};

template<class T>
struct iterator_traits<const T*>
{
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef random_access_iterator_tag iterator_category;
};


#endif
