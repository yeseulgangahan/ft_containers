#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft
{

// (3. 전방선언 forward declaration) : stack안에서 friend로 선언된 두 함수가 무엇인지 미리 알려주었고, 그 함수에서 사용된 stack은 무엇인지도 미리 알려주었다.

template <typename _ValueType, typename _ContainerType = vector<_ValueType> >
class stack;

template <typename _ValueType, typename _ContainerType>
bool operator==(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType, _ContainerType>& __y);

template <typename _ValueType, typename _ContainerType>
bool operator<(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y);


template <
  typename _ValueType, // 요소의 타입
  typename _ContainerType> // underlying container 객체의 타입. 요소가 저장되는 곳. 특정되지 않았을 때 std::stack은 deque을, ft::stack은 vector를 사용한다.
class stack
{

//컨테이너 어댑터container adaptor의 일종이다: 특정 컨테이너 클래스의 캡슐화된 개체를 기본 컨테이너underlying container로 사용하는 클래스
//LIFO에서 작동하도록 설계되었다: 후입선출. 요소가 컨테이너의 한쪽 끝에서만 삽입 및 추출된다.
//요소는 back에서 push/pop된다.

// (2. 전방참조 forward reference): 뒤에서 선언된 두 함수를 friend함수로 선언한다. '나는 operator++, operator<를 나의 친구로 인정해'

  template <typename _ValueType1, typename _ContainerType1>
  friend bool operator== (const stack<_ValueType1, _ContainerType1>&, const stack<_ValueType1, _ContainerType1>&);
  template <typename _ValueType1, typename _ContainerType1>
  friend bool operator< (const stack<_ValueType1, _ContainerType1>&, const stack<_ValueType1, _ContainerType1>&);

public:
  typedef typename _ContainerType::value_type      value_type;
  typedef typename _ContainerType::size_type       size_type;
  typedef          _ContainerType                  container_type;

protected:
  // underlying container :
  // 내부적으로 컨테이너 개체를 데이터로 유지한다.
  // 이 컨테이너 개체는 인자로 전달된 컨테이너의 복사본 혹은 빈 컨테이너다.
  // 기본 컨테이너 클래스 템플릿(vector, deque, list) 혹은 다른 것일 수 있다.
  _ContainerType c;

public:
  // constructor
  explicit stack(const _ContainerType& __container = _ContainerType()) : c(__container) {}

public:
  // 멤버 함수

  bool empty() const { return c.empty(); }

  size_type size() const { return c.size(); }

  value_type &top() { return c.back(); }

  const value_type &top() const { return c.back(); }

  void push(const value_type& __x) { c.push_back(__x); }

  void pop() { c.pop_back(); }

};

// 관련 연산자 realational operators :
// __x, __y 사이의 적절한 비교작업을 수행한다.
// underlying container 개체에서 동일한 연산자를 호출한다.

// 1. 클래스 멤버함수로는 적합하지 않으나, 클래스의 private에 선언된 container 변수에 접근해야 하므로 stack에 friend함수로 선언해두었다.

template <typename _ValueType, typename _ContainerType>
bool operator==(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return __x.c == __y.c;
}

template <typename _ValueType, typename _ContainerType>
bool operator<(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return __x.c < __y.c;
}

template <typename _ValueType, typename _ContainerType>
bool operator!=(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return !(__x == __y);
}

template <typename _ValueType, typename _ContainerType>
bool operator>(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return __y < __x;
}

template <typename _ValueType, typename _ContainerType>
bool operator<=(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return !(__y < __x);
}

template <typename _ValueType, typename _ContainerType>
bool operator>=(const stack<_ValueType,_ContainerType>& __x, const stack<_ValueType,_ContainerType>& __y)
{
  return !(__x < __y);
}
}

#endif // STACK_HPP
