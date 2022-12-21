#include <memory>
	// std::allocator
	// std::uninitialized_fill_n(): Constructs the objects in-place, instead of just copying the value to them.
	// std::uninitialized_copy(): Constructs the objects in-place, instead of just copying them.
  // std::destroy(): Destroys the objects in the range.

#include <iterator>
  // std::distance(): Returns the number of hops from first to last.

namespace ft
{

// The vector base class serves one purpose.
// its constructor and destructor allocate (but don't initialize) storage.
// This makes exception safety easier.

template <class _Type, class _Allocator>
class _Vector_base {
public:
  typedef typename _Alloc_traits<_Type, _Allocator>::allocator_type allocator_type; // ft::allocator<_Type> is default.

	// returns the associated allocator
	allocator_type get_allocator() const { return _M_data_allocator; }

protected:
  allocator_type _M_data_allocator; //instantiation
  _Type* _M_start;
  _Type* _M_finish; // Next to the last data
  _Type* _M_end_of_storage; // Next to the last storable space

  _Type* // Pointer to the first element of an array of n objects of Type
		_M_allocate(size_t __n) { return _M_data_allocator.allocate(__n); }
  void _M_deallocate(_Type* __p, size_t __n) { if (__p) _M_data_allocator.deallocate(__p, __n); }

	private:
	// constructor1
  _Vector_base(const allocator_type& __a)
		: _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}

	// constructor2
  _Vector_base(size_t __n, const allocator_type& __a)
		: _M_data_allocator(__a)/*, _M_start(_M_allocate(__n))*/, _M_finish(_M_start), _M_end_of_storage(_M_start + __n)
	{
    _M_start = _M_allocate(__n); //질문: 위에처럼 해도 될까?
	}

	// destructor
  ~_Vector_base() { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
};

template <
	class _Type, // The type of the elements.
	class _Allocator = std::allocator<_Type> // is used to acquire/release memory, construct/destroy the elements in that memory.
> class vector : protected _Vector_base<_Type, _Allocator>
{
private:
  typedef _Vector_base<_Type, _Allocator> _Base;
  typedef vector<_Type, _Allocator> vector_type;

public:
	// Member types

  typedef _Type value_type; // The first template parameter
  typedef typename _Allocator allocator_type; // The second template parameter

  typedef std::size_t size_type; // Unsigned integer type. this can represent any non-negative value of difference_type.
  typedef std::ptrdiff_t difference_type; // Signed integer type. identical to: iterator_traits<iterator>::difference_type.

	typedef value_type& reference;
  typedef const value_type& const_reference;

  typedef allocator_type::pointer pointer;
  typedef const allocator_type::const_pointer const_pointer;

	typedef _normal_iterator<pointer, vector_type> iterator; // a random access iterator to value_type. convertable to const_iterator.
  typedef _normal_iterator<const_pointer, vector_type> const_iterator; //a random access iterator to const value_type

  typedef reverse_iterator<iterator> reverse_iterator;
  typedef reverse_iterator<const_iterator> const_reverse_iterator;

protected:
  using _Base::_M_allocate;
  using _Base::_M_deallocate;
  using _Base::_M_start;
  using _Base::_M_finish;
  using _Base::_M_end_of_storage;

// public:
// 	// Member functions: Element access

// 	// access specified element with bounds checking
//   void _M_range_check(size_type __n) const {
//     if (__n >= this->size())
//       __throw_out_of_range("vector");
//   }
//   reference at(size_type __n) { _M_range_check(__n); return (*this)[__n]; }
//   const_reference at(size_type __n) const { _M_range_check(__n); return (*this)[__n]; }

// 	// access specified element
//   reference operator[](size_type __n) { return *(begin() + __n); }
//   const_reference operator[](size_type __n) const { return *(begin() + __n); }

// 	// access the first element
//   reference front() { return *begin(); }
//   const_reference front() const { return *begin(); }

// 	// access the last element
// 	reference back() { return *(end() - 1); }
//   const_reference back() const { return *(end() - 1); }

// 	// direct access to the underlying array
// 	//T* data() {}

// public:
// 	// Member functions: Iterators

// 	// returns an iterator to the beginning
//   iterator begin() { return iterator (_M_start); }
//   const_iterator begin() const { return const_iterator (_M_start); }

// 	// returns an iterator to the end
// 	iterator end() { return iterator (_M_finish); }
//   const_iterator end() const { return const_iterator (_M_finish); }

// 	// returns an reverse iterator to the beginning
//   reverse_iterator rbegin() { return reverse_iterator(end()); }
//   const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

// 	// returns an reverse iterator to the end
// 	reverse_iterator rend() { return reverse_iterator(begin()); }
//   const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

// public:
// 	// Member functions: Capacity

// 	// checks whether the container is empty
//   bool empty() const { return begin() == end(); }

// 	// returns the number of elements
//   size_type size() const { return size_type(end() - begin()); }

// 	// returns the maximum possible number of elements
//   size_type max_size() const { return size_type(-1) / sizeof(_Type); }

// 	// reserves storage
//   void reserve(size_type __n) {
//     if (capacity() < __n) {
//       const size_type __old_size = size();
//       pointer __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
//       std::destroy(_M_start, _M_finish);
//       _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//       _M_start = __tmp;
//       _M_finish = __tmp + __old_size;
//       _M_end_of_storage = _M_start + __n;
//     }
//   }

// 	// returns the number of elements that can be held in currently allocated storage
//   size_type capacity() const { return size_type(const_iterator(_M_end_of_storage) - begin()); }

// public:
// 	// Member functions: Modifiers

// 	// clears the contents
// 	void clear() { erase(begin(), end()); }

// 	// inserts elements
//   void insert (iterator __pos, size_type __n, const _Type& __x) //return void: maybe c++98
//     { _M_fill_insert(__pos, __n, __x); }

//   void _M_fill_insert (iterator __pos, size_type __n, const _Type& __x);

//   iterator insert(iterator __position, const _Type& __x) {
//     size_type __n = __position - begin();
//     if (_M_finish != _M_end_of_storage && __position == end()) {
//       _Construct(_M_finish, __x);
//       ++_M_finish;
//     }
//     else
//       _M_insert_aux(iterator(__position), __x);
//     return begin() + __n;
//   }

//   iterator insert(iterator __position) {
//     size_type __n = __position - begin();
//     if (_M_finish != _M_end_of_storage && __position == end()) {
//       _Construct(_M_finish);
//       ++_M_finish;
//     }
//     else
//       _M_insert_aux(iterator(__position));
//     return begin() + __n;
//   }

//   // Check whether it's an integral type.  If so, it's not an iterator.
//   template<class _InputIterator>
//     void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
//       typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
//       _M_insert_dispatch(__pos, __first, __last, _Integral());
//     }

//   template <class _Integer>
//     void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, __true_type)
//     { _M_fill_insert(__pos, static_cast<size_type>(__n), static_cast<_Type>(__val)); }

//   template<class _InputIterator>
//     void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, __false_type) {
// 	  typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
//       _M_range_insert(__pos, __first, __last, _IterCategory());
//     }

// 	// erases elements
//   iterator erase(iterator __position) {
//     if (__position + 1 != end())
//       copy(__position + 1, end(), __position);
//     --_M_finish;
//     std::destory(_M_finish);
//     return __position;
//   }
//   iterator erase(iterator __first, iterator __last) {
//     iterator __i(copy(__last, end(), __first));
//     std::destory(__i, end());
//     _M_finish = _M_finish - (__last - __first);
//     return __first;
//   }

// 	// adds an element to the end
//   void push_back(const _Type& __x) {
//     if (_M_finish != _M_end_of_storage) {
//       _Construct(_M_finish, __x);
//       ++_M_finish;
//     }
//     else
//       _M_insert_aux(end(), __x);
//   }

//   void push_back() {
//     if (_M_finish != _M_end_of_storage) {
//       _Construct(_M_finish);
//       ++_M_finish;
//     }
//     else
//       _M_insert_aux(end());
//   }

// 	// removes the last element
//   void pop_back() {
//     --_M_finish;
//     std::destory(_M_finish);
//   }

// 	// changes the number of elements stored
//   void resize(size_type __new_size, const _Type& __x) {
//     if (__new_size < size())
//       erase(begin() + __new_size, end());
//     else
//       insert(end(), __new_size - size(), __x);
//   }
//   void resize(size_type __new_size) { resize(__new_size, _Type()); }

// 	// swaps the contents
//   void
//   swap(vector<_Type, _Alloc>& __x)
//   {
//     std::swap(_M_start, __x._M_start);
//     std::swap(_M_finish, __x._M_finish);
//     std::swap(_M_end_of_storage, __x._M_end_of_storage);
//   }

public:
	// constructor
	// : Constructs a vector, initializing its contents depending on the constructor version used.

	// constructor1. empty container constructor (default constructor)
  explicit vector(
		const allocator_type& __a = allocator_type() // Default is to create a temporary object and copy it using the copy constructor.
	) : _Base(__a) {}

	// constructor2. fill constructor
	// : Constructs a container with n elements. Each element is a copy of __value.
  explicit vector(
			size_type __n,
			const _Type& __value,
			const allocator_type& __a = allocator_type())
    : _Base(__n, __a)
    { _M_finish = std::uninitialized_fill_n(_M_start, __n, __value); }

	// constructor3. range constructor
  template <class _InputIterator>
    vector(
      std::enable_if<!std::is_integral<_InputIterator>::value,// Check whether it's an integral type.  If so, it's not an iterator (and choose constructor2 by SFINE).
      _InputIterator>::type  __first,
      _InputIterator __last,
      const allocator_type& __a = allocator_type())
	: _Base(__a)
  {
	  typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
	  _M_range_initialize(__first, __last, _IterCategory());
  }

	// constructor4. copy constructor
  vector(const vector<_Type, _Alloc>& __x)
    : _Base(__x.size(), __x.get_allocator())
    { _M_finish = std::uninitialized_copy(__x.begin(), __x.end(), _M_start); }

	// destructor
  ~vector()
  { std::destory(_M_start, _M_finish); }

	// // assigns values to the container
  // vector<_Type, _Alloc>& operator=(const vector<_Type, _Alloc>& __x);

	// // assigns values to the container

  // // assign(), a generalized assignment member function.  Two
  // // versions: one that takes a count, and one that takes a range.
  // // The range version is a member template, so we dispatch on whether
  // // or not the type is an integer.

  // void assign(size_type __n, const _Type& __val) { _M_fill_assign(__n, __val); }
  // void _M_fill_assign(size_type __n, const _Type& __val);

  // template<class _InputIterator>
  //   void
	// assign(_InputIterator __first, _InputIterator __last)
	// {
  //     typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
  //     _M_assign_dispatch(__first, __last, _Integral());
  //   }

  // template<class _Integer>
  //   void
	// _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
  //   { _M_fill_assign((size_type) __n, (_Type) __val); }

  // template<class _InputIter>
  //   void
	// _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
  //   {
	//   typedef typename iterator_traits<_InputIter>::iterator_category _IterCategory;
	//   _M_assign_aux(__first, __last, _IterCategory());
	// }

  // template <class _InputIterator>
  // void _M_assign_aux(_InputIterator __first, _InputIterator __last,
  //                    input_iterator_tag);

  // template <class _ForwardIterator>
  // void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
  //                    forward_iterator_tag);

	// // returns the associated allocator
  // allocator_type get_allocator() const { return _Base::get_allocator(); }

//----------------------------------------------------------------------------------------

protected:
  // _M_functions

	// 1. this function only called by the constructor3.
  // 1-1.
  template <class _InputIterator>
  void _M_range_initialize(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
    for ( ; __first != __last; ++__first) // reallocations while growing.
      push_back(*__first);
  }

  // 1-2. The forward_iterator_tag struct and its derived structs
  template <class _ForwardIterator>
  void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag) {
    size_type __n = std::distance(__first, __last);
    _M_start = _M_allocate(__n);
    _M_end_of_storage = _M_start + __n;
    _M_finish = std::uninitialized_copy(__first, __last, _M_start);
  }

// protected:
//   void _M_insert_aux(iterator __position, const _Type& __x);
//   void _M_insert_aux(iterator __position);

// protected:

//   template <class _ForwardIterator>
//   pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first,
//                                                _ForwardIterator __last)
//   {
//     pointer __result = _M_allocate(__n);
//     try {
//       std::uninitialized_copy(__first, __last, __result);
//       return __result;
//     }
//     catch(...)
//       {
// 	_M_deallocate(__result, __n);
// 	__throw_exception_again;
//       }
//   }


//   template <class _InputIterator>
//   void _M_range_insert(iterator __pos,
//                        _InputIterator __first, _InputIterator __last,
//                        input_iterator_tag);

//   template <class _ForwardIterator>
//   void _M_range_insert(iterator __pos,
//                        _ForwardIterator __first, _ForwardIterator __last,
//                        forward_iterator_tag);
// };

// template <class _Type, class _Alloc>
// inline bool
// operator==(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y)
// {
//   return __x.size() == __y.size() &&
//          equal(__x.begin(), __x.end(), __y.begin());
// }

// template <class _Type, class _Alloc>
// inline bool
// operator<(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y)
// {
//   return lexicographical_compare(__x.begin(), __x.end(),
//                                  __y.begin(), __y.end());
// }

// template <class _Type, class _Alloc>
// inline void swap(vector<_Type, _Alloc>& __x, vector<_Type, _Alloc>& __y)
// {
//   __x.swap(__y);
// }

// template <class _Type, class _Alloc>
// inline bool
// operator!=(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y) {
//   return !(__x == __y);
// }

// template <class _Type, class _Alloc>
// inline bool
// operator>(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y) {
//   return __y < __x;
// }

// template <class _Type, class _Alloc>
// inline bool
// operator<=(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y) {
//   return !(__y < __x);
// }

// template <class _Type, class _Alloc>
// inline bool
// operator>=(const vector<_Type, _Alloc>& __x, const vector<_Type, _Alloc>& __y) {
//   return !(__x < __y);
// }

// template <class _Type, class _Alloc>
// vector<_Type,_Alloc>&
// vector<_Type,_Alloc>::operator=(const vector<_Type, _Alloc>& __x)
// {
//   if (&__x != this) {
//     const size_type __xlen = __x.size();
//     if (__xlen > capacity()) {
//       pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
//       std::destory(_M_start, _M_finish);
//       _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//       _M_start = __tmp;
//       _M_end_of_storage = _M_start + __xlen;
//     }
//     else if (size() >= __xlen) {
//       iterator __i(copy(__x.begin(), __x.end(), begin()));
//       std::destory(__i, end());
//     }
//     else {
//       copy(__x.begin(), __x.begin() + size(), _M_start);
//       std::uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
//     }
//     _M_finish = _M_start + __xlen;
//   }
//   return *this;
// }

// template <class _Type, class _Alloc>
// void vector<_Type, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val)
// {
//   if (__n > capacity()) {
//     vector<_Type, _Alloc> __tmp(__n, __val, get_allocator());
//     __tmp.swap(*this);
//   }
//   else if (__n > size()) {
//     fill(begin(), end(), __val);
//     _M_finish = std::uninitialized_fill_n(_M_finish, __n - size(), __val);
//   }
//   else
//     erase(fill_n(begin(), __n, __val), end());
// }

// template <class _Type, class _Alloc> template <class _InputIter>
// void vector<_Type, _Alloc>::_M_assign_aux(_InputIter __first, _InputIter __last,
//                                         input_iterator_tag) {
//   iterator __cur(begin());
//   for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
//     *__cur = *__first;
//   if (__first == __last)
//     erase(__cur, end());
//   else
//     insert(end(), __first, __last);
// }

// template <class _Type, class _Alloc> template <class _ForwardIter>
// void
// vector<_Type, _Alloc>::_M_assign_aux(_ForwardIter __first, _ForwardIter __last,
//                                    forward_iterator_tag) {
//   size_type __len = 0;
//   std::distance(__first, __last, __len);

//   if (__len > capacity()) {
//     pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
//     std::destory(_M_start, _M_finish);
//     _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//     _M_start = __tmp;
//     _M_end_of_storage = _M_finish = _M_start + __len;
//   }
//   else if (size() >= __len) {
//     iterator __new_finish(copy(__first, __last, _M_start));
//     std::destory(__new_finish, end());
//     _M_finish = __new_finish.base();
//   }
//   else {
//     _ForwardIter __mid = __first;
//     advance(__mid, size());
//     copy(__first, __mid, _M_start);
//     _M_finish = std::uninitialized_copy(__mid, __last, _M_finish);
//   }
// }

// template <class _Type, class _Alloc>
// void
// vector<_Type, _Alloc>::_M_insert_aux(iterator __position, const _Type& __x)
// {
//   if (_M_finish != _M_end_of_storage) {
//     _Construct(_M_finish, *(_M_finish - 1));
//     ++_M_finish;
//     _Type __x_copy = __x;
//     copy_backward(__position, iterator(_M_finish - 2), iterator(_M_finish- 1));
//     *__position = __x_copy;
//   }
//   else {
//     const size_type __old_size = size();
//     const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
//     iterator __new_start(_M_allocate(__len));
//     iterator __new_finish(__new_start);
//     try {
//       __new_finish = std::uninitialized_copy(iterator(_M_start), __position,
//                                         __new_start);
//       _Construct(__new_finish.base(), __x);
//       ++__new_finish;
//       __new_finish = std::uninitialized_copy(__position, iterator(_M_finish),
//                                         __new_finish);
//     }
//     catch(...)
//       {
// 	std::destory(__new_start,__new_finish);
// 	_M_deallocate(__new_start.base(),__len);
// 	__throw_exception_again;
//       }
//     std::destory(begin(), end());
//     _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//     _M_start = __new_start.base();
//     _M_finish = __new_finish.base();
//     _M_end_of_storage = __new_start.base() + __len;
//   }
// }

// template <class _Type, class _Alloc>
// void
// vector<_Type, _Alloc>::_M_insert_aux(iterator __position)
// {
//   if (_M_finish != _M_end_of_storage) {
//     _Construct(_M_finish, *(_M_finish - 1));
//     ++_M_finish;
//     copy_backward(__position, iterator(_M_finish - 2),
// 		  iterator(_M_finish - 1));
//     *__position = _Type();
//   }
//   else {
//     const size_type __old_size = size();
//     const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
//     pointer __new_start = _M_allocate(__len);
//     pointer __new_finish = __new_start;
//     try {
//       __new_finish = std::uninitialized_copy(iterator(_M_start), __position,
// 					__new_start);
//       _Construct(__new_finish);
//       ++__new_finish;
//       __new_finish = std::uninitialized_copy(__position, iterator(_M_finish),
// 					__new_finish);
//     }
//     catch(...)
//       {
// 	std::destory(__new_start,__new_finish);
// 	_M_deallocate(__new_start,__len);
// 	__throw_exception_again;
//       }
//     std::destory(begin(), end());
//     _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//     _M_start = __new_start;
//     _M_finish = __new_finish;
//     _M_end_of_storage = __new_start + __len;
//   }
// }

// template <class _Type, class _Alloc>
// void vector<_Type, _Alloc>::_M_fill_insert(iterator __position, size_type __n,
//                                          const _Type& __x)
// {
//   if (__n != 0) {
//     if (size_type(_M_end_of_storage - _M_finish) >= __n) {
//       _Type __x_copy = __x;
//       const size_type __elems_after = end() - __position;
//       iterator __old_finish(_M_finish);
//       if (__elems_after > __n) {
//         std::uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
//         _M_finish += __n;
//         copy_backward(__position, __old_finish - __n, __old_finish);
//         fill(__position, __position + __n, __x_copy);
//       }
//       else {
//         std::uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
//         _M_finish += __n - __elems_after;
//         std::uninitialized_copy(__position, __old_finish, _M_finish);
//         _M_finish += __elems_after;
//         fill(__position, __old_finish, __x_copy);
//       }
//     }
//     else {
//       const size_type __old_size = size();
//       const size_type __len = __old_size + max(__old_size, __n);
//       iterator __new_start(_M_allocate(__len));
//       iterator __new_finish(__new_start);
//       try {
//         __new_finish = std::uninitialized_copy(begin(), __position, __new_start);
//         __new_finish = std::uninitialized_fill_n(__new_finish, __n, __x);
//         __new_finish
//           = std::uninitialized_copy(__position, end(), __new_finish);
//       }
//       catch(...)
// 	{
// 	  std::destory(__new_start,__new_finish);
// 	  _M_deallocate(__new_start.base(),__len);
// 	  __throw_exception_again;
// 	}
//       std::destory(_M_start, _M_finish);
//       _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//       _M_start = __new_start.base();
//       _M_finish = __new_finish.base();
//       _M_end_of_storage = __new_start.base() + __len;
//     }
//   }
// }

// template <class _Type, class _Alloc> template <class _InputIterator>
// void
// vector<_Type, _Alloc>::_M_range_insert(iterator __pos,
//                                      _InputIterator __first,
//                                      _InputIterator __last,
//                                      input_iterator_tag)
// {
//   for ( ; __first != __last; ++__first) {
//     __pos = insert(__pos, *__first);
//     ++__pos;
//   }
// }

// template <class _Type, class _Alloc> template <class _ForwardIterator>
// void
// vector<_Type, _Alloc>::_M_range_insert(iterator __position,
//                                      _ForwardIterator __first,
//                                      _ForwardIterator __last,
//                                      forward_iterator_tag)
// {
//   if (__first != __last) {
//     size_type __n = 0;
//     std::distance(__first, __last, __n);
//     if (size_type(_M_end_of_storage - _M_finish) >= __n) {
//       const size_type __elems_after = end() - __position;
//       iterator __old_finish(_M_finish);
//       if (__elems_after > __n) {
//         std::uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
//         _M_finish += __n;
//         copy_backward(__position, __old_finish - __n, __old_finish);
//         copy(__first, __last, __position);
//       }
//       else {
//         _ForwardIterator __mid = __first;
//         advance(__mid, __elems_after);
//         std::uninitialized_copy(__mid, __last, _M_finish);
//         _M_finish += __n - __elems_after;
//         std::uninitialized_copy(__position, __old_finish, _M_finish);
//         _M_finish += __elems_after;
//         copy(__first, __mid, __position);
//       }
//     }
//     else {
//       const size_type __old_size = size();
//       const size_type __len = __old_size + max(__old_size, __n);
//       iterator __new_start(_M_allocate(__len));
//       iterator __new_finish(__new_start);
//       try {
//         __new_finish = std::uninitialized_copy(iterator(_M_start),
// 					  __position, __new_start);
//         __new_finish = std::uninitialized_copy(__first, __last, __new_finish);
//         __new_finish
//           = std::uninitialized_copy(__position, iterator(_M_finish), __new_finish);
//       }
//       catch(...)
// 	{
// 	  std::destory(__new_start,__new_finish);
// 	  _M_deallocate(__new_start.base(), __len);
// 	  __throw_exception_again;
// 	}
//       std::destory(_M_start, _M_finish);
//       _M_deallocate(_M_start, _M_end_of_storage - _M_start);
//       _M_start = __new_start.base();
//       _M_finish = __new_finish.base();
//       _M_end_of_storage = __new_start.base() + __len;
//     }
//   }
// }

} // namespace fd
