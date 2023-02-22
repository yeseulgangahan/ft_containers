#ifndef FUNCTION_HPP
#define FUNCTION_HPP

namespace ft
{

// select1st :
// value_type에서 first(즉 key)를 뽑아내주는 functor

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