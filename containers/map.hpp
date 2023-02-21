#ifndef MAP_HPP
#define MAP_HPP

#include "tree.hpp"

namespace ft
{
// map
// 특정 순서에 따라 키값과 매핑된 값의 조합으로 형성된 요소를 저장하는 연관 컨테이너.
template <
  typename _KeyType, // 요소를 정렬하고, 고유하게 식별하는 데에 사용된다. 
  typename _MappedType, // 내가 저장하는 실제 데이터
  typename _KeyCompare = std::less<_KeyType>, // 요소의 순서를 비교하고, 동일한지 결정할 때 사용한다.
  typename _Alloc = std::allocator<pair<const _KeyType, _MappedType> >
> class map {
public:
  typedef _KeyType key_type;
  typedef _MappedType data_type;
  typedef _MappedType mapped_type;
  typedef pair<const _KeyType, _MappedType> value_type; // 노드(pair)의 타입
  typedef _KeyCompare key_compare;

  class value_compare
  {
  friend class map<_KeyType, _MappedType, _KeyCompare, _Alloc>;

  protected :
    _KeyCompare comp;
    value_compare(_KeyCompare __c) : comp(__c) {}

  public:
    bool operator()(const value_type& __x, const value_type& __y) const {
      return comp(__x.first, __y.first);
    }
  };

private:
  typedef _Rb_tree<
    key_type, 
    value_type, 
    _Select1st<value_type>, // value_type에서 first(즉 key)를 뽑아내주는 functor(struct 형태)
    key_compare, 
    _Alloc> 
    _Rb_tree_type;
  _Rb_tree_type _M_tree; // 이것이 실제 map
public:
  typedef typename _Rb_tree_type::pointer pointer;
  typedef typename _Rb_tree_type::const_pointer const_pointer;
  typedef typename _Rb_tree_type::reference reference;
  typedef typename _Rb_tree_type::const_reference const_reference;
  typedef typename _Rb_tree_type::iterator iterator;
  typedef typename _Rb_tree_type::const_iterator const_iterator;
  typedef typename _Rb_tree_type::reverse_iterator reverse_iterator;
  typedef typename _Rb_tree_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rb_tree_type::size_type size_type;
  typedef typename _Rb_tree_type::difference_type difference_type;
  typedef typename _Rb_tree_type::allocator_type allocator_type;

  // allocation/deallocation

  map() : _M_tree(_KeyCompare(), allocator_type()) {}
  explicit map(const _KeyCompare& __comp,
               const allocator_type& __a = allocator_type())
    : _M_tree(__comp, __a) {}

  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last)
    : _M_tree(_KeyCompare(), allocator_type())
    { _M_tree.insert_unique(__first, __last); }

  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last, const _KeyCompare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_tree(__comp, __a) { _M_tree.insert_unique(__first, __last); }
  map(const map<_KeyType, _MappedType, _KeyCompare, _Alloc>& __x) : _M_tree(__x._M_tree) {}

  map<_KeyType, _MappedType, _KeyCompare, _Alloc>&
  operator=(const map<_KeyType, _MappedType, _KeyCompare, _Alloc>& __x)
  {
    _M_tree = __x._M_tree;
    return *this; 
  }

  // accessors:

  key_compare key_comp() const { return _M_tree.key_comp(); }
  value_compare value_comp() const { return value_compare(_M_tree.key_comp()); }
  allocator_type get_allocator() const { return _M_tree.get_allocator(); }

  iterator begin() { return _M_tree.begin(); }
  const_iterator begin() const { return _M_tree.begin(); }
  iterator end() { return _M_tree.end(); }
  const_iterator end() const { return _M_tree.end(); }
  reverse_iterator rbegin() { return _M_tree.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_tree.rbegin(); }
  reverse_iterator rend() { return _M_tree.rend(); }
  const_reverse_iterator rend() const { return _M_tree.rend(); }
  bool empty() const { return _M_tree.empty(); }
  size_type size() const { return _M_tree.size(); }
  size_type max_size() const { return _M_tree.max_size(); }

  // opearator[] :
  // 해당 키를 이용하여 직접 접근할 수 있다.
  _MappedType& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
      __i = insert(__i, value_type(__k, _MappedType()));
    return (*__i).second;
  }

  // at():
  _MappedType& at(size_type __n) { 
    if (__n >= this->size())
      throw std::out_of_range("map");
    return (*this)[__n]; 
  }
  
  // map 객체가 const-qualified인 경우
  const _MappedType& at(size_type __n) const { 
    if (__n >= this->size())
      throw std::out_of_range("map");
    return (*this)[__n]; 
  }

  // swap() :
  void swap(map<_KeyType, _MappedType, _KeyCompare, _Alloc>& __x) { _M_tree.swap(__x._M_tree); }

  // insert() :
  // 1. 단일요소
  pair<iterator, bool> insert(const value_type& __x)
    { return _M_tree.insert_unique(__x); }

  // 2. 힌트와 함께
  iterator insert(iterator position, const value_type& __x)
    { return _M_tree.insert_unique(position, __x); }

  // 3. 범위
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_tree.insert_unique(__first, __last);
  }

  // erase() :
  // 1. 단일 요소 (반복자로)
  void erase(iterator __position) 
    { _M_tree.erase(__position); }

  // 2. 단일 요소 (키로)
  // 지운 요소의 개수를 반환한다.
  size_type erase(const key_type& __x) 
    { return _M_tree.erase(__x); }
  
  // 3. 범위
  void erase(iterator __first, iterator __last)
    { _M_tree.erase(__first, __last); }
  
  void clear() { _M_tree.clear(); }

  // map operations:
  
  iterator find(const key_type& __x) { return _M_tree.find(__x); }
  const_iterator find(const key_type& __x) const { return _M_tree.find(__x); }

// count() :
// __x가 key인 요소 수를 계산한다.
// map은 모든 요소가 고유하므로 1과 0 중에서만 반환 -> 특정 키가 존재하는지 확인하는 데 사용할 수 있다.
  size_type count(const key_type& __x) const {
    return _M_tree.find(__x) == _M_tree.end() ? 0 : 1; // 찾았으면 1, 아니면 0
  }

  iterator lower_bound(const key_type& __x) {return _M_tree.lower_bound(__x); }
  const_iterator lower_bound(const key_type& __x) const {
    return _M_tree.lower_bound(__x); 
  }

  iterator upper_bound(const key_type& __x) {return _M_tree.upper_bound(__x); }
  const_iterator upper_bound(const key_type& __x) const {
    return _M_tree.upper_bound(__x); 
  }
  // * lower_bound()와 upper_bound()의 활용
  // [1245]에서 1부터 4까지의 요소를 포함하는 범위는 [lower_bound(1), upper_bound(4))로 표현할 수 있다.
  // (upper_bound(4)는 5를 가리키고 있으므로)
  
  pair<iterator,iterator> equal_range(const key_type& __x) {
    return _M_tree.equal_range(__x);
  }
  pair<const_iterator,const_iterator> equal_range(const key_type& __x) const {
    return _M_tree.equal_range(__x);
  }

  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator== (const map<_K1, _T1, _C1, _A1>&,
                          const map<_K1, _T1, _C1, _A1>&);
  template <class _K1, class _T1, class _C1, class _A1>
  friend bool operator< (const map<_K1, _T1, _C1, _A1>&,
                         const map<_K1, _T1, _C1, _A1>&);
};

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator==(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                       const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return __x._M_tree == __y._M_tree;
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator<(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                      const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return __x._M_tree < __y._M_tree;
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator!=(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                       const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator>(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                      const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator<=(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                       const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
bool operator>=(const map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                       const map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _Key, class _MappedType, class _KeyCompare, class _Alloc>
void swap(map<_Key,_MappedType,_KeyCompare,_Alloc>& __x, 
                 map<_Key,_MappedType,_KeyCompare,_Alloc>& __y) {
  __x.swap(__y);
}

} // namespace std

#endif /* MAP_HPP */

// Local Variables:
// mode:C++
// End:
