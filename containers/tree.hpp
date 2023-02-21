#ifndef TREE_HPP
#define TREE_HPP

#include <exception>
  // out_of_range
#include <cstddef>
  // ptrdiff_t

#include <algorithm>
  // less

#include <iterator>
  // std::distance()

#include "pair.hpp"
  // pair
  // make_pair

#include "iterator.hpp"

#include "alloc.hpp"
  // alloc_traits

#include "function.hpp"
  // select1st

#include "construct.hpp"
  // construct

#include "type_traits.hpp"
  // ft::enable_if
  // ft::is_integral

namespace ft
{ 

typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;
const _Rb_tree_Color_type _S_rb_tree_black = true;

//------------------     node     ------------------//

struct _Rb_tree_node_base
{
  typedef _Rb_tree_Color_type _Color_type;
  typedef _Rb_tree_node_base* _Base_ptr;

  _Color_type _M_color; 
  _Base_ptr _M_parent;
  _Base_ptr _M_left;
  _Base_ptr _M_right;

  static _Base_ptr _S_minimum(_Base_ptr __x)
  {
    while (__x->_M_left != 0) __x = __x->_M_left; // 제일 왼쪽 노드를 찾는다
    return __x;
  }

  static _Base_ptr _S_maximum(_Base_ptr __x)
  {
    while (__x->_M_right != 0) __x = __x->_M_right; // 제일 오른쪽 노드를 찾는다
    return __x;
  }
};

template <typename _Value>
struct _Rb_tree_node : public _Rb_tree_node_base
{
  typedef _Rb_tree_node<_Value>* _Link_type;

  _Value _M_value_field;
};

//------------------     iterator     ------------------//

struct _Rb_tree_base_iterator
{
  typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  _Base_ptr _M_node;

  void _M_increment()
  {
    // 오른쪽 서브트리가 있을 때
    if (_M_node->_M_right != 0) {
      _M_node = _M_node->_M_right; // 오른쪽 서브트리에서
      while (_M_node->_M_left != 0) // 가장 왼쪽 노드를 찾는다
        _M_node = _M_node->_M_left;
    }

    // 내 위에서 찾아야 할 때
    else { 
      _Base_ptr __target = _M_node->_M_parent;
      while (_M_node == __target->_M_right) { // 타겟이 나의 왼쪽에 있으면(나보다 작으면) 계속 찾는다
        _M_node = __target;
        __target = __target->_M_parent;
      }
      if (_M_node->_M_right != __target)
        _M_node = __target; // increment. // 최초 _M_node가 제일 큰  노드였을 때: sentinel node(_M_header)로 갱신된다.
    }
  }

  void _M_decrement()
  {
    // _M_node가 header일 때
    if (_M_node->_M_color == _S_rb_tree_red && // root가 아닐 때
        _M_node->_M_parent->_M_parent == _M_node) // _M_node가 root일 때 혹은 header일 때
      _M_node = _M_node->_M_right;
    
    // 왼쪽 서브트리가 있을 때
    else if (_M_node->_M_left != 0) {
      _M_node = _M_node->_M_left; // 왼쪽 서브트리에서
      while (_M_node->_M_right != 0) // 가장 오른쪽 노드를 찾는다
        _M_node = _M_node->_M_right;
    }

    // 내 위에서 찾아야 할 때
    else {
      _Base_ptr __target = _M_node->_M_parent;
      while (_M_node == __target->_M_left) { // 타겟이 나의 오른쪽에 있으면(나보다 크면) 계속 찾는다
        _M_node = __target;
        __target = __target->_M_parent;
      }
      _M_node = __target;
    }
  }
};

template <typename _Value, typename _Reference, typename _Pointer>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator
{
  typedef _Value value_type;
  typedef _Reference reference;
  typedef _Pointer pointer;
  typedef _Rb_tree_iterator<_Value, _Value&, _Value*> iterator;
  typedef _Rb_tree_iterator<_Value, const _Value&, const _Value*> const_iterator;
  typedef _Rb_tree_iterator<_Value, _Reference, _Pointer> _Self;
  typedef _Rb_tree_node<_Value>* _Link_type;

  _Rb_tree_iterator() {}
  _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
  _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }

  reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
  pointer operator->() const { return &(operator*()); }

  // 전위
  _Self& operator++() { _M_increment(); return *this; }
  // 후위
  _Self operator++(int) {
    _Self __tmp = *this;
    _M_increment();
    return __tmp;
  }
  
  // 전위
  _Self& operator--() { _M_decrement(); return *this; }
  // 후위
  _Self operator--(int) {
    _Self __tmp = *this;
    _M_decrement();
    return __tmp;
  }
};

//------------------     operator     ------------------//

template <class _Value, class _Reference, class _Pointer>
bool operator==(const _Rb_tree_iterator<_Value, _Reference, _Pointer>& __x,
		       const _Rb_tree_iterator<_Value, _Reference, _Pointer>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value>
bool operator==(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
		       const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value>
bool operator==(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
		       const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value, class _Reference, class _Pointer>
bool operator!=(const _Rb_tree_iterator<_Value, _Reference, _Pointer>& __x,
		       const _Rb_tree_iterator<_Value, _Reference, _Pointer>& __y) {
  return __x._M_node != __y._M_node;
}

template <class _Value>
bool operator!=(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
		       const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
  return __x._M_node != __y._M_node;
}

template <class _Value>
bool operator!=(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
		       const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
  return __x._M_node != __y._M_node;
}

//------------------     utils: insert, erase에 사용됨     ------------------//

void _Rb_tree_rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  //      [P]
  //   [1]   [x]
  //            [y] 
  //          [2] [z]
  //        
  // =>   [P]
  //   [1]   [y]
  //       [x] [z] 
  //        [2]

  // 연결을 바꾸어준다
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left != 0)
    __y->_M_left->_M_parent = __x;
  
  // 회전한 뭉치를 더 위의 노드와 연결해준다
  __y->_M_parent = __x->_M_parent; // __y에 부모 연결
  // 부모에 __y 연결
  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else /* (__x == __x->_M_parent->_M_right) */ 
    __x->_M_parent->_M_right = __y;

  // __x와 __y를 서로 연결해준다
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

void _Rb_tree_rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  // _Rb_tree_rotate_left()와 동일하다.
  
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;

  __y->_M_parent = __x->_M_parent;
  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;

  __y->_M_right = __x;
  __x->_M_parent = __y;
}

void _Rb_tree_rebalance_for_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  __x->_M_color = _S_rb_tree_red; // 삽입되는 노드는 항상 red다.
  while (__x != __root &&  // root에 도착하지 않았다면 체크를 계속한다
    __x->_M_parent->_M_color == _S_rb_tree_red) { // red-red 연속일 때

    // 1. 나는 왼쪽, 삼촌은 오른쪽
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;

      // 1-1. 삼촌이 red -> recoloring
      if (__y && __y->_M_color == _S_rb_tree_red) {
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __y->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        __x = __x->_M_parent->_M_parent;
      }
      // 1-2. 삼촌이 red가 아닐 때 -> restructing
      else {
        // case2: 할아버지까지 꺾여있을 때 -> 회전을 통해 case3으로 만들기
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;// rotate하면 부모자식 관계가 바뀌므로, 그전에 __x는 자식을 가리키도록 해준다.
          _Rb_tree_rotate_left(__x, __root);
        }
        // case3: 색을 바꾸고 회전
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        _Rb_tree_rotate_right(__x->_M_parent->_M_parent, __root);
      }
    }
    // 2. 나는 오른쪽, 삼촌은 왼쪽
    else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      // 2-1. 삼촌이 red -> recoloring
      if (__y && __y->_M_color == _S_rb_tree_red) {
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __y->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        __x = __x->_M_parent->_M_parent;
      }
      // 2-2. 삼촌이 red가 아닐 때 -> restructing
      else {
        // case2: 할아버지까지 꺾여있을 때 -> 회전을 통해 case3으로 만들기
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_right(__x, __root);
        }
        // case3: 색을 바꾸고 회전
        __x->_M_parent->_M_color = _S_rb_tree_black;
        __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
        _Rb_tree_rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = _S_rb_tree_black;// root의 색은 언제나 black이다.
}

_Rb_tree_node_base*
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* __innode,// 사용자가 erase에 넣은 노드
                             _Rb_tree_node_base*& __root,
                             _Rb_tree_node_base*& __leftmost,
                             _Rb_tree_node_base*& __rightmost)
{
  _Rb_tree_node_base* __target = __innode; // 지울 노드
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;

  /************* erase 과정 *************/

  // 1. 자식이 하나일 때
  if (__target->_M_left == 0)
    __x = __target->_M_right; // (이 때 __x는 있을 수도 없을 수도 있다.)
  else
    if (__target->_M_right == 0)
      __x = __target->_M_left;
  // 2. 자식이 둘일 때
    // 일단 successor(오른쪽 서브트리의 leftmost)를 찾아 __target로 설정한다. 
    else {                 
      __target = __target->_M_right; 
      while (__target->_M_left != 0)
        __target = __target->_M_left;
      __x = __target->_M_right;
    }
  // 2-1. target이 지울 노드가 아닌 successor를 의미할 때
  if (__target != __innode) {
    // 2-1-1. __innode 이하의 서브트리를 __innode 없이 정렬한다.
    // (1). successor(target)의 왼쪽에 삭제될 노드의 왼쪽을 연결해준다.
    __innode->_M_left->_M_parent = __target; 
    __target->_M_left = __innode->_M_left;
    // (2). __x와 __x_parent를 연결한다.
    if (__target != __innode->_M_right) {
      __x_parent = __target->_M_parent;
      if (__x) __x->_M_parent = __target->_M_parent;
      __target->_M_parent->_M_left = __x; 
      // (3). (1)과 (2)를 연결한다.
      __target->_M_right = __innode->_M_right;
      __innode->_M_right->_M_parent = __target;
    }
    else
      __x_parent = __target;  

// 2-1-2. 2-1-1에서 만든 서브트리를 innode의 부모와 연결한다.
    if (__root == __innode)
      __root = __target;
    else if (__innode->_M_parent->_M_left == __innode)
      __innode->_M_parent->_M_left = __target;
    else 
      __innode->_M_parent->_M_right = __target;
    __target->_M_parent = __innode->_M_parent;
    std::swap(__target->_M_color, __innode->_M_color); // successor는 그 자리에 있던(삭제된 노드)의 색을 물려받는다. 삭제된 노드는 successor의 색을 가진다.
    __target = __innode;
    // => 이제 2의 __target도 정말 지운(트리에서 out된) 노드를 가리킨다.
  }
  // 1-1. (__target == __innode)
  else {  
    // 1-1-1. __innode를 뺀 위아래를 연결해준다.                   
    __x_parent = __target->_M_parent;
    if (__x) __x->_M_parent = __target->_M_parent;   
    if (__root == __innode)
      __root = __x;
    else 
      if (__innode->_M_parent->_M_left == __innode)
        __innode->_M_parent->_M_left = __x;
      else
        __innode->_M_parent->_M_right = __x;
    // 1-1-2. __leftmost, __rightmost를 갱신해준다.
    if (__leftmost == __innode) {
      if (__innode->_M_right == 0) 
        __leftmost = __innode->_M_parent;
      else
        __leftmost = _Rb_tree_node_base::_S_minimum(__x);
    }
    if (__rightmost == __innode) {
      if (__innode->_M_left == 0)     
        __rightmost = __innode->_M_parent;  
      else           
        __rightmost = _Rb_tree_node_base::_S_maximum(__x);
    }
  }
    // __target은 지울 노드다.

  /************* rebalance 과정 *************/

  // 삭제되는 색이 red면 어떠한 속성도 위반하지 않는다.
  // 삭제되는 색이 black일 때만 위반을 해결한다.

  if (__target->_M_color != _S_rb_tree_red) { 
    while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
      if (__x == __x_parent->_M_left) {
        _Rb_tree_node_base* __sister = __x_parent->_M_right;
        // case1. __sister가 red일 때
        // : 회전을 통해 __sister가 black인 상황(case2, 3, 4)으로 만든다.
        if (__sister->_M_color == _S_rb_tree_red) {
          __sister->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rb_tree_rotate_left(__x_parent, __root);
          __sister = __x_parent->_M_right;
        }
        // case2. __sister가 black이고, __sister의 두 자녀 모두 black일 때
        if ((__sister->_M_left == 0 || 
             __sister->_M_left->_M_color == _S_rb_tree_black) &&
            (__sister->_M_right == 0 || 
             __sister->_M_right->_M_color == _S_rb_tree_black)) {
          __sister->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          // case3. __sister가 black이고, __sister의 왼쪽 자녀가 red
          // : 회전을 통해 case4로 만든다.
          if (__sister->_M_right == 0 || 
              __sister->_M_right->_M_color == _S_rb_tree_black) {
            if (__sister->_M_left) __sister->_M_left->_M_color = _S_rb_tree_black;
            __sister->_M_color = _S_rb_tree_red;
            _Rb_tree_rotate_right(__sister, __root);
            __sister = __x_parent->_M_right;
          }
          // case4. __sister가 black이고, __sister의 오른쪽 자녀가 red
          __sister->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__sister->_M_right) __sister->_M_right->_M_color = _S_rb_tree_black;
          _Rb_tree_rotate_left(__x_parent, __root);
          break;
        }
      } else { 
        // (오른쪽 왼쪽이 바뀌어 반복되는 코드)       
        _Rb_tree_node_base* __sister = __x_parent->_M_left;
        if (__sister->_M_color == _S_rb_tree_red) {
          __sister->_M_color = _S_rb_tree_black;
          __x_parent->_M_color = _S_rb_tree_red;
          _Rb_tree_rotate_right(__x_parent, __root);
          __sister = __x_parent->_M_left;
        }
        if ((__sister->_M_right == 0 || 
             __sister->_M_right->_M_color == _S_rb_tree_black) &&
            (__sister->_M_left == 0 || 
             __sister->_M_left->_M_color == _S_rb_tree_black)) {
          __sister->_M_color = _S_rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__sister->_M_left == 0 || 
              __sister->_M_left->_M_color == _S_rb_tree_black) {
            if (__sister->_M_right) __sister->_M_right->_M_color = _S_rb_tree_black;
            __sister->_M_color = _S_rb_tree_red;
            _Rb_tree_rotate_left(__sister, __root);
            __sister = __x_parent->_M_left;
          }
          __sister->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_rb_tree_black;
          if (__sister->_M_left) __sister->_M_left->_M_color = _S_rb_tree_black;
          _Rb_tree_rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x) __x->_M_color = _S_rb_tree_black;// 루트노드의 색은 언제나 black이다.
  }
  return __target;
}

//------------------     Rb_tree     ------------------//


template <class _Type, class _Alloc>
struct _Rb_tree_base
{
  public:
    typedef typename _Alloc_traits<_Type, _Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const { return _M_node_allocator; }

  protected:
    typename _Alloc_traits<_Rb_tree_node<_Type>, _Alloc>::allocator_type _M_node_allocator;
   
    _Rb_tree_node<_Type>* _M_header; // 메타정보를 저장한다. root, rightmost, leftmost.

    _Rb_tree_node<_Type>* _M_get_node() 
    { return _M_node_allocator.allocate(1); } // 1개의 빈 노드를 할당하여 반환한다

    void _M_put_node(_Rb_tree_node<_Type>* __p)
    { _M_node_allocator.deallocate(__p, 1); } // _p에 있는 1개의 노드를 해제한다

  public:
    // 생성자
    _Rb_tree_base(const allocator_type& __a) 
      : _M_node_allocator(__a), _M_header(0)
    { _M_header = _M_get_node(); }
    
    // 소멸자
    ~_Rb_tree_base() { _M_put_node(_M_header); }

}; // struct _Rb_tree_base


template <typename _Key, typename _Value, typename _GetKeyOfValue, typename _Compare, typename _Alloc = std::allocator<_Value> >
class _Rb_tree : protected _Rb_tree_base<_Value, _Alloc> {

  typedef _Rb_tree_base<_Value, _Alloc> _Base;

protected:
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef _Rb_tree_node<_Value> _Rb_tree_node;
  typedef _Rb_tree_Color_type _Color_type;

public:
  typedef _Key key_type;
  typedef _Value value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef _Rb_tree_node* _Link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }

protected:
  using _Base::_M_get_node;
  using _Base::_M_put_node;
  using _Base::_M_header;

protected:

  _Link_type _M_create_node(const value_type& __x)
  {
    _Link_type __tmp = _M_get_node();
    try {
      _Construct(&__tmp->_M_value_field, __x);
    }
    catch(...)
      {
	_M_put_node(__tmp);
	throw; 
      }
    return __tmp;
  }

  _Link_type _M_clone_node(_Link_type __x)
  {
    _Link_type __tmp = _M_create_node(__x->_M_value_field);
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = 0;
    __tmp->_M_right = 0;
    return __tmp;
  }

  void destroy_node(_Link_type __p)
  {
    _Destroy(&__p->_M_value_field);
    _M_put_node(__p);
  }

  template <class _Tp>
  void _Destroy(_Tp* __pointer)
  { __pointer->~_Tp(); }

protected:
  size_type _M_node_count; // keeps track of size of tree
  _Compare _M_key_compare;

  _Link_type& _M_root() const 
    { return (_Link_type&) _M_header->_M_parent; }
  _Link_type& _M_leftmost() const 
    { return (_Link_type&) _M_header->_M_left; }
  _Link_type& _M_rightmost() const 
    { return (_Link_type&) _M_header->_M_right; }

  static _Link_type& _S_left(_Link_type __x)
    { return (_Link_type&)(__x->_M_left); }
  static _Link_type& _S_right(_Link_type __x)
    { return (_Link_type&)(__x->_M_right); }
  static _Link_type& _S_parent(_Link_type __x)
    { return (_Link_type&)(__x->_M_parent); }
  static reference _S_value(_Link_type __x)
    { return __x->_M_value_field; }
  static const _Key& _S_key(_Link_type __x)
    { return _GetKeyOfValue()(_S_value(__x)); }
  static _Color_type& _S_color(_Link_type __x)
    { return (_Color_type&)(__x->_M_color); }

  static _Link_type& _S_left(_Base_ptr __x)
    { return (_Link_type&)(__x->_M_left); }
  static _Link_type& _S_right(_Base_ptr __x)
    { return (_Link_type&)(__x->_M_right); }
  static _Link_type& _S_parent(_Base_ptr __x)
    { return (_Link_type&)(__x->_M_parent); }
  static reference _S_value(_Base_ptr __x)
    { return ((_Link_type)__x)->_M_value_field; }
  static const _Key& _S_key(_Base_ptr __x)
    { return _GetKeyOfValue()(_S_value(_Link_type(__x)));} 
  static _Color_type& _S_color(_Base_ptr __x)
    { return (_Color_type&)(_Link_type(__x)->_M_color); }

  static _Link_type _S_minimum(_Link_type __x) 
    { return (_Link_type)  _Rb_tree_node_base::_S_minimum(__x); }

  static _Link_type _S_maximum(_Link_type __x)
    { return (_Link_type) _Rb_tree_node_base::_S_maximum(__x); }

public:
  typedef _Rb_tree_iterator<value_type, reference, pointer> iterator;
  typedef _Rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;

private:
  iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v);
  _Link_type _M_copy(_Link_type __x, _Link_type __p);
  void _M_clear(_Link_type __x);

public:
                                // allocation/deallocation
  _Rb_tree()
    : _Base(allocator_type()), _M_node_count(0), _M_key_compare()
    { _M_empty_initialize(); }

  _Rb_tree(const _Compare& __comp)
    : _Base(allocator_type()), _M_node_count(0), _M_key_compare(__comp) 
    { _M_empty_initialize(); }

  _Rb_tree(const _Compare& __comp, const allocator_type& __a)
    : _Base(__a), _M_node_count(0), _M_key_compare(__comp) 
    { _M_empty_initialize(); }

  _Rb_tree(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x) 
    : _Base(__x.get_allocator()),
      _M_node_count(0), _M_key_compare(__x._M_key_compare)
  { 
    if (__x._M_root() == 0)
      _M_empty_initialize();
    else {
      _S_color(_M_header) = _S_rb_tree_red;
      _M_root() = _M_copy(__x._M_root(), _M_header);
      _M_leftmost() = _S_minimum(_M_root());
      _M_rightmost() = _S_maximum(_M_root());
    }
    _M_node_count = __x._M_node_count;
  }
  ~_Rb_tree() { clear(); }
  _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& 
  operator=(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x);

private:
  void _M_empty_initialize() {
    _S_color(_M_header) = _S_rb_tree_red;// header의 color는 red, root의 color는 black.
    _M_root() = 0;
    _M_leftmost() = _M_header;
    _M_rightmost() = _M_header;
  }

public:    
                                // accessors:

  // key를 비교하는 함수를 반환한다.
  _Compare key_comp() const { return _M_key_compare; }
  
  iterator begin() { return _M_leftmost(); }
  const_iterator begin() const { return _M_leftmost(); }
  iterator end() { return _M_header; }
  const_iterator end() const { return _M_header; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { 
    return const_reverse_iterator(begin());
  } 
  bool empty() const { return _M_node_count == 0; }
  size_type size() const { return _M_node_count; }
  size_type max_size() const { return size_type(-1); }

  void swap(_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __t) {
    std::swap(_M_header, __t._M_header);
    std::swap(_M_node_count, __t._M_node_count);
    std::swap(_M_key_compare, __t._M_key_compare);
  }
    
public:
                                // insert/erase
  pair<iterator,bool> insert_unique(const value_type& __x);

  iterator insert_unique(iterator __position, const value_type& __x);

  template <class _InputIterator>
  void insert_unique(_InputIterator __first, _InputIterator __last);

  void erase(iterator __position);
  size_type erase(const key_type& __x);
  void erase(iterator __first, iterator __last);

  void clear() {
    if (_M_node_count != 0) {
      _M_clear(_M_root());
      _M_leftmost() = _M_header;
      _M_root() = 0;
      _M_rightmost() = _M_header;
      _M_node_count = 0;
    }
  }      

public:
                                // set operations:
  iterator find(const key_type& __x);
  const_iterator find(const key_type& __x) const;
  size_type count(const key_type& __x) const;
  iterator lower_bound(const key_type& __x);
  const_iterator lower_bound(const key_type& __x) const;
  iterator upper_bound(const key_type& __x);
  const_iterator upper_bound(const key_type& __x) const;
  pair<iterator,iterator> equal_range(const key_type& __x);
  pair<const_iterator, const_iterator> equal_range(const key_type& __x) const;

}; // Rb_tree


//------------------     Rb_tree 관련 연산자     ------------------//

template <class _Key, class _Value, class _GetKeyOfValue, class _Compare, class _Alloc>
bool 
operator==(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
           const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y)
{
  return __x.size() == __y.size() &&
         equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Key, class _Value, class _GetKeyOfValue, class _Compare, class _Alloc>
bool 
operator<(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
          const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y)
{
  return lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <class _Key, class _Value, class _GetKeyOfValue, class _Compare, class _Alloc>
bool 
operator!=(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
           const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
bool 
operator>(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
          const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
bool 
operator<=(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
           const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
bool 
operator>=(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
           const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y) {
  return !(__x < __y);
}


template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
void 
swap(_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x, 
     _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __y)
{
  __x.swap(__y);
}

//------------------     Rb_tree 클래스 멤버함수의 정의     ------------------//


template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& 
_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>
  ::operator=(const _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>& __x)
{
  if (this != &__x) {
                                // Note that _Key may be a constant type.
    clear();
    _M_node_count = 0;
    _M_key_compare = __x._M_key_compare;        
    if (__x._M_root() == 0) {
      _M_root() = 0;
      _M_leftmost() = _M_header;
      _M_rightmost() = _M_header;
    }
    else {
      _M_root() = _M_copy(__x._M_root(), _M_header);
      _M_leftmost() = _S_minimum(_M_root());
      _M_rightmost() = _S_maximum(_M_root());
      _M_node_count = __x._M_node_count;
    }
  }
  return *this;
}

template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>::iterator
_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>
  ::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v)
{
  _Link_type __x = (_Link_type) __x_;
  _Link_type __y = (_Link_type) __y_;
  _Link_type __z;

  // 1. __y의 왼쪽노드에 insert!
  if (__y == _M_header || __x != 0 || // (*)요소가 없거나
      _M_key_compare(_GetKeyOfValue()(__v), _S_key(__y))) {
         // (*)__y가 _M_header라면 새 노드는 여기서 leftmost가 된다.
    __z = _M_create_node(__v);
    _S_left(__y) = __z; 
    // (*)첫 노드일 때 추가설정
    if (__y == _M_header) {
      _M_root() = __z;
      _M_rightmost() = __z;
    }
    // y가 leftmost였다면, leftmost 노드를 갱신해준다
    else if (__y == _M_leftmost())
      _M_leftmost() = __z;
  }
  // 2. __y의 오른쪽 노드에 insert!
  else {
    __z = _M_create_node(__v);
    _S_right(__y) = __z;
    // y가 rightmost였다면, rightmost 노드를 갱신해준다
    if (__y == _M_rightmost())
      _M_rightmost() = __z;
  }
  _S_parent(__z) = __y;
  _S_left(__z) = 0;
  _S_right(__z) = 0;
  _Rb_tree_rebalance_for_insert(__z, _M_header->_M_parent);
  ++_M_node_count;
  return iterator(__z);
}

// insert_unique() :

// 요소 1개
template <class _Key, class _Value, class _GetKeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>::iterator, bool> // 리턴값
_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>::insert_unique(const _Value& __v)
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  bool __comp = true;
  while (__x != 0) {  // 원소가 1개 이상이면 진입
    __y = __x;
    __comp = _M_key_compare(_GetKeyOfValue()(__v), _S_key(__x));// less일 때, 대소구분이 넣은 순서대로면 true 반환
    if (__comp) {
      __x = _S_left(__x); // __v < __x
    } else {
      __x = _S_right(__x); // __v > __x
    }
  }// 리프노드(삽입할 적당한 위치)가 나올 때까지 while문 반복
  iterator __j = iterator(__y);   
  if (__comp) {// __x가 __y의 왼쪽 노드일 때
    if (__j == begin())     
      {return pair<iterator,bool>(_M_insert(__x, __y, __v), true);}
    else
      {--__j;}// 삽입할 위치로 이터레이터 이동
  }
  if (_M_key_compare(_S_key(__j._M_node), _GetKeyOfValue()(__v)))
    {return pair<iterator,bool>(_M_insert(__x, __y, __v), true);}
  return pair<iterator,bool>(__j, false);
}

// 힌트와 함께
template <class _Key, class _Val, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Val, _GetKeyOfValue, _Compare, _Alloc>::iterator 
_Rb_tree<_Key, _Val, _GetKeyOfValue, _Compare, _Alloc>
  ::insert_unique(iterator __position, const _Val& __v)
{
  if (__position._M_node == _M_header->_M_left) { // begin()
    if (size() > 0 && 
       _M_key_compare(_GetKeyOfValue()(__v), _S_key(__position._M_node)))
      return _M_insert(__position._M_node, __position._M_node, __v);
    // first argument just needs to be non-null 
    else
      return insert_unique(__v).first;
  } else if (__position._M_node == _M_header) { // end()
    if (_M_key_compare(_S_key(_M_rightmost()), _GetKeyOfValue()(__v)))
      return _M_insert(0, _M_rightmost(), __v);
    else
      return insert_unique(__v).first;
  } else {
    iterator __before = __position;
    --__before;
    if (_M_key_compare(_S_key(__before._M_node), _GetKeyOfValue()(__v)) 
        && _M_key_compare(_GetKeyOfValue()(__v), _S_key(__position._M_node))) {
      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v); 
      else
        return _M_insert(__position._M_node, __position._M_node, __v);
    // first argument just needs to be non-null 
    } else
      return insert_unique(__v).first;
  }
}

// 범위
template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc> 
  template<class _II>
void _Rb_tree<_Key,_Val,_KoV,_Cmp,_Alloc>
  ::insert_unique(_II __first, _II __last) {
  for ( ; __first != __last; ++__first)
    insert_unique(*__first);
}

// erase() :

// 1. 반복자로
template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
void _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>
  ::erase(iterator __position)
{
  _Link_type __y = 
    (_Link_type) _Rb_tree_rebalance_for_erase(__position._M_node,
                                              _M_header->_M_parent,
                                              _M_header->_M_left,
                                              _M_header->_M_right);
  destroy_node(__y);
  --_M_node_count;
}

// 2. key로
template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>::size_type 
_Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>::erase(const _Key& __x)
{
  pair<iterator,iterator> __p = equal_range(__x);
  size_type __n = 0;
  __n = std::distance(__p.first, __p.second);
  erase(__p.first, __p.second);
  return __n;
}

// 3. 범위로
template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
void _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>
  ::erase(iterator __first, iterator __last)
{
  if (__first == begin() && __last == end())
    clear();
  else
    while (__first != __last) erase(__first++);
}

// _M_copy() :
// 재귀를 이용해서 이진탐색트리를 깊은 복사한다.
template <class _Key, class _Val, class _KoV, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type 
_Rb_tree<_Key,_Val,_KoV,_Compare,_Alloc>
  ::_M_copy(
      _Link_type __x,  // __x의 복사본을
      _Link_type __p)// __x_parent에 연결하여 반환한다.
{
    // 1. 오른쪽 서브트리를 해결한다.
  // 1-1. top노드 생성 및 연결
  _Link_type __top = _M_clone_node(__x); // 복사 후 서브트리의 맨 위 노드를 먼저 만들고
  __top->_M_parent = __p; // 연결해둔다.
 
  try {
     // 1-1. 오른쪽 서브트리 복사
    if (__x->_M_right)
      __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;

    // 2. 왼쪽 서브트리로 계속 이동하면서 해결해나간다.
    __x = _S_left(__x);

    while (__x != 0) {
      // 2-1. (sub)top노드 생성 및 연결
      _Link_type __y = _M_clone_node(__x);
      __p->_M_left = __y;
      __y->_M_parent = __p;
      // 2-2. 왼쪽 서브트리 복사
      if (__x->_M_right)
        __y->_M_right = _M_copy(_S_right(__x), __y);
      __p = __y;
      __x = _S_left(__x);
    }
  }
  catch(...)
    {
      _M_clear(__top);
      throw; 
    }
  return __top;
}

// _M_clear() :
// 전체 서브트리를 지운다. (원래 제목은 _M_erase()였다.)
template <class _Key, class _Value, class _GetKeyOfValue, 
          class _Compare, class _Alloc>
void _Rb_tree<_Key,_Value,_GetKeyOfValue,_Compare,_Alloc>
  ::_M_clear(_Link_type __x)
{
  while (__x != 0) {
    _M_clear(_S_right(__x)); // __x의 오른쪽 서브트리를 모두 지운다.
    _Link_type __y = _S_left(__x); // __x의 왼쪽 서브트리는 다음에 작업하기 위해 save.
    destroy_node(__x); // 이제 __x를 지워도 된다.
    __x = __y;
  }
}

// find() :
// __x가 key인 요소가 있으면 그 요소의 반복자를 반환하고, 없으면 end()를 반환한다.
template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::find(const _Key& __k)
{
  _Link_type __y = _M_header; 
  _Link_type __x = _M_root(); 

  while (__x != 0) 
    if (!_M_key_compare(_S_key(__x), __k)) { // (*) 같으면 !less()는 true
      __y = __x;// (*) 같으면 그 값으로 갱신한 다음
      __x = _S_left(__x); // (*) 더 작은 수를 넣어서, 다음 while문부터는 여기에 들어오지 않도록 한다.
    } else
      __x = _S_right(__x);

  iterator __j = iterator(__y);   
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? 
     end() : __j;
}

template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::const_iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::find(const _Key& __k) const
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  while (__x != 0) {
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  }
  const_iterator __j = const_iterator(__y);   
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ?
    end() : __j;
}

template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::size_type 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::count(const _Key& __k) const
{
  pair<const_iterator, const_iterator> __p = equal_range(__k);
  size_type __n = 0;
  distance(__p.first, __p.second, __n);
  return __n;
}

// lower_bound() :
// 하한 반복자를 반환한다. 앞선 것으로 간주되지 않는(=동일하거나 뒤인) 요소 중 첫 번째 요소.
// 즉 [1245]에서 3을 찾을 경우 4, 2를 찾을 경우 2를 가리킨다.
template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::lower_bound(const _Key& __k)
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  while (__x != 0) 
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);

  return iterator(__y);
}

template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::const_iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::lower_bound(const _Key& __k) const
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  while (__x != 0) 
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);

  return const_iterator(__y);
}

// upper_bound() :
// 상한 반복자를 반환한다. 다음에 오는 것으로 간주되는(=뒤인) 요소 중 첫 번째 요소.
// 즉 [1245]에서 3을 찾을 경우 4, 2를 찾을 경우 4를 가리킨다.
template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::upper_bound(const _Key& __k)
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
   while (__x != 0) 
     if (_M_key_compare(__k, _S_key(__x)))
       __y = __x, __x = _S_left(__x);
     else
       __x = _S_right(__x);

   return iterator(__y);
}

template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>
typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::const_iterator 
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::upper_bound(const _Key& __k) const
{
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
   while (__x != 0) 
     if (_M_key_compare(__k, _S_key(__x)))
       __y = __x, __x = _S_left(__x);
     else
       __x = _S_right(__x);

   return const_iterator(__y);
}

// equal_range() :
// __x가 키인 모든 요소를 포함하는 범위를 반환환다.
// map은 키가 고유하므로 -> 반환되는 범위에는 요소가 1이거나 0개가 포함된다.
template <class _Key, class _Value, class _KeyOfValue, 
          class _Compare, class _Alloc>

pair<typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator,
     typename _Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>::iterator>
_Rb_tree<_Key,_Value,_KeyOfValue,_Compare,_Alloc>
  ::equal_range(const _Key& __k)
{
  return pair<iterator, iterator>(lower_bound(__k), upper_bound(__k));
}

template <class _Key, class _Value, class _KoV, class _Compare, class _Alloc>

pair<typename _Rb_tree<_Key, _Value, _KoV, _Compare, _Alloc>::const_iterator,
     typename _Rb_tree<_Key, _Value, _KoV, _Compare, _Alloc>::const_iterator>
_Rb_tree<_Key, _Value, _KoV, _Compare, _Alloc>
  ::equal_range(const _Key& __k) const
{
  return pair<const_iterator,const_iterator>(lower_bound(__k),
                                             upper_bound(__k));
}

} // namespace ft

#endif // TREE_HPP