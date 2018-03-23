#pragma once

template <typename T> struct gbRemove_Reference       { typedef T Type; };
template <typename T> struct gbRemove_Reference<T &>  { typedef T Type; };
template <typename T> struct gbRemove_Reference<T &&> { typedef T Type; };
template <typename T> inline T &&gb_forward_ownership(typename gbRemove_Reference<T>::Type &t)  { return static_cast<T &&>(t); }
template <typename T> inline T &&gb_forward_ownership(typename gbRemove_Reference<T>::Type &&t) { return static_cast<T &&>(t); }

template <typename F>
struct gbImpl_Defer {
	F f;
	gbImpl_Defer(F &&f) : f(gb_forward_ownership<F>(f)) {}
	~gbImpl_Defer() { f(); }
};
template <typename F> gbImpl_Defer<F> gb_defer_func(F &&f) { return gbImpl_Defer<F>(gb_forward_ownership<F>(f)); }

#ifndef defer
#define GB_DEFER_1(x, y) x##y
#define GB_DEFER_2(x, y) GB_DEFER_1(x, y)
#define GB_DEFER_3(x)    GB_DEFER_2(x, __COUNTER__)
#define defer(code)      auto GB_DEFER_3(_defer_) = gb_defer_func([&](){code;})
#endif