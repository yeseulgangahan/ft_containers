// This file contains all of the general iterator-related utility
// types, such as iterator_traits and struct iterator.

#include <cstddef>
  //std::ptrdiff_t: Result of pointer subtraction

namespace ft
{
  // iterator category

  struct input_iterator_tag {};
  struct output_iterator_tag {};
  struct forward_iterator_tag : public input_iterator_tag {};
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};

  // base of all iterator

  template<typename _Category, typename _Type, typename _Distance = std::ptrdiff_t, typename _Pointer = _Type*, typename _Reference = _Type&>
    struct iterator {
      typedef _Category  iterator_category;
      typedef _Type      value_type;
      typedef _Distance  difference_type;
      typedef _Pointer   pointer;
      typedef _Reference reference;
    };

  // iterator traits
  // Traits class defining properties of iterators.
  // Standard algorithms determine (1) certain *properties* of the iterators passed to them and (2) the *range* they represent
  // by using the members of the corresponding iterator_traits instantiation.

  // generic definition
  template<typename _Iterator>
    struct iterator_traits {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  // _Type* specialization
  template<typename _Type>
    struct iterator_traits<_Type*> {
      typedef random_access_iterator_tag    iterator_category;
      typedef _Type                         value_type;
      typedef std::ptrdiff_t                difference_type;
      typedef _Type*                        pointer;
      typedef _Type&                        reference;
    };

  // const _Type* specialization
  template<typename _Type>
    struct iterator_traits<const _Type*> {
      typedef random_access_iterator_tag    iterator_category;
      typedef _Type                         value_type;
      typedef std::ptrdiff_t                difference_type;
      typedef const _Type*                  pointer;
      typedef const _Type&                  reference;
    };
} // namespace ft
