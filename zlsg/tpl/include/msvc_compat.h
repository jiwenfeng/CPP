#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <functional>

// don't warn about multiple assignment operators
#pragma warning(disable:4522)

// replace the =default, =delete semantics
struct nocopy_volatile
{
private:
	nocopy_volatile(const nocopy_volatile&);
	nocopy_volatile& operator=(const nocopy_volatile&);
	nocopy_volatile& operator=(const nocopy_volatile&) volatile;

public:
	nocopy_volatile() {}
};

// use to emulate an enum class
#define ENUM_CLASS_BEGIN(x) struct x { enum vals {
#define ENUM_CLASS_END(x) \
	}; \
	x() {} \
	x(vals val) : m_val(val) {} \
	vals m_val; \
	}; \
	inline bool operator==(x lhs, x rhs) { return lhs.m_val == rhs.m_val; } \
	inline bool operator!=(x lhs, x rhs) { return !(lhs == rhs); }

#define noexcept throw()

// map the various msvc interlocked intrinsics to overloaded funcs

// x86 and x64 have no acquire or release semantics! Not much we can do...

#if !defined _M_IX86 && !defined _M_X64
#error No support for your architecture
#endif

#include <intrin.h>

#define __sync_synchronize _mm_mfence
#define __sync_bool_compare_and_swap(SRC, OLD, NEW) InterlockedCompareExchange(SRC, NEW, OLD)

template <typename T, size_t S>
struct IsSizeAndArithmetic
{
	static const bool value = std::is_arithmetic<T>::value && !std::is_same<T, bool>::value && (sizeof(T) == S);
};

#ifdef _M_X64
typedef unsigned __int32 __atomic_pointer_t;
typedef long __atomic_int32_t;
#else
typedef unsigned __int64 __atomic_pointer_t;
typedef int __atomic_int32_t;
#endif

// __sync_lock_test_and_set
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_lock_test_and_set(T* addr, __int8 val)
{ return _InterlockedExchange8((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_lock_test_and_set(T* addr, unsigned __int16 val)
{ return _InterlockedExchange16((unsigned __int16*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_lock_test_and_set(T* addr, __atomic_int32_t val)
{ return _InterlockedExchange((long*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_lock_test_and_set(T* addr, unsigned __int64 val)
{ return _InterlockedExchange64((unsigned __int64*)addr, val); }

__forceinline bool __sync_lock_test_and_set(volatile bool* addr, bool val)
{ return 0 != __sync_lock_test_and_set((__int8*)addr, (char)val); }

template <typename T>
__forceinline typename std::enable_if<std::is_pointer<T>::value, T>::type
__sync_lock_test_and_set(T* addr, T val)
{ return (T)__sync_lock_test_and_set((__atomic_pointer_t*)addr, (__atomic_pointer_t)val); }

// __sync_lock_release
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_lock_release(T* addr)
{ return _InterlockedExchange8((__int8*)addr, 0); }

__forceinline bool __sync_lock_release(volatile bool* addr)
{ return 0 != __sync_lock_release((__int8*)addr); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_lock_release(T* addr)
{ return _InterlockedExchange16((unsigned __int16*)addr, 0); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_lock_release(T* addr)
{ return _InterlockedExchange((long*)addr, 0); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_lock_release(T* addr)
{ return _InterlockedExchange64((unsigned __int64*)addr, 0); }

// __sync_val_compare_and_swap
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_val_compare_and_swap(T* addr, __int8 old_val, __int8 new_val)
{ return _InterlockedCompareExchange8((__int8*)addr, new_val, old_val); }

__forceinline bool __sync_val_compare_and_swap(volatile bool* addr, bool old_val, bool new_val)
{ return 0 != __sync_val_compare_and_swap((__int8*)addr, old_val, new_val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_val_compare_and_swap(T* addr, unsigned __int16 old_val, unsigned __int16 new_val)
{ return _InterlockedCompareExchange16((unsigned __int16*)addr, new_val, old_val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_val_compare_and_swap(T* addr, __atomic_int32_t old_val, __atomic_int32_t new_val)
{ return _InterlockedCompareExchange((__atomic_int32_t*)addr, new_val, old_val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_val_compare_and_swap(T* addr, unsigned __int64 old_val, unsigned __int64 new_val)
{ return _InterlockedCompareExchange64((unsigned __int64*)addr, new_val, old_val); }

// __sync_fetch_and_add
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_fetch_and_add(T* addr, __int8 val)
{ return _InterlockedExchangeAdd8((__int8*)addr, val); }

__forceinline bool __sync_fetch_and_add(volatile bool* addr, bool val)
{ return 0 != __sync_fetch_and_add((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_fetch_and_add(T* addr, unsigned __int16 val)
{ return _InterlockedExchangeAdd16((unsigned __int16*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_fetch_and_add(T* addr, __atomic_int32_t val)
{ return _InterlockedExchangeAdd((long*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_fetch_and_add(T* addr, unsigned __int64 val)
{ return _InterlockedExchangeAdd64((unsigned __int64*)addr, val); }

// __sync_fetch_and_sub
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_fetch_and_sub(T* addr, __int8 val)
{ return _InterlockedExchangeAdd8((__int8*)addr, -val); }

__forceinline bool __sync_fetch_and_sub(volatile bool* addr, bool val)
{ return 0 != __sync_fetch_and_sub((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_fetch_and_sub(T* addr, unsigned __int16 val)
{ return _InterlockedExchangeAdd16((unsigned __int16*)addr, -val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_fetch_and_sub(T* addr, __atomic_int32_t val)
{ return _InterlockedExchangeAdd((long*)addr, -val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_fetch_and_sub(T* addr, unsigned __int64 val)
{ return _InterlockedExchangeAdd64((unsigned __int64*)addr, -val); }

// __sync_fetch_and_and
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_fetch_and_and(T* addr, __int8 val)
{ return _InterlockedAnd8((__int8*)addr, val); }

__forceinline bool __sync_fetch_and_and(volatile bool* addr, bool val)
{ return 0 != __sync_fetch_and_and((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_fetch_and_and(T* addr, unsigned __int16 val)
{ return _InterlockedAnd16((unsigned __int16*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_fetch_and_and(T* addr, __atomic_int32_t val)
{ return _InterlockedAnd((__atomic_int32_t*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_fetch_and_and(T* addr, unsigned __int64 val)
{ return _InterlockedAnd64((unsigned __int64*)addr, val); }

// __sync_fetch_and_or
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_fetch_and_or(T* addr, __int8 val)
{ return _InterlockedOr8((__int8*)addr, val); }

__forceinline bool __sync_fetch_and_or(volatile bool* addr, bool val)
{ return 0 != __sync_fetch_and_or((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_fetch_and_or(T* addr, unsigned __int16 val)
{ return _InterlockedAnd16((unsigned __int16*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_fetch_and_or(T* addr, __atomic_int32_t val)
{ return _InterlockedOr((__atomic_int32_t*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_fetch_and_or(T* addr, unsigned __int64 val)
{ return _InterlockedOr64((unsigned __int64*)addr, val); }

// __sync_fetch_and_xor
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_fetch_and_xor(T* addr, __int8 val)
{ return _InterlockedXor8((__int8*)addr, val); }

__forceinline bool __sync_fetch_and_xor(volatile bool* addr, bool val)
{ return 0 != __sync_fetch_and_xor((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_fetch_and_xor(T* addr, unsigned __int16 val)
{ return _InterlockedXor16((unsigned __int16*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_fetch_and_xor(T* addr, __atomic_int32_t val)
{ return _InterlockedXor((__atomic_int32_t*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_fetch_and_xor(T* addr, unsigned __int64 val)
{ return _InterlockedXor64((unsigned __int64*)addr, val); }

// __sync_add_and_fetch
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_add_and_fetch(T* addr, __int8 val)
{ return _InterlockedExchangeAdd8((__int8*)addr, val) + val; }

__forceinline bool __sync_add_and_fetch(volatile bool* addr, bool val)
{ return 0 != __sync_add_and_fetch((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_add_and_fetch(T* addr, unsigned __int16 val)
{ return _InterlockedExchangeAdd16((unsigned __int16*)addr, val) + val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_add_and_fetch(T* addr, __atomic_int32_t val)
{ return _InterlockedExchangeAdd((long*)addr, val) + val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_add_and_fetch(T* addr, unsigned __int64 val)
{ return _InterlockedExchangeAdd64((unsigned __int64*)addr, val) + val; }

// __sync_sub_and_fetch
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_sub_and_fetch(T* addr, __int8 val)
{ return _InterlockedExchangeAdd8((__int8*)addr, -val) - val; }

__forceinline bool __sync_sub_and_fetch(volatile bool* addr, bool val)
{ return 0 != __sync_sub_and_fetch((__int8*)addr, val); }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_sub_and_fetch(T* addr, unsigned __int16 val)
{ return _InterlockedExchangeAdd16((unsigned __int16*)addr, -val) - val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_sub_and_fetch(T* addr, __atomic_int32_t val)
{ return _InterlockedExchangeAdd((long*)addr, -val) - val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_sub_and_fetch(T* addr, unsigned __int64 val)
{ return _InterlockedExchangeAdd64((unsigned __int64*)addr, -val) - val; }

// __sync_and_and_fetch
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_and_and_fetch(T* addr, __int8 val)
{ return _InterlockedAnd8((__int8*)addr, val) & val; }

__forceinline bool __sync_and_and_fetch(volatile bool* addr, bool val)
{ return (0 != __sync_and_and_fetch((__int8*)addr, val)) & val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_and_and_fetch(T* addr, unsigned __int16 val)
{ return _InterlockedAnd16((unsigned __int16*)addr, val) & val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_and_and_fetch(T* addr, __atomic_int32_t val)
{ return _InterlockedAnd((__atomic_int32_t*)addr, val) & val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_and_and_fetch(T* addr, unsigned __int64 val)
{ return _InterlockedAnd64((unsigned __int64*)addr, val) & val; }

// __sync_or_and_fetch
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_or_and_fetch(T* addr, __int8 val)
{ return _InterlockedOr8((__int8*)addr, val) | val; }

__forceinline bool __sync_or_and_fetch(volatile bool* addr, bool val)
{ return (0 != __sync_or_and_fetch((__int8*)addr, val)) | val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_or_and_fetch(T* addr, unsigned __int16 val)
{ return _InterlockedAnd16((unsigned __int16*)addr, val) | val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_or_and_fetch(T* addr, __atomic_int32_t val)
{ return _InterlockedOr((__atomic_int32_t*)addr, val) | val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_or_and_fetch(T* addr, unsigned __int64 val)
{ return _InterlockedOr64((unsigned __int64*)addr, val) | val; }

// __sync_xor_and_fetch
template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 1>::value, T>::type
__sync_xor_and_fetch(T* addr, __int8 val)
{ return _InterlockedXor8((__int8*)addr, val) ^ val; }

__forceinline bool __sync_xor_and_fetch(volatile bool* addr, bool val)
{ return (0 != __sync_xor_and_fetch((__int8*)addr, val)) ^ val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 2>::value, T>::type
__sync_xor_and_fetch(T* addr, unsigned __int16 val)
{ return _InterlockedXor16((unsigned __int16*)addr, val) ^ val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 4>::value, T>::type
__sync_xor_and_fetch(T* addr, __atomic_int32_t val)
{ return _InterlockedXor((__atomic_int32_t*)addr, val) ^ val; }

template <typename T>
__forceinline typename std::enable_if<IsSizeAndArithmetic<T, 8>::value, T>::type
__sync_xor_and_fetch(T* addr, unsigned __int64 val)
{ return _InterlockedXor64((unsigned __int64*)addr, val) ^ val; }
