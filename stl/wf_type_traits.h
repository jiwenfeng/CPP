#ifndef __WF_TYPE_TRAITS_H__
#define __WF_TYPE_TRAITS_H__

struct true_type {};
struct false_type {};

template<class T>
struct type_tratis
{
	typedef true_type this_dummy_member_must_be_first;
	typedef false_type has_trivial_default_constructor;
	typedef false_type has_trivial_copy_constructor;
	typedef false_type has_trivial_assignment_operator;
	typedef false_type has_trivial_destructor;
	typedef false_type is_POD_type;
};

template<>
struct type_tratis<char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<signed char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<unsigned char>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<short>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<unsigned short>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<int>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<unsigned int>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};
 
template<>
struct type_tratis<long>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<unsigned long>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<float>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};
template<>
struct type_tratis<double>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<>
struct type_tratis<long double>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

template<class T>
struct type_tratis<T *>
{
	typedef true_type has_trivial_default_constructor;
	typedef true_type has_trivial_copy_constructor;
	typedef true_type has_trivial_assignment_operator;
	typedef true_type has_trivial_destructor;
	typedef true_type is_POD_type;
};

#endif
