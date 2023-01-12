#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "iterator_base_types.hpp"
  // ft::iterator_traits

namespace ft
{

// reverse_iterator
// : 반복자 어댑터. 양방향 또는 임의엑세스 반복자가 범위를 반복하는 방향을 반대로 바꾼다.

template<typename _Iterator>
class reverse_iterator 
  : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                    typename iterator_traits<_Iterator>::value_type,
                    typename iterator_traits<_Iterator>::difference_type,
                    typename iterator_traits<_Iterator>::pointer,
                    typename iterator_traits<_Iterator>::reference>
{
  protected:
    // base iterator란?
    // : original iterator의 copy. 내부적으로 유지된다.
    //   단, reverse iterator가 가리키는 요소의 다음 요소를 가리키고 있다. reverse iterator와 반대방향으로 반복한다.
    //   base()를 사용하면 base iterator의 copy를 얻을 수 있다.
    _Iterator current;

  public:
    typedef _Iterator    iterator_type;
    typedef typename iterator_traits<_Iterator>::difference_type    difference_type;
    typedef typename iterator_traits<_Iterator>::reference    reference;
    typedef typename iterator_traits<_Iterator>::pointer    pointer;

  public:
    // CONSTRUCTOR1. default
    // 객체를 가리키고 있지 않은 역반복자를 생성한다. 
    // 내부의 base iterator는 value-initialized된다. 
    //
    // Q. value initialized란?
    // A. 이 경우에는 base iterator를 nullptr로 초기화해준다는 말이다.
    //
    // Q. 이 생성자는 어디에 쓸까?
    // A. 알고리즘 내부에서 사용할 수 있다.
    reverse_iterator() : current(_Iterator()) { }

    // CONSTRUCTOR2. initialization
    // 원본을 복제한다. 동작만 역으로 바뀐다.
    explicit reverse_iterator(iterator_type __x): current(__x) {}
    
    // CONSTRUCTOR3. copy
    // Q. 함수템플릿이 있는데 함수도 만든 이유
    // A. 컴파일러가 자동생성하는 복사생성자 대신 같은 타입 인자에 대한 복사생성자를 위해 선언했다.
    reverse_iterator(const reverse_iterator& __x) : current(__x.current) { }

    // Q. 함수템플릿으로 복사생성자를 만든 이유.
    // A. Allow iterator to const_iterator conversion.
    //    즉, 들어오는 얘가 나랑 다른 타입의 reverse_iterator일 때도 지원하기 위해서 만든다.
    //    예를 들어, 나 자신은 const int *를 담고 있는데, 들어오는 얘가 그냥 int *를 담고 있을 때. (반대의 경우에는 형변환이 안 되므로 컴파일에러가 발생할 것이다.)
    //    그때는 __x의 protected 멤버변수에 접근할 수 없으니, base()를 사용하여 접근해야 한다.
    template<typename _Iterator2>
    reverse_iterator(const reverse_iterator<_Iterator2>& __x): current(__x.base()) {}

    // base iterator의 복사를 반환한다.
    iterator_type base() const { return current; }

    reference operator*() const {
      _Iterator __tmp = current;
      return *--__tmp;
    }

    pointer operator->() const { return &(operator*()); }

    reverse_iterator& operator++() {
      --current;
      return *this;
    }

    reverse_iterator operator++(int) {
      reverse_iterator __tmp = *this;
      --current;
      return __tmp;
    }

    reverse_iterator& operator--() {
      ++current;
      return *this;
    }

    reverse_iterator operator--(int) {
      reverse_iterator __tmp = *this;
      ++current;
      return __tmp;
    }
    
    reverse_iterator operator+(difference_type __n) const { return reverse_iterator(current - __n); }

    reverse_iterator& operator+=(difference_type __n) {
      current -= __n;
      return *this;
    }

    reverse_iterator operator-(difference_type __n) const 
    { return reverse_iterator(current + __n); }

    reverse_iterator& operator-=(difference_type __n) {
      current += __n;
      return *this;
    }

    reference operator[](difference_type __n) const { return *(*this + __n); }  
}; 

// normal iterator가 const와 그냥 반복자의 비교연산을 지원해주는 것과 달리,
// C++98의 reverse iterator는 타입이 완전히 같은 경우에 한해서만 비교연산을 지원하는 한계를 갖는다.
// C++11에서 수정되었다.

// EQUALITY/UNEQUALITY
// : 같은지 비교한다. 두 반복자가 같은 시퀀스를 반복하고 있을 때 의미있다.
// (정방향반복자 필수 연산자)

template<typename _Iterator>
  inline bool operator==(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return __x.base() == __y.base(); }

template<typename _Iterator>
  inline bool operator!=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return !(__x == __y); }
  
// INEQUALITY RELATIONAL
// : 비교한다. (랜덤액세스반복자 필수 연산자)

template<typename _Iterator>
  inline bool operator<(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return __y.base() < __x.base(); }

template<typename _Iterator>
  inline bool operator>(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return __y < __x; }

template<typename _Iterator>
  inline bool operator<=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return !(__y < __x); }

template<typename _Iterator>
  inline bool operator>=(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return !(__x < __y); }

template<typename _Iterator>
  inline typename reverse_iterator<_Iterator>::difference_type operator-(const reverse_iterator<_Iterator>& __x, const reverse_iterator<_Iterator>& __y) 
  { return __y.base() - __x.base(); }

template<typename _Iterator>
  inline reverse_iterator<_Iterator> operator+(typename reverse_iterator<_Iterator>::difference_type __n, const reverse_iterator<_Iterator>& __x) 
  { return reverse_iterator<_Iterator>(__x.base() - __n); }


// normal_iterator
// 1) 이 반복자 어댑터의 주요 목적은 클래스가 아닌 이터레이터(예: 포인터)를 클래스인 이터레이터로 변환하는 것이다. 
// 2) 템플릿인자 _Iterator의 연산자들을 변경하지 않는다.
// 3) 템플릿인자 _Container는 단독으로 존재한다(즉 다른 곳에서 사용되지 않는다). _Iterator가 동일하더라도 _Container가 다른 경우 다른 유형을 인스턴스화할 수 있도록 한다.
//		예: _normal_iterator< char*, vector<char> > 와 _normal_iterator< char*, basic_string<char> > 는 다른 타입이다.

template<typename _Iterator, typename _Container>
class __normal_iterator
  : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                    typename iterator_traits<_Iterator>::value_type,
                    typename iterator_traits<_Iterator>::difference_type,
                    typename iterator_traits<_Iterator>::pointer,
                    typename iterator_traits<_Iterator>::reference>
{
  protected:
    _Iterator _M_current;

  public:
    typedef typename iterator_traits<_Iterator>::difference_type	difference_type;
    typedef typename iterator_traits<_Iterator>::reference	reference;
    typedef typename iterator_traits<_Iterator>::pointer	pointer;

    // CONSTRUCTOR (DEFAULT)
    __normal_iterator() : _M_current(_Iterator()) { }

    // CONSTRUCTOR (COPY)
    explicit __normal_iterator(const _Iterator& __i) : _M_current(__i) { }

    // CONSTRUCTOR (COPY)
    // Allow iterator to const_iterator conversion
    template<typename _Iterator2>
    inline __normal_iterator(const __normal_iterator<_Iterator2, _Container>& __i) 
    : _M_current(__i.base()) {}

    // 객체 내부에서 실제 쓰이고 있는 (int* 등의) 포인터를 const reference로 반환한다.
    const _Iterator& base() const { return _M_current; }

  public:
  // random access iterator에 필요한 연산자

    // DEREFERENCE

    // rvalue 혹은 lvalue로써 역참조될 수 있다. (non-const iterators)
    // 예: a = *iter; *iter = a;
    reference operator*() const { return *_M_current; }

    // rvalue로써 역참조될 수 있다.
    // 가리키는 요소가 struct이나 union일 때 사용된다. 예: iter->m;
    // (*pointer).m 로 변환된다.
    pointer operator->() const { return _M_current; }

    // INCRESMENT
    // : 이 결과는 역참조될 수 있거나 마지막을 가리키고 있다.
    __normal_iterator& operator++() { ++_M_current; return *this; }
    __normal_iterator operator++(int) { return __normal_iterator(_M_current++); }

    // DECRESMENT
    __normal_iterator& operator--() { --_M_current; return *this; }
    __normal_iterator operator--(int) { return __normal_iterator(_M_current--); }

    // ARITHMETIC
    // : 반복자 + 정수타입
    __normal_iterator operator+(const difference_type& __n) const { return __normal_iterator(_M_current + __n); } // copy constructor creates a temporary object
    __normal_iterator operator-(const difference_type& __n) const { return __normal_iterator(_M_current - __n); }

    // COMPOUND ASSIGNMENT
    __normal_iterator& operator+=(const difference_type& __n) { _M_current += __n; return *this; }
    __normal_iterator& operator-=(const difference_type& __n) { _M_current -= __n; return *this; }

    // SUBTRACT
    // : 반복자 - 반복자
    difference_type operator-(const __normal_iterator& __i) const { return _M_current - __i._M_current; }

    // OFFSET DEREFERENCE
    reference operator[](const difference_type& __n) const { return _M_current[__n]; }

}; // class __normal_iterator



// normal iterator 연산자
// 이 연산자를 사용할 수 있으려면, 1) normal iterator 타입이어야 하고, 2) 같은 container 타입이어야 한다.

// EQUALITY/UNEQUALITY
// : 같은지 비교한다. 두 반복자가 같은 시퀀스를 반복하고 있을 때 의미있다.
// (정방향반복자 필수 연산자)

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator==(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs == __rhs); }

// INEQUALITY RELATIONAL
// : 비교한다. (랜덤액세스반복자 필수 연산자)

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator<(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() < __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator>(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __rhs < __lhs; }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__rhs < __lhs); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs < __rhs); }

// ARITHMETIC
// : 정수타입 + 반복자 (멤버함수에 operator+가 있지만, 교환법칙을 지원하기 위해서 비멤버함수로도 오버로딩을 해준다.)
template<typename _Iterator, typename _Container>
inline __normal_iterator<_Iterator, _Container> operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n, const __normal_iterator<_Iterator, _Container>& __i)
{ return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }

} // namespace ft

#endif //ITERATOR_HPP