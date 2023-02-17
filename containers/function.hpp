#ifndef FUNCTION_HPP
#define FUNCTION_HPP

namespace ft
{

template <typename _Pair>
struct _Select1st {
  typename _Pair::first_type& operator()(_Pair& __x) const {
    return __x.first;
  }
  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

}

#endif // FUNCTION_HPP