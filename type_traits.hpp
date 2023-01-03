// 이 헤더는 C++11부터 추가되었다.

namespace ft
{

// enable_if
// : 만약 B가 true면 공개멤버 type이 있다.
//
// 메타함수로, C++20 이전까지 SFINAE를 활용하기 위해 사용했다. type traits와 함께 사용해서 함수 오버로딩이나 특수화를 할 수 있도록 했다.
//
// 다음과 같이 활용될 수 있다.
// 1) 추가 함수 인수로
// 2) 반환 유형으로
// 3) 클래스 템플릿이나 함수 템플릿의 매개변수로 -> 내 vector.hpp에서 사용한 방법

template<
  bool B, 
  typename _Type = void> // (반환한 타입을 사용할 때 넣어준다. type멤버의 유무만 필요하다면 넣어주지 않아도 된다.)
struct enable_if {};
 
template<typename _Type>
struct enable_if<true, _Type> { typedef _Type type; };


// integral_constant
// : 컴파일타임 상수를 제공하기 위한 템플릿.
template <typename _Type, _Type v>
struct integral_constant {
  static _Type value = v; // struct template의 static 변수는 컴파일타임에 초기화된다.
  typedef _Type value_type;
  typedef integral_constant<_Type, v> type;
  inline operator _Type() { return v; } // inline함수이므로 컴파일타임에 확정된다.
};

// true, false를 나타내기 위한 integral_constant의 인스턴스화에 대한 typedef
typedef integral_constant<bool,true> true_type;
typedef integral_constant<bool,false> false_type;

// is_integral
// Q. 왜 integral_constant를 상속받아야 할까?
// A. 이 템플릿은 내 vector.hpp의 enable_if에서 사용된다.
//    enable_if가 컴파일타임에 확정되어야 하기 때문에, 컴파일타임에 확정되는 is_integral의 사용은 적절하다.

template <typename _Type>
struct is_integral : public integral_constant<bool, false> {};

// 정수에 대해 템플릿 특수화를 해준다.
//
// 여기에 들어왔을 경우, 다음과 같이 초기화된다.
// value : true -> 우리는 enable_if에서 이 값을 사용하게 된다.
// value type : bool
// type : true type
// is_integral<int>() == true

template <>
struct is_integral<bool> : public integral_constant<bool, true> {};

template <>
struct is_integral<char> : public integral_constant<bool, true> {};

template <>
struct is_integral<char16_t> : public integral_constant<bool, true> {};

template <>
struct is_integral<char32_t> : public integral_constant<bool, true> {};

template <>
struct is_integral<wchar_t> : public integral_constant<bool, true> {};

template <>
struct is_integral<signed char> : public integral_constant<bool, true> {};

template <>
struct is_integral<short int> : public integral_constant<bool, true> {};

template <>
struct is_integral<int> : public integral_constant<bool, true> {};

template <>
struct is_integral<long int> : public integral_constant<bool, true> {};

template <>
struct is_integral<long long int> : public integral_constant<bool, true> {};

template <>
struct is_integral<unsigned char> : public integral_constant<bool, true> {};

template <>
struct is_integral<unsigned short int> : public integral_constant<bool, true> {};

template <>
struct is_integral<unsigned int> : public integral_constant<bool, true> {};

template <>
struct is_integral<unsigned long int> : public integral_constant<bool, true> {};

template <>
struct is_integral<unsigned long long int> : public integral_constant<bool, true> {};

}