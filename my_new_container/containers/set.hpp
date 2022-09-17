#ifndef SET_HPP
#define SET_HPP

#include <limits>
#include "../utils/rb_tree.hpp"

namespace ft {
template <typename _Key, typename _Compare = std::less<_Key>,
          typename _Alloc = std::allocator<_Key> >
class set {
 public:
  typedef _Key key_type;
  typedef _Key value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare;
  typedef _Alloc allocator_type;

 private:
  typedef _Rb_tree<key_type, value_type, ft::_Identity<value_type>, key_compare,
                   allocator_type>
      _Rep_type;

  /// The actual tree structure.
  _Rep_type _M_t;

 public:
  // many of these are specified differently in ISO, but the following are
  // "functionally equivalent"
  typedef typename _Alloc::reference reference;
  typedef typename _Alloc::const_reference const_reference;
  typedef typename _Alloc::pointer pointer;
  typedef typename _Alloc::const_pointer const_pointer;
  typedef typename _Rep_type::size_type size_type;              // size_t
  typedef typename _Rep_type::difference_type difference_type;  // ptrdiff_t
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

  //// Member functions

  set() : _M_t() {}
  explicit set(const _Compare& comp, const allocator_type& a = allocator_type())
      : _M_t(comp, a) {}

  /*
  insert(begin,end)をすればいい
  */
  template <typename InputIt>
  set(InputIt first, InputIt last, const _Compare& comp = _Compare(),
      const allocator_type& a = allocator_type(),
      typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* =
          NULL)
      : _M_t(comp, a) {
    insert(first, last);
  }

  set(const set& x) : _M_t(x._M_t) {}
  virtual ~set() {}
  set& operator=(const set& x) {
    if (this == &x) return *this;
    _M_t = x._M_t;
    return *this;
  }
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  //// Iterators

  iterator begin() { return _M_t.begin(); }
  const_iterator begin() const { return _M_t.begin(); }
  iterator end() { return _M_t.end(); }
  const_iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() { return _M_t.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() { return _M_t.rend(); }
  const_reverse_iterator rend() const { return _M_t.rend(); }

  //// Capacity

  bool empty() const { return _M_t.empty(); }
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { 
//    return std::numeric_limits<size_type>::max() / sizeof(value_type);
    return _M_t.max_size();
    }

  //// Modifiers

  void clear() { _M_t.clear(); }  // header以外削除

  ft::pair<iterator, bool> insert(const value_type& x) {
    return _M_t.insert(x);
  }

  /*
  positionの1個次に挿入される場合、償却時間処理になるのが正しいが、
  BSTinsertの開始位置をroot固定の実装なため適応スキップ。
  */
  iterator insert(iterator position, const value_type& x) {
    (void)position;
    return insert(x).first;
  }

  template <class InputIt>
  void insert(InputIt first, InputIt last,
              typename ft::enable_if<!ft::is_integral<InputIt>::value,
                                     InputIt>::type* = NULL) {
    for (InputIt head = first; head != last; ++head) {
      insert(*head);
    }
  }

  void erase(iterator pos) { erase(*pos); }

  size_type erase(const key_type& key) { return _M_t.erase(key); }

  void erase(iterator first, iterator last) {
    if (first == begin() && last == end())
      clear();
    else
      while (first != last)
        erase(first++);  // 消す前に次iteratorをとっておかないと動いちゃう
  }

  void swap(set& x) { _M_t.swap(x._M_t); }

  //// Lookup

  size_type count(const key_type& x) const {
    if (_M_t.find(x) == _M_t.end())
      return 0;
    else
      return 1;
  }

  iterator find(const key_type& x) { return _M_t.find(x); }

  const_iterator find(const key_type& x) const { return _M_t.find(x); }

  iterator lower_bound(const key_type& x) { return _M_t.lower_bound(x); }

  const_iterator lower_bound(const key_type& x) const {
    return _M_t.lower_bound(x);
  }

  iterator upper_bound(const key_type& x) { return _M_t.upper_bound(x); }

  const_iterator upper_bound(const key_type& x) const {
    return _M_t.upper_bound(x);
  }

  ft::pair<iterator, iterator> equal_range(const key_type& x) {
    return _M_t.equal_range(x);
  }

  ft::pair<const_iterator, const_iterator> equal_range(
      const key_type& x) const {
    return _M_t.equal_range(x);
  }

  //// observers

  key_compare key_comp() const { return _M_t.key_comp(); };
  value_compare value_comp() const { return value_compare(_M_t.key_comp()); }

  //// Non-member functions
  template <typename _K1, typename _C1, typename _A1>
  friend bool operator==(const set<_K1, _C1, _A1>&, const set<_K1, _C1, _A1>&);

  template <typename _K1, typename _C1, typename _A1>
  friend bool operator<(const set<_K1, _C1, _A1>&, const set<_K1, _C1, _A1>&);
  //// debug

 public:
  void debug() { _M_t.debug(); }
  void debug2() { _M_t.debug2(); }
};

//// Non-member functions
template <typename _Key, typename Compare, typename _Alloc>
inline bool operator==(const ft::set<_Key, Compare, _Alloc>& x,
                       const ft::set<_Key, Compare, _Alloc>& y) {
  return x._M_t == y._M_t;
}

template <typename _Key, typename Compare, typename _Alloc>
inline bool operator<(const ft::set<_Key, Compare, _Alloc>& x,
                      const ft::set<_Key, Compare, _Alloc>& y) {
  return x._M_t < y._M_t;
}

/// Based on operator==
template <typename _Key, typename Compare, typename _Alloc>
inline bool operator!=(const ft::set<_Key, Compare, _Alloc>& x,
                       const ft::set<_Key, Compare, _Alloc>& y) {
  return !(x == y);
}

/// Based on operator<
template <typename _Key, typename Compare, typename _Alloc>
inline bool operator>(const ft::set<_Key, Compare, _Alloc>& x,
                      const ft::set<_Key, Compare, _Alloc>& y) {
  return y < x;
}

/// Based on operator<
template <typename _Key, typename Compare, typename _Alloc>
inline bool operator<=(const ft::set<_Key, Compare, _Alloc>& x,
                       const ft::set<_Key, Compare, _Alloc>& y) {
  return !(y < x);
}

/// Based on operator<
template <typename _Key, typename Compare, typename _Alloc>
inline bool operator>=(const ft::set<_Key, Compare, _Alloc>& x,
                       const ft::set<_Key, Compare, _Alloc>& y) {
  return !(x < y);
}

}  // namespace ft

/// See std::set::swap().
namespace std {
template <typename _Key, typename Compare, typename _Alloc>
void swap(ft::set<_Key, Compare, _Alloc>& x,
          ft::set<_Key, Compare, _Alloc>& y) {
  x.swap(y);
}

}  // namespace std
#endif
