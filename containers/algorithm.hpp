#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ft
{

// equal() :
// 두 요소를 비교한다.

// equal1. default (operator== 사용)
// 요소들을 비교하고, 다른 곳에서 멈춘다.
template <typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  while (first1!=last1) {
    if (!(*first1 == *first2))
      return false;
    ++first1; ++first2;
  }
  return true;
}

// equal2. custom (pred 사용)
// 4번째 인자로 들어온 pred를 사용하여 요소를 비교하고, 다른 곳에서 멈춘다.
// pred의 조건: 함수객체 혹은 함수포인터로, bool로 변환할 수 있는 값을 반환하고 2개의 요소를 인자로 받는다. 인자를 변형하지 않는다.
template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred) {
  while (first1!=last1) {
    if (!pred(*first1,*first2))
      return false;
    ++first1; ++first2;
  }
  return true;
}

// lexicographical_compare() :
// 첫 번째 범위가 두 번째 범위보다 사전적으로 더 작을 경우 true, 같거나 클 경우 false를 반환한다.
// 요소가 다를 때까지 순차적으로 비교한다. 두 요소는 사전식으로 비교한다.
// '사전식'이란:
//		일반 ASCII 문자코드로 비교한다.
//		두 범위 중 하나가 끝날 때까지 같으면 짧은 범위가 사전식으로 더 작다.

// lexicographical_compare1. default (operator< 사용)
template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
  while (first1 != last1) // 범위1이 끝날 때까지 비교한다
  {
    if (first2 == last2 ||  // 범위2가 끝났을 때
      *first2 < *first1) // 범위2의 요소가 더 작을 때
		  return false;
    else if (*first1 < *first2) // 범위1의 요소가 더 작을 때
		  return true;
    ++first1; ++first2;
  }
  return (first2 != last2); // 범위2가 아직 남았다면 범위1이 더 짧으므로 true 반환
}

// lexicographical_compare1. custom (comp 사용)
template <typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
  while (first1 != last1)
  {
    if (first2 == last2 || // 범위2가 끝났을 때
      comp(*first2, *first1)) // 범위2의 요소가 더 작을 때
      return false;
    else if (comp(*first1, *first2)) // 범위1의 요소가 더 작을 때
      return true;
    ++first1; ++first2;
  }
  return (first2 != last2); // 범위2가 아직 남았다면 범위1이 더 짧으므로 true 반환
}

template <typename T>
void swap(T& a, T& b)
{
  T c(a);
  a = b;
  b = c;
}

template<class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
   typedef typename iterator_traits<InputIterator>::difference_type _Distance;
    for(_Distance __n = last - first; __n > 0; --__n) {
    *result = *first;
    ++result; ++first;
  }
  return result;
}

}

#endif // ALGORITHM_HPP