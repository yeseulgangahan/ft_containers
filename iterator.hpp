#include "iterator_base_types.hpp"
  // ft::iterator_traits

namespace ft
{

// reverse_iterator
// : 반복자 어댑터. 양방향 또는 임의엑세스 반복자가 범위를 반복하는 방향을 반대로 바꾼다.
// 기본(원래)반복자는 내부적으로 유지된다. base()를 사용하면 기본반복자를 얻을 수 있다.


template<typename _Iterator>
class reverse_iterator 
  : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                    typename iterator_traits<_Iterator>::value_type,
                    typename iterator_traits<_Iterator>::difference_type,
                    typename iterator_traits<_Iterator>::pointer,
                    typename iterator_traits<_Iterator>::reference>
{
  protected:
    _Iterator current; // base 반복자

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
    // A. 빈 이니셜라이저로 개체를 생성할 때 수행되는 초기화.
    //    여기서는 Iterator()가 호출됨을 의미하고, Iterator가 사용자정의가 아닌 default constructor가 있는 타입이므로 값은 0으로 초기화된다.
    //
    // Q. 이 생성자는 어디에 쓸까?
    // A. 우선 역반복자는 양방향반복자인데, 양방향반복자는 기본생성자를 지원해야 하기 때문이다.
    // 
    //
    reverse_iterator() {}
    
    // CONSTRUCTOR2. initialization
    explicit reverse_iterator(iterator_type __x): current(__x) {}
    
    // CONSTRUCTOR3. copy
    reverse_iterator(const reverse_iterator& __x): current(__x.current) { }

    template<typename _Iter>
    reverse_iterator(const reverse_iterator<_Iter>& __x): current(__x.base()) {}
  
    iterator_type 
    base() const { return current; }

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
    
    reverse_iterator 
    operator+(difference_type __n) const 
    { return reverse_iterator(current - __n); }

    reverse_iterator& 
    operator+=(difference_type __n) {
      current -= __n;
      return *this;
    }

    reverse_iterator 
    operator-(difference_type __n) const 
    { return reverse_iterator(current + __n); }

    reverse_iterator& operator-=(difference_type __n) {
      _M_current += __n;
      return *this;
    }

    reference operator[](difference_type __n) const { return *(*this + __n); }  
}; 

template<typename _Iterator>
  inline bool 
  operator==(const reverse_iterator<_Iterator>& __x, 
        const reverse_iterator<_Iterator>& __y) 
  { return __x.base() == __y.base(); }

template<typename _Iterator>
  inline bool 
  operator<(const reverse_iterator<_Iterator>& __x, 
      const reverse_iterator<_Iterator>& __y) 
  { return __y.base() < __x.base(); }

template<typename _Iterator>
  inline bool 
  operator!=(const reverse_iterator<_Iterator>& __x, 
        const reverse_iterator<_Iterator>& __y) 
  { return !(__x == __y); }

template<typename _Iterator>
  inline bool 
  operator>(const reverse_iterator<_Iterator>& __x, 
      const reverse_iterator<_Iterator>& __y) 
  { return __y < __x; }

template<typename _Iterator>
  inline bool 
  operator<=(const reverse_iterator<_Iterator>& __x, 
  const reverse_iterator<_Iterator>& __y) 
  { return !(__y < __x); }

template<typename _Iterator>
  inline bool 
  operator>=(const reverse_iterator<_Iterator>& __x, 
        const reverse_iterator<_Iterator>& __y) 
  { return !(__x < __y); }

template<typename _Iterator>
  inline typename reverse_iterator<_Iterator>::difference_type
  operator-(const reverse_iterator<_Iterator>& __x, 
      const reverse_iterator<_Iterator>& __y) 
  { return __y.base() - __x.base(); }

template<typename _Iterator>
  inline reverse_iterator<_Iterator> 
  operator+(typename reverse_iterator<_Iterator>::difference_type __n,
      const reverse_iterator<_Iterator>& __x) 
  { return reverse_iterator<_Iterator>(__x.base() - __n); }


// normal_iterator
// 1) it does not change the semantics of any of the operators of its iterator parameter.
//
// 2) Its primary purpose is to convert an iterator that is not a class, into an iterator that is a class.
//		e.g. a pointer
//
// 3) The _Container parameter exists solely so that different containers
// 		using this template can instantiate different types, even if the _Iterator parameter is the same.
//		e.g. _normal_iterator< char*, vector<char> > != _normal_iterator< char*, basic_string<char> >

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

    // CONSTRUCTOR (COPY): Allow iterator to const_iterator conversion //질문:???
    template<typename _Iter>
    inline __normal_iterator(const __normal_iterator<_Iter, _Container>& __i) : _M_current(__i.base()) { }

    // copy assign: default
    // destructor: default

    // 객체 내부에서 실제 쓰이고 있는 (int* 등의) 포인터를 const reference로 반환한다.
    const _Iterator& base() const { return _M_current; }

  public:
  // random access iterator requirements

    // DEREFERENCE

    // can be dereferenced as an rvalue or as an lvalue (for non-const iterators)
    // e.g. a = *iter; *iter = a;
    reference operator*() const { return *_M_current; }

    // can be dereferenced as an rvalue
    // use if the type is structure or union. translate as (*pointer).m
    // e.g. iter->m;
    pointer operator->() const { return _M_current; }

    // INCRESMENT
    // : The result is either also dereferenceable or a past-the-end iterator
    __normal_iterator& operator++() { ++_M_current; return *this; }
    __normal_iterator operator++(int) { return __normal_iterator(_M_current++); }

    // DECRESMENT
    __normal_iterator& operator--() { --_M_current; return *this; }
    __normal_iterator operator--(int) { return __normal_iterator(_M_current--); }

    // ARITHMETIC
    // : between an iterator and an integer value
    __normal_iterator operator+(const difference_type& __n) const { return __normal_iterator(_M_current + __n); } // copy constructor creates a temporary object
    __normal_iterator operator-(const difference_type& __n) const { return __normal_iterator(_M_current - __n); }

    // COMPOUND ASSIGNMENT
    __normal_iterator& operator+=(const difference_type& __n) { _M_current += __n; return *this; }
    __normal_iterator& operator-=(const difference_type& __n) { _M_current -= __n; return *this; }

    // SUBTRACT
    // : subtracting an iterator from another
    difference_type operator-(const __normal_iterator& __i) const { return _M_current - __i._M_current; }

    // OFFSET DEREFERENCE
    reference operator[](const difference_type& __n) const { return _M_current[__n]; }

}; // class __normal_iterator



// normal iterator operator
// they shoulde be 1) normal iterator type, 2) same container type to use this operators

// EQUALITY/UNEQUALITY
// : Can be compared for equivalence. meaningful when both iterator values iterate over the same underlying sequence. (Forward iterator requirements)

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator==(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() == __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs, const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return !(__lhs == __rhs); }

// INEQUALITY RELATIONAL
// : Can be compared. (Random access iterator requirements)

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
// : between an iterator and an integer value (To support the commutative property.)

template<typename _Iterator, typename _Container>
inline __normal_iterator<_Iterator, _Container> operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n, const __normal_iterator<_Iterator, _Container>& __i)
{ return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }

} // namespace ft
