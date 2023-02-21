#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft
{

// pair()

template <typename _T1, typename _T2>
  struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  _T1 first;
  _T2 second;

  // default constructor
  pair() : first(_T1()), second(_T2()) {}

  //// copy constructor
  template <typename _U1, typename _U2>
  pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}

  // initialization constructor
  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

// pair의 연관 함수 (비멤버함수)

// 타입이 같은 2개의 pair는, 멤버가 같으면 equal하다고 본다.
template <typename _T1, typename _T2>
bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first == __y.first && __x.second == __y.second; 
}

template <typename _T1, typename _T2>
bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{ 
  return __x.first < __y.first || 
         (!(__y.first < __x.first) && __x.second < __y.second); 
}

template <typename _T1, typename _T2>
bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x == __y);
}

template <typename _T1, typename _T2>
bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return __y < __x;
}

template <typename _T1, typename _T2>
bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__y < __x);
}

template <typename _T1, typename _T2>
bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x < __y);
}

// make_pair()

template <typename _T1, typename _T2>
pair<_T1, _T2> make_pair(_T1 __x, _T2 __y)
{
  return pair<_T1, _T2>(__x, __y);
}

}

#endif // PAIR_HPP