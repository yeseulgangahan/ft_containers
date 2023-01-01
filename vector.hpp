#include <memory>
  // std::allocator<T>::construct(메모리 주소, 복사할 값):
  //   new((void *)p) T(val)를 호출한다. 즉 p가 가리키는 초기화되지 않은 메모리에, new를 이용하여 T타입의 객체를 복사생성한다.

  // std::allocator<T>::destroy:
  //   ((T*)p)->~T()를 호출한다. 즉 p가 가리키는 객체의 소멸자를 호출한다.

  // std::uninitialized_fill_n(dest의 시작지점, n, 복사할 값):
  //   값만 복사하는 대신, 객체를 해당 위치에 생성한다. 초기화되지 않은 공간을 채운다.

  // std::uninitialized_copy(source의 시작지점, source의 끝지점, dest의 시작지점):
  //   값만 복사하는 대신, 객체를 해당 위치에 생성한다. 초기화되지 않은 공간에 복사한다.
  //   dest에 복사된 범위의 finish를 가리키는 반복자를 반환한다.
  //   초기화 도중에 exception이 발생하면, 이미 생성된 객체는 소멸된다.

#include <iterator>
  // std::distance(): first부터 last까지 몇 칸인지 반환한다.

#include <limits>
  // std::numeric_limits

#include <algorithm>
  // std::copy_backward(source의 시작지점, source의 끝지점, dest범위의 끝지점):
  //   source를 마지막인자를 끝지점으로 하는 범위에 끝에서부터 복사한다.

#include "iterator.hpp"
  // ft::__normal_iterator

namespace ft
{

template <typename _Type, typename _AllocatorTypeType>atorType
class _Vector_base {

// vector base의 목적은,
// vector base의 생성자와 소멸자가 vector의 공간할당을 담당하도록 하는 것이다. (초기화 제외)
// 이렇게 함으로써  예외 안정성을 더 쉽게 달성할 수 있다.

public:
  typedef _AllocatorType allocator_type; // std::allocator<_Type>가 default이다.

  // get_allocator()는 관련 할당기를 반환한다.
  allocator_type get_allocator() const { return _M_data_allocator; }

protected:
  allocator_type _M_data_allocator; // 할당기의 인스턴스화. 메모리를 할당/해제할 때, 메모리에 요소를 생성/소멸할 때 사용된다.
  _Type* _M_start;
  _Type* _M_finish; // 마지막 data의 다음
  _Type* _M_end_of_storage; // 마지막 저장가능한 공간의 다음

  _Type* // 할당된 공간의 첫번째 요소에 대한 포인터
    _M_allocate(size_t __n) { return _M_data_allocator.allocate(__n); }
  void _M_deallocate(_Type* __p, size_t __n) { if (__p) _M_data_allocator.deallocate(__p, __n); }

public:
  // CONSTRUCTOR
  _Vector_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}

  _Vector_base(size_t __n, const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(_M_allocate(__n)), _M_finish(_M_start), _M_end_of_storage(_M_start + __n) {}

  // DESTRUCTOR
  ~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
};

template <
  typename _Type, // 요소의 타입
  typename _AllocatorTypeType = std::allocator<_Type> // 할당기 객체의 타입
> class vector : protected _Vector_base<_TatorTypeype, _AllocatorType>
{

// (멤버함수의 순서는 cppreference.com의 순서를 따라 정렬했습니다.)

private:
  typedef _Vector_base<_Type, _AllocatorType> _Base;
  typedef vector<_Type, _AllocatorType> vector_type;

public:
  // Member types

  typedef _Type  value_type; // 첫번째 템플릿 인수
  typedef _AllocatorType  allocator_type; // 두번째 템플릿 인수

  // 부호없는 정수 타입. (size_t는 unsigned long의 typedef다.)
  // defference_type의 부호없는 값을 나타낼 수 있다.
  typedef std::size_t  size_type;

  // 부호있는 정수 타입. (ptrdiff_t는 long int의 typedef다.)
  // iterator_traits<iterator>::difference_type와 일치한다.
  // (vector 클래스 내부에서는 사용되지 않았다.)
  typedef std::ptrdiff_t  difference_type;

  typedef value_type& reference;
  typedef const value_type&  const_reference;

  typedef typename  allocator_type::pointer pointer;
  typedef const typename  allocator_type::const_pointer const_pointer;

   typedef __normal_iterator<pointer, vector_type>  iterator; // 임의접근반복자. const iterator로 변형이 가능하다.
   typedef __normal_iterator<const_pointer, vector_type>  const_iterator; //임의접근반복자.

   typedef std::reverse_iterator<iterator>  reverse_iterator;
   typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;

protected:
  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_start;
  using _Base::_M_finish;
  using _Base::_M_end_of_storage;

public:
  // 멤버함수: 요소 접근 관련

  // at() :
  // 벡터에서 위치 __n에 있는 요소에 대한 참조를 반환한다.
  // 자동적으로 __n이 범위 내에 있는지 확인하고, 아닐 경우 out_of_range를 던진다. 범위 확인을 하지 않는 operator[]와 대조된다.
  // Exception safety: strong guarantee.
  reference at(size_type __n) { _M_range_check(__n); return (*this)[__n]; }
  const_reference at(size_type __n) const { _M_range_check(__n); return (*this)[__n]; } // 벡터 객체가 const-qualified인 경우

  // operator[] :
  // at과 동일하되, 범위 확인을 하지 않는다.
  // 범위 밖의 위치를 요구했을 때는 out_of_range exception을 던지지만, undefined behavior이므로 이식성을 고려한다면 사용하지 않아야 한다.
  // Exception safety: 벡터의 size가 __n보다 클 경우 no-throw guarantee, 나머지 경우는 undefined behavior.
  reference operator[](size_type __n) { return *(begin() + __n); }
  const_reference operator[](size_type __n) const { return *(begin() + __n); }

  // front() :
  // vector의 첫번째 요소에 대한 참조를 반환한다.
  reference front() { return *begin(); }
  const_reference front() const { return *begin(); }

  // back() :
  // vector의 마지막 요소에 대한 참조를 반환한다.
  reference back() { return *(end() - 1); }
  const_reference back() const { return *(end() - 1); }

  // data() :
  // vector가 내부적으로 사용하는 메모리 배열에 대한 포인터를 반환한다.
  value_type* data() { return _M_start; }
  value_type* data() const { return _M_start; } // 벡터 객체가 const-qualified인 경우

public:
  // 멤버함수: 반복자 관련

  // begin() :
  // 첫번째 요소를 가리키는 반복자를 반환한다.
  iterator begin() { return iterator (_M_start); }
  const_iterator begin() const { return const_iterator (_M_start); }

  // end() :
  // '마지막 요소의 다음 요소'을 가리키는 반복자를 반환한다.
  iterator end() { return iterator (_M_finish); }
  const_iterator end() const { return const_iterator (_M_finish); }

  // rbegin() :
  // 마지막 요소를 가리키는 역반복자를 반환한다.
  // (역반복자 입장에서는 마지막 요소가 첫번째 요소다.)
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  // rend() :
  // '첫번째 요소의 (이론적) 앞 요소'을 가리키는 반복자를 반환한다.
  // (역반복자 입장에서는 '첫번째 요소의 앞 요소'가 '마지막 요소의 다음 요소'다.)
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

 public:
 // 멤버함수: capacity 관련

  // empty() :
  // vector가 비었으면(즉 size가 0이면) true, 아니면 false를 반환한다.
  bool empty() const { return begin() == end(); }

  // size() :
  // vector의 요소의 수를 반환한다.
  size_type size() const { return size_type(end() - begin()); }

  // max_size() :
  // vector가 저장할 수 있는 요소의 최대의 개수를 반환한다. 단, 그 크기에 도달하기 전에 어느 시점에서든지 할당이 불가능해질 수 있다.
  // Q. 왜 std::numeric_limits<difference_type>::max()를 사용했나?
  // A. 만약 32-bit 시스템이라면, 이론적으로 4Gb == 2^32bits만큼 할당할 수 있다.
  //    이 수는 가능한 주소값의 개수로 구할 수 있는데, 주소값의 개수가 곧 할당할 수 있는 공간의 개수이기 때문이다.
  size_type max_size() const { return std::numeric_limits<difference_type>::max() / sizeof(_Type); }

  // reserve() :
  // 동작순서: 새 메모리를 할당 → 원래 내용을 새 메모리에 복사 → 원래 메모리를 destroy, deallocate → 새로운 메모리를 start, finish, end_of_storage에 저장
  void reserve(size_type __n) {
    if (capacity() < __n) {
      const size_type __old_size = size();
      pointer __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
      _M_destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_finish = __tmp + __old_size;
      _M_end_of_storage = _M_start + __n;
    }
  }

  // capacity() :
  // 현재 할당되어 있는 메모리에서 저장할 수 있는 요소의 최대 수
  size_type capacity() const { return size_type(const_iterator(_M_end_of_storage) - begin()); }

 public:
  // 멤버함수: 변환자

  // clear() :
  // vector에서 (이미 destroy된) 원소를 모두 제거하고, size를 0으로 만든다.
  // 재할당, capacity의 변경은 이 함수에서 일어나지 않는다. 할당을 해제하고 capacity를 0으로 만들고 싶다면 임시객체와의 swap을 활용한다. ( x에 대해: vector<T>().swap(x) )
  void clear() { erase(begin(), end()); }

  // insert() :

  // insert1. 단일 요소 :
  // 새로 insert된 요소를 가리키는 반복자를 반환한다.
  iterator insert(iterator __position, const _Type& __x) {
    size_type __n = __position - begin();
    if (_M_finish != _M_end_of_storage && __position == end()) { // case1: 맨 끝에 넣는 경우
      _M_data_allocator.construct(_M_finish, __x);
      ++_M_finish;
    }
    else // case2: 중간에 넣는 경우
      _M_insert_aux(iterator(__position), __x);
    return begin() + __n;
  }
  // insert2. 채우기 :
  // __n개의 __x로 __pos부터 채운다.
  void insert (iterator __pos, size_type __n, const _Type& __x)
    { _M_fill_insert(__pos, __n, __x); }

  // insert3. 범위 :
  // Check whether it's an integral type.  If so, it's not an iterator.
  template<typename _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
      typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
      _M_insert_dispatch(__pos, __first, __last, _Integral());
    }

  template <typename _Integer>
    void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, __true_type)
    { _M_fill_insert(__pos, static_cast<size_type>(__n), static_cast<_Type>(__val)); }

  template<typename _InputIterator>
    void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, __false_type) {
    typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
      _M_range_insert(__pos, __first, __last, _IterCategory());
    }

  // erase() :

  // erase1.
  iterator erase(iterator __position) {
    if (__position + 1 != end())
      copy(__position + 1, end(), __position);
    --_M_finish;
    _M_destroy(_M_finish);
    return __position;
  }

  // erase2.
  iterator erase(iterator __first, iterator __last) {
    iterator __i(copy(__last, end(), __first));
    _M_destroy(__i, end());
    _M_finish = _M_finish - (__last - __first);
    return __first;
  }

  // push_back() :
  // 맨 끝에 요소를 넣는다.
  // 재할당 시에는 insert와 동일한 함수를 사용하지만, 좀더 간단하게 할 수 있는 부분은 따로 구현한다.

  // push_back1. 특정 객체의 복사본 삽입
  void push_back(const _Type& __x) {
    if (_M_finish != _M_end_of_storage) { // case1. 빈 공간이 있는 경우
      _M_data_allocator.construct(_M_finish, __x);
      ++_M_finish;
    }
    else // case2. 재할당이 필요한 경우
      _M_insert_aux(end(), __x);
  }

// push_back2. 빈 객체 삽입
  void push_back() {
    if (_M_finish != _M_end_of_storage) { // case1. 빈 공간이 있는 경우
      _M_data_allocator.construct(_M_finish);
      ++_M_finish;
    }
    else // case2. 재할당이 필요한 경우
      _M_insert_aux(end());
  }

  // pop_back() :
  // 마지막 요소를 제거destroy한다.
  void pop_back() {
    --_M_finish;
    _M_destroy(_M_finish);
  }

  // resize() :
  // vector가 n개의 요소를 가지도록 size를 변경한다.
  // erase()나 insert() 멤버함수를 이용한다.
  void resize(size_type __new_size, const _Type& __x = _Type()) {
    if (__new_size < size())
      erase(begin() + __new_size, end());
    else
      insert(end(), __new_size - size(), __x);
  }

  // swap() :
  // 인자로 들어온 (타입이 같은) 또다른 vector와 가지고 있던 메모리를 바꾸어 가진다.
  // 비멤버함수 swap이 vector에 대해 사용될 경우 vector의 멤버함수 swap을 쓰도록 오버로딩되어 있다.
  void swap(vector<_Type, _AllocatorType>& __x) {
    std::swap(_M_start, __x._M_start);
    std::swap(_M_finish, __x._M_finish);
    std::swap(_M_end_of_storage, __x._M_end_of_storage);
  }

public:
  // CONSTRUCTOR
  // : vector를 생성하고 초기화한다.
  // Exception safety: Strong guarantee

  // constructor1. default constructor :
  // 빈 vector를 생성한다.
  explicit vector(
    const allocator_type& __a = allocator_type() // 할당기 객체. vector는 할당기의 복사생성자를 호출하여 이 객체의 복사본을 얻는다.
  ) : _Base(__a) {}

  // constructor2. 채우기 :
  // __n개의 요소를 가진 vector를 생성한다. 각 요소는 __value의 복사본이다.
  explicit vector(
    size_type __n, // 초기 vector size
    const _Type& __value, // vector를 채울 값
    const allocator_type& __a = allocator_type()
  ) : _Base(__n, __a)
    { _M_finish = std::uninitialized_fill_n(_M_start, __n, __value); }

  // constructor3. 범위 :
  // 단, 만약 [__first, __last) 가 유효하지 않다면, 그것은 undefined behavior다.
  template <typename _InputIterator>
    vector(
      typename std::enable_if<!std::is_integral<_InputIterator>::value,// 정수인지 확인한다. 정수라면 반복자가 아니라는 뜻이므로 SFINE에 의해 constructor2를 찾아간다.
      _InputIterator>::type  __first,
      _InputIterator __last,
      const allocator_type& __a = allocator_type()
    ) : _Base(__a) {
    typedef typename std::iterator_traits<_InputIterator>::iterator_category __IteratorCategory;
    _M_range_initialize(__first, __last, __IteratorCategory());
  }

  // constructor4. copy constructor
  vector(
    const vector& __x // 같은 타입(즉 템플릿 인수가 같은)의 또다른 vector 객체.
  ) : _Base(__x.size(), __x.get_allocator())
    { _M_finish = std::uninitialized_copy(__x.begin(), __x.end(), _M_start); }

  // DESTRUCTOR
  ~vector()
  { _M_destroy(_M_start, _M_finish); }

  // ASSIGN OPERATOR
  vector<_Type,_AllocatorType>& operator=(const vector<_Type, _AllocatorType>& __x) {
  if (&__x != this) {
    const size_type __xlen = __x.size();
    if (__xlen > capacity()) {
      pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
      _M_destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_end_of_storage = _M_start + __xlen;
    }
    else if (size() >= __xlen) {
      iterator __i(copy(__x.begin(), __x.end(), begin()));
      _M_destroy(__i, end());
    }
    else {
      copy(__x.begin(), __x.begin() + size(), _M_start);
      std::uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
    }
    _M_finish = _M_start + __xlen;
  }
  return *this;
}

  // ASSIGN FUNCTION

  // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.

  // assign1.
  void assign(size_type __n, const _Type& __val) { _M_fill_assign(__n, __val); }

  // assign2.
  template<typename _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  //ALLOCATOR
  // returns the associated allocator
  allocator_type get_allocator() const { return _Base::get_allocator(); }


//----------------------------------------------------------------------------------------

protected:
  // _M_FUNCTIONS

  // _M_destroy() :

  // _M_destroy1. 범위
  template <typename _ForwardIterator>
  void _M_destroy(_ForwardIterator __first, _ForwardIterator __last) {
    for (; __first != last; ++first)
      _M_data_allocator.destroy(__first);
  }

  // _M_destroy2. 단일 요소
  template <typename _ForwardIterator>
  void _M_destory(_ForwardIterator __position) {
      _M_data_allocator.destroy(__position);
  }

  // _M_range_check() :
    void _M_range_check(size_type __n) const {
      if (__n >= this->size())
        __throw_out_of_range("vector");
    }

  // _M_range_initialize() :
  //  vector를 [__first, __last) 구간의 값과 동일하게 (값을 복사하여) 초기화해준다.

  // _M_range_initialize1. input iterator
  template <typename _InputIterator>
  void _M_range_initialize(_InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
    for ( ; __first != __last; ++__first) // forward iterator가 아닐 경우 하나하나 할당해주어야 한다.
      push_back(*__first);
  }

  // _M_range_initialize2. forward iterator와 forward iterator의 파생클래스
  template <typename _ForwardIterator>
  void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
    size_type __n = std::distance(__first, __last);
    _M_start = _M_allocate(__n);
    _M_end_of_storage = _M_start + __n;
    _M_finish = std::uninitialized_copy(__first, __last, _M_start);
  }

  // _M_allocate_and_copy() :
  template <typename _ForwardIterator>
  pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
    pointer __result = _M_allocate(__n); // (여기서 예외가 던져진 경우, 사용자가 캐치해야 한다.)
    try {
      std::uninitialized_copy(__first, __last, __result); // (destroy까지는 해준다.)
      return __result;
    }
    catch(...) {
      _M_deallocate(__result, __n);
      throw; // (try에서 발생한 예외를 재발생시킨다.)
    }
  }

  // _M_insert_aux() :
  // 해당 위치에 객체를 생성하여 삽입한다. (aux는 보조라는 의미)

  // _M_insert_aux1. 특정 객체 삽입
  // __position에 __x의 복사본을 넣는다.
  void _M_insert_aux(iterator __position, const _Type& __x)
  {
    if (_M_finish != _M_end_of_storage) { // case1: 아직 빈 자리가 있는 경우
      // if, old: [12340], *position == 2, x == A (0 represents empty space)

      _M_data_allocator.construct(_M_finish, *(_M_finish - 1)); // [12344]
      ++_M_finish;
      std::copy_backward(__position, iterator(_M_finish - 2), iterator(_M_finish - 1)); // [12234] (값만 복사하므로 destroy는 필요없다.)
      _Type __x_copy = __x; // 지역변수로 복사생성
      *__position = __x_copy; // [1A234]
    }

    else { // case2: 재할당이 필요한 경우
      // if, old: [1234], *position == 2, x == A

      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1; // 재할당정책: 기존 크기의 2배
      iterator __new_start(_M_allocate(__len)); // new: [00000000] (0 represents empty space)
      iterator __new_finish(__new_start);
      try {
        __new_finish = std::uninitialized_copy(iterator(_M_start), __position, __new_start); // new: [10000000]
        _M_data_allocator.construct(__new_finish.base(), __x); // new: [1A000000]
        ++__new_finish;
        __new_finish = std::uninitialized_copy(__position, iterator(_M_finish), __new_finish); // new: [1A234000]
      }
      catch(...) {
        _M_destroy(__new_start,__new_finish);
        _M_deallocate(__new_start.base(),__len);
        throw;
        }
      _M_destroy(begin(), end());
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start.base();
      _M_finish = __new_finish.base();
      _M_end_of_storage = __new_start.base() + __len;
    }
  }

  // _M_insert_aux2. 빈 객체 삽입
  // _M_insert_aux1과 동일하게 작동하되, __position에 빈 객체를 생성한다.
  void _M_insert_aux(iterator __position)
  {
    if (_M_finish != _M_end_of_storage) {
      _M_data_allocator.construct(_M_finish, *(_M_finish - 1));
      ++_M_finish;
      copy_backward(__position, iterator(_M_finish - 2),
      iterator(_M_finish - 1));
      *__position = _Type();
    }
    else {
      const size_type __old_size = size();
      const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
      pointer __new_start = _M_allocate(__len);
      pointer __new_finish = __new_start;
      try {
        __new_finish = std::uninitialized_copy(iterator(_M_start), __position,
          __new_start);
        _M_data_allocator.construct(__new_finish);
        ++__new_finish;
        __new_finish = std::uninitialized_copy(__position, iterator(_M_finish),
          __new_finish);
      }
      catch(...) {
        _M_destroy(__new_start,__new_finish);
        _M_deallocate(__new_start,__len);
        throw;
      }
      _M_destroy(begin(), end());
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __new_start;
      _M_finish = __new_finish;
      _M_end_of_storage = __new_start + __len;
    }
  }

  // _M_assign_dispatch():

  template<typename _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
  { _M_fill_assign((size_type) __n, (_Type) __val); }

  template<typename _InputIter>
  void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type) {
    typedef typename iterator_traits<_InputIter>::iterator_category _IterCategory;
    _M_assign_aux(__first, __last, _IterCategory());
  }


  // _M_fill_assign():
  void _M_fill_assign(size_t __n, const value_type& __val)
  {
    if (__n > capacity()) {
      vector<_Type, _AllocatorType> __tmp(__n, __val, get_allocator());
      __tmp.swap(*this);
    }
    else if (__n > size()) {
      fill(begin(), end(), __val);
      _M_finish = std::uninitialized_fill_n(_M_finish, __n - size(), __val);
    }
    else
      erase(fill_n(begin(), __n, __val), end());
  }

  // _M_assign_aux() :

  // _M_assign_aux1.
  template <typename _InputIter>
  void _M_assign_aux(_InputIter __first, _InputIter __last,
                                          input_iterator_tag) {
    iterator __cur(begin());
    for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
      *__cur = *__first;
    if (__first == __last)
      erase(__cur, end());
    else
      insert(end(), __first, __last);
  }

  // _M_assign_aux2.
  template <typename _ForwardIter>
  void _M_assign_aux(_ForwardIter __first, _ForwardIter __last,
                                    forward_iterator_tag) {
    size_type __len = 0;
    std::distance(__first, __last, __len);

    if (__len > capacity()) {
      pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
      _M_destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_end_of_storage = _M_finish = _M_start + __len;
    }
    else if (size() >= __len) {
      iterator __new_finish(copy(__first, __last, _M_start));
      _M_destroy(__new_finish, end());
      _M_finish = __new_finish.base();
    }
    else {
      _ForwardIter __mid = __first;
      advance(__mid, size());
      copy(__first, __mid, _M_start);
      _M_finish = std::uninitialized_copy(__mid, __last, _M_finish);
    }
  }

  // _M_fill_insert() :
  void _M_fill_insert(iterator __position, size_type __n, const _Type& __x)
  {
    if (__n != 0) {
      if (size_type(_M_end_of_storage - _M_finish) >= __n) {
        _Type __x_copy = __x;
        const size_type __elems_after = end() - __position;
        iterator __old_finish(_M_finish);
        if (__elems_after > __n) {
          std::uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
          _M_finish += __n;
          copy_backward(__position, __old_finish - __n, __old_finish);
          fill(__position, __position + __n, __x_copy);
        }
        else {
          std::uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
          _M_finish += __n - __elems_after;
          std::uninitialized_copy(__position, __old_finish, _M_finish);
          _M_finish += __elems_after;
          fill(__position, __old_finish, __x_copy);
        }
      }
      else {
        const size_type __old_size = size();
        const size_type __len = __old_size + max(__old_size, __n);
        iterator __new_start(_M_allocate(__len));
        iterator __new_finish(__new_start);
        try {
          __new_finish = std::uninitialized_copy(begin(), __position, __new_start);
          __new_finish = std::uninitialized_fill_n(__new_finish, __n, __x);
          __new_finish
            = std::uninitialized_copy(__position, end(), __new_finish);
        }
        catch(...)
  {
    _M_destroy(__new_start,__new_finish);
    _M_deallocate(__new_start.base(),__len);
    __throw_exception_again;
  }
        _M_destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start.base();
        _M_finish = __new_finish.base();
        _M_end_of_storage = __new_start.base() + __len;
      }
    }
  }

  // _M_range_insert() :

  // _M_range_insert1.
  template <typename _InputIterator>
  void _M_range_insert(iterator __pos, _InputIterator __first, _InputIterator __last, input_iterator_tag)
  {
    for ( ; __first != __last; ++__first) {
      __pos = insert(__pos, *__first);
      ++__pos;
    }
  }

  // _M_range_insert2.
  template <typename _ForwardIterator>
  void _M_range_insert(iterator __position, _ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag)
  {
    if (__first != __last) {
      size_type __n = 0;
      std::distance(__first, __last, __n);
      if (size_type(_M_end_of_storage - _M_finish) >= __n) {
        const size_type __elems_after = end() - __position;
        iterator __old_finish(_M_finish);
        if (__elems_after > __n) {
          std::uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
          _M_finish += __n;
          copy_backward(__position, __old_finish - __n, __old_finish);
          copy(__first, __last, __position);
        }
        else {
          _ForwardIterator __mid = __first;
          advance(__mid, __elems_after);
          std::uninitialized_copy(__mid, __last, _M_finish);
          _M_finish += __n - __elems_after;
          std::uninitialized_copy(__position, __old_finish, _M_finish);
          _M_finish += __elems_after;
          copy(__first, __mid, __position);
        }
      }
      else {
        const size_type __old_size = size();
        const size_type __len = __old_size + max(__old_size, __n);
        iterator __new_start(_M_allocate(__len));
        iterator __new_finish(__new_start);
        try {
          __new_finish = std::uninitialized_copy(iterator(_M_start),
            __position, __new_start);
          __new_finish = std::uninitialized_copy(__first, __last, __new_finish);
          __new_finish
            = std::uninitialized_copy(__position, iterator(_M_finish), __new_finish);
        }
        catch(...)
  {
    _M_destroy(__new_start,__new_finish);
    _M_deallocate(__new_start.base(), __len);
    __throw_exception_again;
  }
        _M_destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start.base();
        _M_finish = __new_finish.base();
        _M_end_of_storage = __new_start.base() + __len;
      }
    }

  }; // class vector


// 비멤버함수

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator==(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y)
  {
    return __x.size() == __y.size() &&
          equal(__x.begin(), __x.end(), __y.begin());
  }

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator<(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y)
  {
    return lexicographical_compare(__x.begin(), __x.end(),
                                  __y.begin(), __y.end());
  }

  // swap을 오버로드한다.
  // vector의 멤버함수 swap을 쓰도록 하여 swap의 동작을 vector에 최적화한다.
  template <typename _Type, typename _AllocatorType>
  inline void swap(vector<_Type, _AllocatorType>& __x, vector<_Type, _AllocatorType>& __y)
  {
    __x.swap(__y);
  }

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator!=(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y) {
    return !(__x == __y);
  }

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator>(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y) {
    return __y < __x;
  }

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator<=(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y) {
    return !(__y < __x);
  }

  template <typename _Type, typename _AllocatorType>
  inline bool
  operator>=(const vector<_Type, _AllocatorType>& __x, const vector<_Type, _AllocatorType>& __y) {
    return !(__x < __y);
  }

};

} // namespace ft
