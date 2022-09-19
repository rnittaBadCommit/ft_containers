#ifndef UTIL_HPP
#define UTIL_HPP

namespace ft {
//// _KeyOfValue

template <typename _Pair>
struct _Select1st {
  typename _Pair::first_type& operator()(_Pair& __x) const { return __x.first; }

  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

template <typename _Tp>
struct _Identity {
  _Tp& operator()(_Tp& __x) const { return __x; }

  const _Tp& operator()(const _Tp& __x) const { return __x; }
};

// Partial specialization, avoids confusing errors in e.g. std::set<const T>.
template <typename _Tp>
struct _Identity<const _Tp> : _Identity<_Tp> {};

}   // namespace ft

#endif
