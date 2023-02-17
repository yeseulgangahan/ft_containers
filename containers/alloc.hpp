#ifndef ALLOC_HPP
#define ALLOC_HPP

template <class _Type, class _AllocatorType>
struct _Alloc_traits
{
  typedef typename _AllocatorType::template rebind<_Type>::other allocator_type;
};

#endif // ALLOC_HPP