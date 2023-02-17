#ifndef CONSTRUCT_HPP
#define CONSTRUCT_HPP

namespace ft
{

template <typename _T1, typename _T2>
	void _Construct(_T1* __p, const _T2& __value)
	{ new (static_cast<void*>(__p)) _T1(__value); }
  
}

#endif // CONSTRUCT_HPP
