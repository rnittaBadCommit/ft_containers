#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <iostream>
#include <limits>

#include "./algorithm.hpp"
#include "./iterator.hpp"
#include "./pair.hpp"
#include "./util.hpp"

namespace ft {

enum _Rb_tree_color { _S_red = false, _S_black = true };

template <typename _Val>
struct _Rb_tree_node {
  typedef _Rb_tree_node<_Val>* _Link_type;

  _Rb_tree_color _M_color;
  _Link_type _M_parent;
  _Link_type _M_left;
  _Link_type _M_right;
  _Val* _M_value;

  // returns pointer to uncle
  /*
       B
      / \
     R   R uncle
    /
   B me
  */
  _Link_type uncle() {
    // If no parent or grandparent, then no uncle
    if (_M_parent == NULL or _M_parent->_M_parent == NULL) return NULL;

    if (_M_parent->isOnLeft())
      // uncle on right
      return _M_parent->_M_parent->_M_right;
    else
      // uncle on left
      return _M_parent->_M_parent->_M_left;
  }

  // check if node is left child of parent
  bool isOnLeft() { return this == _M_parent->_M_left; }

  // returns pointer to sibling
  /*
         B
        / \
    me R   R sibling
  */
  _Link_type sibling() {  // 兄弟
    // sibling null if no parent
    if (_M_parent == NULL) return NULL;

    if (isOnLeft()) return _M_parent->_M_right;

    return _M_parent->_M_left;
  }

  // moves node down and moves given node in its place
  /*
  before
       R parent
      /
     B me

  after
       R parent
      /
     B nParent
     |
    B me
    "nParent->(left or right) = me" decide in rotate() func.
  */
  void moveDown(_Link_type nParent) {
    if (_M_parent != NULL) {
      if (isOnLeft()) {
        _M_parent->_M_left = nParent;
      } else {
        _M_parent->_M_right = nParent;
      }
    }
    nParent->_M_parent = _M_parent;
    _M_parent = nParent;
  }

  bool hasRedChild() {
    return (_M_left != NULL and _M_left->_M_color == _S_red) or
           (_M_right != NULL and _M_right->_M_color == _S_red);
  }
  /*
               B10
              /   \
            R7     R22
           / \      / \
         B6  B8   B13  B26
         /
        R2

    ---B10.swap(B13)---
               B13
              /   \
            R7     B22
           / \        \
         B6  B8        R26
         /
        R2
  */
  /*
  swap except _Val = pair(const key, val)
  const cannot swap or copy

  header->parent cannot apply
  */
  void swapNode(_Link_type& x) {
    // xのまわり
    _Link_type xp = x->_M_parent;
    _Link_type xr = x->_M_right;
    _Link_type xl = x->_M_left;
    _Link_type tp = _M_parent;
    _Link_type tr = _M_right;
    _Link_type tl = _M_left;

    if (xp->_M_left == x)
      xp->_M_left = this;
    else if (xp->_M_right == x)
      xp->_M_right = this;
    if (xl) xl->_M_parent = this;
    if (xr) xr->_M_parent = this;

    // 自分のまわり
    if (isOnLeft())
      tp->_M_left = x;
    else
      tp->_M_right = x;
    if (tl) tl->_M_parent = x;
    if (tr) tr->_M_parent = x;

    std::swap(_M_color, x->_M_color);
    std::swap(_M_parent, x->_M_parent);
    std::swap(_M_right, x->_M_right);
    std::swap(_M_left, x->_M_left);
  }
};

template <typename _Val>
static _Rb_tree_node<_Val>* local_Rb_tree_increment(
    _Rb_tree_node<_Val>* x) throw() {
  /*
           DUMMY(R): 0x7ffe5ab13d08
             +       |     right: 7
             |       +     left: 1
     [ROOT] 2(B): 0x1892500
              right: 4
              left: 1
             /       \
   1(B): 0x18924d0     4(R): 0x1892560
     right: 0            right: 6
     left: 0             left: 3
                         /      \
             3(B): 0x1892530     6(B): 0x18925c0
              right: 0             right: 7
              left: 0              left: 5
                                  /       \
                        5(R): 0x1892590    7(R): 0x18925f0
                          right: 0            right: 0
                          left: 0             left: 0
  1. 右がある(4->5)
    右に進んだ後、突き当りまで左
  2. 右がない(5->6)
    親から見て、自分が右なら戻り続ける。
  3. 最大だった場合(7->dummy)
    Dummyまで戻る。dummyの右、4の上で終了
  */

  // size==0
  if (x->_M_right == x) return x;
  if (x->_M_right != 0) {
    x = x->_M_right;
    while (x->_M_left != 0) x = x->_M_left;
  } else {
    _Rb_tree_node<_Val>* y = x->_M_parent;
    while (x == y->_M_right) {
      x = y;
      y = y->_M_parent;
    }
    if (x->_M_right != y) {
      /*
      x->_M_right == y <=> M.size()==1 && x==dummy, return(dummy)

        dummy(R): 0x7ffde6b05688
       | + right: ROOT
       + | left: ROOT
        ROOT(B): 0x9414d0
          right: 0
          left: 0
      */
      x = y;
    }
  }
  return x;
}

template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_increment(_Rb_tree_node<_Val>* x) throw() {
  return local_Rb_tree_increment(x);
}

template <typename _Val>
const _Rb_tree_node<_Val>* _Rb_tree_increment(
    const _Rb_tree_node<_Val>* x) throw() {
  return local_Rb_tree_increment(const_cast<_Rb_tree_node<_Val>*>(x));
}

template <typename _Val>
static _Rb_tree_node<_Val>* local_Rb_tree_decrement(
    _Rb_tree_node<_Val>* x) throw() {
  /*
           DUMMY(R): 0x7ffe5ab13d08
             +       |     right: 7
             |       +     left: 1
     [ROOT] 2(B): 0x1892500
              right: 4
              left: 1
             /       \
   1(B): 0x18924d0     4(R): 0x1892560
     right: 0            right: 6
     left: 0             left: 3
                         /      \
             3(B): 0x1892530     6(B): 0x18925c0
              right: 0             right: 7
              left: 0              left: 5
                                  /       \
                        5(R): 0x1892590    7(R): 0x18925f0
                          right: 0            right: 0
                          left: 0             left: 0
  1. 左がある(4->3)
    左に進んだ後、突き当りまで右
  2. 左がない(5->4)
    親から見て、自分が左なら戻り続ける。
  3. 最後だった場合(dummy->7)
    Dummy->right
  */

  // size==0
  if (x->_M_right == x) return x;
  // x == dummy == M.end()
  if (x->_M_color == _S_red && x->_M_parent->_M_parent == x)
    x = x->_M_right;
  else if (x->_M_left != 0) {
    _Rb_tree_node<_Val>* y = x->_M_left;
    while (y->_M_right != 0) {
      y = y->_M_right;
    }
    x = y;
  } else {
    _Rb_tree_node<_Val>* y = x->_M_parent;
    while (x == y->_M_left) {
      x = y;
      y = y->_M_parent;
    }
    x = y;
  }
  return x;
}

template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_decrement(_Rb_tree_node<_Val>* x) throw() {
  return local_Rb_tree_decrement(x);
}

template <typename _Val>
const _Rb_tree_node<_Val>* _Rb_tree_decrement(const _Rb_tree_node<_Val>* x) throw() {
  return local_Rb_tree_decrement(const_cast<_Rb_tree_node<_Val>*>(x));
}

template <typename _Tp>
class _Rb_tree_iterator {
	public:
  typedef _Tp value_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  typedef ft::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_iterator<_Tp> _Self;
  typedef typename _Rb_tree_node<_Tp>::_Link_type _Link_type;

  _Rb_tree_iterator() : _M_node() {}
  explicit _Rb_tree_iterator(_Link_type x) : _M_node(x) {}

  reference operator*() const { return *_M_node->_M_value; }
  pointer operator->() const { return _M_node->_M_value; }

  _Self& operator++() {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self operator++(int) {
    _Self tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self operator--(int) {
    _Self tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return tmp;
  }

  bool operator==(const _Self& x) const { return _M_node == x._M_node; }
  bool operator!=(const _Self& x) const { return _M_node != x._M_node; }

  // debug
 public:
  _Rb_tree_color get_color() { return _M_node->_M_color; }
  _Link_type get_link() { return _M_node; }

  // member value /////
private:
  _Link_type _M_node;
};

template <typename _Tp>
struct _Rb_tree_const_iterator {
  typedef _Tp value_type;
  typedef const _Tp& reference;
  typedef const _Tp* pointer;

  typedef _Rb_tree_iterator<_Tp> iterator;

  typedef ft::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_const_iterator<_Tp> _Self;
  typedef const _Rb_tree_node<_Tp>* _Link_type;

  _Rb_tree_const_iterator() : _M_node() {}
  explicit _Rb_tree_const_iterator(_Link_type x) : _M_node(x) {}
  _Rb_tree_const_iterator(const iterator& it) : _M_node(it._M_node) {}
  iterator _M_const_cast() const {
    return iterator(const_cast<typename iterator::_Link_type>(_M_node));
  }
  reference operator*() const { return *_M_node->_M_value; }
  pointer operator->() const { return _M_node->_M_value; }

  _Self& operator++() {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self operator++(int) {
    _Self tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self operator--(int) {
    _Self tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return tmp;
  }

  bool operator==(const _Self& x) const { return _M_node == x._M_node; }
  bool operator!=(const _Self& x) const { return _M_node != x._M_node; }

  // member value ////
  private:
  _Link_type _M_node;
};

// template <typename _Val>
// inline bool operator==(const _Rb_tree_iterator<_Val>& __x,
//                        const _Rb_tree_const_iterator<_Val>& __y) {
//   return __x._M_node == __y._M_node;
// }

// template <typename _Val>
// inline bool operator!=(const _Rb_tree_iterator<_Val>& __x,
//                        const _Rb_tree_const_iterator<_Val>& __y) {
//   return __x._M_node != __y._M_node;
// }

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc = std::allocator<_Val> >
class rb_tree {
  // _Allocの引数<_Val> を <Node<_Val> > に 置き換える
  typedef typename _Alloc::template rebind<_Rb_tree_node<_Val> >::other
      _Node_allocator;

 protected:
  typedef _Rb_tree_node<_Val>* _Link_type;
  typedef const _Rb_tree_node<_Val>* _Const_Link_type;

 public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Alloc allocator_type;
  typedef _Compare key_compare;

  //// member value ////
 private:
  _Link_type _M_header;
  size_type _M_node_count;
  key_compare _M_key_compare;
  _Node_allocator _M_node_alloc;

 public:
  typedef _Rb_tree_iterator<value_type> iterator;
  typedef _Rb_tree_const_iterator<value_type> const_iterator;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

 protected:
  key_type _S_key(_Link_type x) const {
    return _KeyOfValue()(*(x->_M_value));
  }
  key_type _S_key(_Const_Link_type x) const {
    return _KeyOfValue()(*(x->_M_value));
  }
  key_type _S_key(value_type x) const { return _KeyOfValue()(x); }
  key_type _S_key(iterator x) const { return _KeyOfValue()(*x); }
  key_type _S_key(const_iterator x) const { return _KeyOfValue()(*x); }
  _Link_type _M_end() { return _M_header; }
  _Const_Link_type _M_end() const { return _M_header; }
  _Link_type _S_root() { return _M_header->_M_parent; }
  _Const_Link_type _S_root() const { return _M_header->_M_parent; }
  _Link_type& _M_root() { return _M_header->_M_parent; }
  _Const_Link_type _M_root() const { return _M_header->_M_parent; }
  _Link_type _S_mostleft() {
    _Link_type ret = _S_root();
    if (ret == NULL) return _M_header;
    while (ret->_M_left) ret = ret->_M_left;
    return ret;
  }
  _Const_Link_type _S_mostleft() const {
    return (_Const_Link_type)_S_mostleft();
  }
  _Link_type _S_mostright() {
    _Link_type ret = _S_root();
    if (ret == NULL) return _M_header;
    while (ret->_M_right) ret = ret->_M_right;
    return ret;
  }
  _Const_Link_type _S_mostright() const {
    return (_Const_Link_type)_S_mostright();
  }

  void _M_reset() {
    _M_header->_M_color = _S_red;
    _M_header->_M_parent = 0;
    _M_header->_M_left = _M_header;
    _M_header->_M_right = _M_header;
    _M_node_count = 0;
  }

  void _M_move_data(rb_tree& __from) {  // from が削除される
    _M_header->_M_color = __from._M_header->_M_color;
    _M_root() = __from._M_header->_M_parent;
    _M_header->_M_left = __from._M_header->_M_left;
    _M_header->_M_right = __from._M_header->_M_right;
    _M_root()->_M_parent = _M_header;
    _M_node_count = __from._M_node_count;
    __from._M_reset();
  }

  //// iterator ////////////////

 public:
  iterator begin() { return iterator(_M_header->_M_left); }
  const_iterator begin() const { return const_iterator(_M_header->_M_left); }
  iterator end() { return iterator(_M_header); }
  const_iterator end() const { return const_iterator(_M_header); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  key_compare key_comp() const { return _M_key_compare; }
  allocator_type get_allocator() const { return allocator_type(_M_node_alloc); }
  bool empty() const { return _M_node_count == 0; }
  size_type size() const { return _M_node_count; }
  size_type max_size() const {
    size_t div = sizeof(_Link_type) * 4 + sizeof(value_type);
    div = (div / 8) * 8;
    return std::numeric_limits<size_type>::max() / div;
    //    return _M_node_alloc.max_size();
  }

 public:
  rb_tree() : _M_key_compare(key_compare()), _M_node_alloc(_Node_allocator()) {
    _M_header = _M_create_node(value_type());  // size()==1 になっちゃう
    _M_reset();
  }
  rb_tree(const key_compare& key_comp, const _Node_allocator& node_alloc)
      : _M_key_compare(key_comp), _M_node_alloc(node_alloc) {
    _M_header = _M_create_node(value_type());  // size()==1 になっちゃう
    _M_reset();
  }
  rb_tree(rb_tree const& src) {
    // headerもコピー
    _M_header = _M_create_node(value_type());  // size()==1 になっちゃう
    _M_reset();
    _M_key_compare = src._M_key_compare;
    _M_node_alloc = src._M_node_alloc;
    if (src._M_root() != 0) {
      _M_root() = _M_copy(src._M_header->_M_parent, _M_header);
      _M_header->_M_left = _S_mostleft();
      _M_header->_M_right = _S_mostright();
    }
  }
  ~rb_tree() {
    _M_erase(_S_root());
    ++_M_node_count;
    _M_put_node(_M_header);
  }
  rb_tree& operator=(const rb_tree& src) {
    if (this == &src) return *this;
    _M_erase(_S_root());

    // headerは上書き再利用
    _M_reset();
    _M_key_compare = src._M_key_compare;
    _M_node_alloc = src._M_node_alloc;
    // root以下クローン
    if (src._M_node_count) {
      _M_root() = _M_copy(src._M_header->_M_parent, _M_header);
      _M_header->_M_left = _S_mostleft();
      _M_header->_M_right = _S_mostright();
    }
    return *this;
  }

  //// Modifiers

  void clear() {
    _M_erase(_S_root());
    _M_reset();
  }

  // insertのhelper4関数

  /*
             DUMMY(R): 0x7ffe5ab13d08
               +       |     right: 7
               |       +     left: 1
       [ROOT] 2(B): 0x1892500
                right: 4
                left: 1
               /       \
     1(B): 0x18924d0     4(R): 0x1892560
       right: 0            right: 6
       left: 0             left: 3
                           /      \
               3(B): 0x1892530     6(B): 0x18925c0
                right: 0             right: 7
                left: 0              left: 5
                                    /       \
                          5(R): 0x1892590    7(R): 0x18925f0
                            right: 0            right: 0
                            left: 0             left: 0


  4.5を入れるとしたら？
  root=2
  4.5>2

  root=4
  4.5>4

  root=6
  4.5<6

  root=5
  4.5<5

  root=NULL

  (root=5) // かわるのはここだけ。
  5->left = 4.5 (NULL->4.5)
  4.5->parent = 5

  (root=6)
  6->left = 5 (5->5)
  5->parent = 6 (6->6)
  ...
  */
 protected:
  _Link_type BSTInsert(_Link_type root, _Link_type pt) {
    /* If the tree is empty, return a new _Link_type */
    if (root == NULL) return pt;

    /* Otherwise, recur down the tree */
    if (_M_key_compare(_S_key(pt), _S_key(root))) {
      root->_M_left = BSTInsert(root->_M_left, pt);
      root->_M_left->_M_parent = root;
    } else if (_M_key_compare(_S_key(root), _S_key(pt))) {
      root->_M_right = BSTInsert(root->_M_right, pt);
      root->_M_right->_M_parent = root;
    }

    /* return the (unchanged) node pointer */
    return root;
  }

  /*
  before                                        after
  　　　　　　root                                    root
              |                                       |
             50(B) x:grand_parent                  70(B):nParent
            /   \                                   /   \
    uncle:20(B)  70(R):nParent                    x:50(R)  88(R)
                /  \                             /   \
            60(B)  88(R)                     20(B)  60(B)

  祖父母の右を親の左に、
  親を祖父母に、
  祖父母を兄弟に回転し、
  色を塗りなおす。
  親が黒になるので、上は気にしないでよくなる。
  */
  // left rotates the given node
  void leftRotate(_Link_type x) {
    // new parent will be node's right child
    _Link_type nParent = x->_M_right;  // 70

    // update root if current node is root
    if (x == _S_root()) _M_root() = nParent;

    x->moveDown(nParent);  // root->left = 70 and 70->parent = root

    // connect x with new parent's left element
    x->_M_right = nParent->_M_left;  // 50->right = 60
    // connect new parent's left element with node
    // if it is not null
    if (nParent->_M_left != NULL)
      nParent->_M_left->_M_parent = x;  // 50->parent = 70

    // connect new parent with x
    nParent->_M_left = x;  // 70->left = 50
  }

  /*
  before                                        after
  　　　　　　root                                    root
              |                                       |
             50(B) x:grand_parent                  20(B):nParent
            /   \                                   /  \
  nParent:20(R)  70(B):uncle                     16(R)  50(R):x
          /  \                                         /   \
       16(R)  24(B)                                  24(B)  70(B)

  祖父母の左を親の右に、
  親を祖父母に、
  祖父母を兄弟に回転し、
  色を塗りなおす。
  親が黒になるので、上は気にしないでよくなる。
  */
  void rightRotate(_Link_type x) {
    // new parent will be node's left child
    _Link_type nParent = x->_M_left;  // 20

    // update root if current node is root
    if (x == _S_root()) _M_root() = nParent;

    x->moveDown(nParent);  // root->right = 20 and 20->parent = root

    // connect x with new parent's right element
    x->_M_left = nParent->_M_right;  // 50->left = 24
    // connect new parent's right element with node
    // if it is not null
    if (nParent->_M_right != NULL) nParent->_M_right->_M_parent = x;

    // connect new parent with x
    nParent->_M_right = x;  // 20->right = 50
  }

  void fixRedRed(_Link_type& pt) {
    _Link_type parent_pt = NULL;
    _Link_type grand_parent_pt = NULL;

    while ((pt != _S_root()) && (pt->_M_color != _S_black) &&
           (pt->_M_parent->_M_color ==
            _S_red))  // 根っこじゃない、かつ赤赤が続いてる
    {
      parent_pt = pt->_M_parent;
      grand_parent_pt = pt->_M_parent->_M_parent;

      /*
         Case : A
              Parent of pt is left child of Grand-parent of pt.
              ptの親はptの祖父母の左の子になります
                B grand_parent
               /
              R parent
              |
              R
      */
      if (parent_pt == grand_parent_pt->_M_left) {
        _Link_type uncle_pt = grand_parent_pt->_M_right;

        /* Case : 1
        The uncle of pt is also red
        Only Recoloring required

        叔父も赤なら、色を変えるだけ
                 B grand_parent
                / \
        Parent R   R uncle
               |
               R
        */
        if (uncle_pt != NULL && uncle_pt->_M_color == _S_red) {
          grand_parent_pt->_M_color = _S_red;
          parent_pt->_M_color = _S_black;
          uncle_pt->_M_color = _S_black;
          pt = grand_parent_pt;
        } else {
          /* Case : 2
          pt is right child of its parent
          Left-rotation require
          いったんptを左につけかえる
                   B grand_parent
                  / \
          Parent R   B uncle
                  \
                   R
          */
          if (pt == parent_pt->_M_right) {
            leftRotate(parent_pt);
            pt = parent_pt;
            parent_pt = pt->_M_parent;
          }

          /* Case : 3
          pt is left child of its parent
          Right-rotation required
                   B grand_parent
                  / \
          Parent R   B uncle
                /
               R

          before 　　　　　
                    root
                     |
                    50(B) grandparent
                    /   \
          Parent 20(R)   70(B) uncle
                  /
               16(R)

          after　　　　
                    root
                     |
                    20(B)
                    /   \
                 16(R)  50(R)
                           \
                            70(B)
          親を祖父母に、
          祖父母を兄弟に回転し、
          色を塗りなおす。 swap(20(B) 50(R))
          親が黒になるので、上は気にしないでよくなる。
          */
          rightRotate(grand_parent_pt);
          std::swap(parent_pt->_M_color, grand_parent_pt->_M_color);
          pt = parent_pt;
        }
      }

      /* Case : B
      Parent of pt is right child
      of Grand-parent of pt */
      else {
        _Link_type uncle_pt = grand_parent_pt->_M_left;  // null

        /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
        if ((uncle_pt != NULL) && (uncle_pt->_M_color == _S_red)) {
          grand_parent_pt->_M_color = _S_red;
          parent_pt->_M_color = _S_black;
          uncle_pt->_M_color = _S_black;
          pt = grand_parent_pt;
        } else {
          /* Case : 2
          pt is left child of its parent
          Right-rotation required
          */
          if (pt == parent_pt->_M_left) {
            rightRotate(parent_pt);
            pt = parent_pt;
            parent_pt = pt->_M_parent;
          }

          /* Case : 3
          pt is right child of its parent
          Left-rotation required
              head
              root
                \
                 1(B)
                  \
                   2(R)
                    \
                     3(R)
          */
          leftRotate(grand_parent_pt);
          std::swap(parent_pt->_M_color, grand_parent_pt->_M_color);
          pt = parent_pt;
        }
      }
    }

    _M_root()->_M_color = _S_black;
  }

  // delete

  // find node that do not have a left child
  // in the subtree of the given node
  // 今いる場所から、左の突き当りノード
  _Link_type successor(_Link_type x) {
    _Link_type temp = x;

    while (temp->_M_left != NULL) temp = temp->_M_left;

    return temp;
  }

  // find node that replaces a deleted node in BST
  _Link_type BSTreplace(_Link_type x) {
    // when node have 2 children
    // 子供が2人いるなら、RLLLLL
    if (x->_M_left != NULL and x->_M_right != NULL)
      return successor(x->_M_right);

    // when leaf
    if (x->_M_left == NULL and x->_M_right == NULL) return NULL;

    // when single child
    // 子供が1人なら左から返す。
    if (x->_M_left != NULL)
      return x->_M_left;
    else
      return x->_M_right;
  }

  // deletes the given node
  void deleteNode(_Link_type v) {
    _Link_type u = BSTreplace(v);

    // True when u and v are both black
    bool uvBlack =
        ((u == NULL or u->_M_color == _S_black) and (v->_M_color == _S_black));
    _Link_type parent = v->_M_parent;

    if (u == NULL) {  // vが葉
      // u is NULL therefore v is leaf
      if (v == _S_root()) {
        // v is root, making root null
        _M_root() = _M_header;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          /*
               B
              /
            v:B
          */
          fixDoubleBlack(v);
        } else {
          // v is red
          /*
                B
               / \
            v:R  (R)
          */
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->_M_color = _S_red;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          parent->_M_left = NULL;
        } else {
          parent->_M_right = NULL;
        }
      }
      _M_put_node(v);
      return;
    }

    if (v->_M_left == NULL or v->_M_right == NULL) {
      // v has 1 child
      if (v == _S_root()) {
        // v is root, assign the value of u to v, and delete u
        /*
              v
             /
            u

             u
            / \
           NU NU
        */
        _M_root() = u;
        v->swapNode(u);
        u->_M_left = u->_M_right = NULL;
        _M_put_node(v);
      } else {
        // Detach v from tree and move u up
        /*
                 B
                /
             v:R
              /
          u:(B)
        */
        if (v->isOnLeft()) {
          parent->_M_left = u;
        } else {
          parent->_M_right = u;
        }
        _M_put_node(v);
        u->_M_parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->_M_color = _S_black;
        }
      }
      return;
    }

    // v has 2 children, swap values with successor and recurse
    /*
     before del(B10)
             B10:v
            /   \
          R7      R22
         / \      /  \
       B6  B8   B13:u B26
       /
      R2

      after
             B13
            /   \
          R7     B22
         / \        \
       B6  B8        R26
       /
      R2
      */

    if (v == _S_root()) _M_root() = u;
    v->swapNode(u);
    deleteNode(v);
  }

  void fixDoubleBlack(_Link_type x) {
    if (x == _S_root())
      // Reached root
      return;

    _Link_type sibling = x->sibling();
    _Link_type parent = x->_M_parent;
    if (sibling == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->_M_color == _S_red) {
        // Sibling red
        /*
        before
              B
             / \
          x:B   R

        after
             (R)
             / \
          x:B   (B)
        */
        parent->_M_color = _S_red;
        sibling->_M_color = _S_black;
        if (sibling->isOnLeft()) {
          // left case
          rightRotate(parent);
        } else {
          // right case
          /*
               (B)
               /
             (R)
             /
           x:B
          */
          leftRotate(parent);  // 右が持ち上がる
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // 赤い子供がいるならおしまい
          // vを削除しても、どの葉からも黒ノードが同じ数になる。
          // at least 1 red children
          if (sibling->_M_left != NULL and
              sibling->_M_left->_M_color == _S_red) {  // 左が赤
            if (sibling->isOnLeft()) {
              // left left
              sibling->_M_left->_M_color = sibling->_M_color;
              sibling->_M_color = parent->_M_color;
              rightRotate(parent);
            } else {
              // right left
              sibling->_M_left->_M_color = parent->_M_color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {  // 右が赤
            if (sibling->isOnLeft()) {
              // left right
              /*
              before
                  R
                 / \
              x:B   B
               /   /
              R   R
              xをdelすると、x下の葉がB0, それ以外がB1になってしまう

              changeColor
                  R
                 / \
              x:B  (R)
                   /
                 (B)

              leftRotate
                    (R)
                    /  \
                   R    B
                 /
               x:B
              changeColor
                     (R)
                     /  \
            parent:(B)    B
                  /
                x:B
              // xを除いた場合どの葉もB1になった。
              */
              sibling->_M_right->_M_color = parent->_M_color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              // right right
              sibling->_M_right->_M_color = sibling->_M_color;
              sibling->_M_color = parent->_M_color;
              leftRotate(parent);
            }
          }
          parent->_M_color = _S_black;
        } else {
          // 2 black children
          /*
                R
              /  \
            x:B   B
                 / \
                B   B
            xがなくなるとx側がB1に。右側がB2になってしまう

              (B)
              /  \
            x:B  (R)
                 / \
                B   B
          */
          sibling->_M_color = _S_red;
          if (parent->_M_color ==
              _S_black)  // parent以下全体の黒が1個減ってしまうので、さらにparentで調整
            fixDoubleBlack(parent);
          else
            parent->_M_color = _S_black;
        }
      }
    }
  }

 public:
  ft::pair<iterator, bool> insert(const value_type& x) {
    ft::pair<iterator, bool> ret;
    // keyが重複してないこと
    iterator it = lower_bound(_S_key(x));
    if (it != end() && !_M_key_compare(_S_key(x), _S_key(it))) {
      ret.first = it;
      ret.second = false;
      return ret;
    }
    // node作る
    _Link_type pt = _M_create_node(x);
    _Link_type old = pt;
    // insert
    _M_root() = BSTInsert(_S_root(), pt);

    // rebalanceする ptは変化する
    fixRedRed(pt);

    // rootの親がheaderに
    _M_root()->_M_parent = _M_header;

    // headerの更新
    _M_header->_M_left = _S_mostleft();
    _M_header->_M_right = _S_mostright();

    ret.first = (iterator)old;
    ret.second = true;
    return ret;
  }

  // utility function that deletes the node with given value
  size_type erase(const key_type& key) {
    if (_S_root() == NULL)
      // Tree is empty
      return 0;

    iterator v = lower_bound(key);  // {1,3,5} key=4 v=5

    if (v == end() || _M_key_compare(key, _S_key(v))) return 0;

    deleteNode(v.get_link());

    // headerの更新
    if (size()) {
      _M_root()->_M_parent = _M_header;
      _M_header->_M_left = _S_mostleft();
      _M_header->_M_right = _S_mostright();
    } else {
      _M_root() = NULL;
      _M_header->_M_left = _M_header;
      _M_header->_M_right = _M_header;
    }

    return 1;
  }

  void swap(rb_tree& __t) {
    // どちらかが size==0 の場合、移す or reset()する
    if (_M_root() == 0) {
      if (__t._M_root() != 0) _M_move_data(__t);
    } else if (__t._M_root() == 0)
      __t._M_move_data(*this);
    else {  // どっちも要素がある場合
      std::swap(_M_root(), __t._M_root());
      std::swap(_M_header->_M_left, __t._M_header->_M_left);
      std::swap(_M_header->_M_right, __t._M_header->_M_right);

      _M_root()->_M_parent = _M_end();
      __t._M_root()->_M_parent = __t._M_end();
      std::swap(_M_node_count, __t._M_node_count);
    }
    // No need to swap header's color as it does not change.
    std::swap(_M_key_compare, __t._M_key_compare);
    std::swap(_M_node_alloc, __t._M_node_alloc);
  }

  // helper ////
 protected:
  _Link_type _M_get_node() {
    ++_M_node_count;
    return _M_node_alloc.allocate(1);
  }
  void _M_put_node(_Link_type p) {
    --_M_node_count;
    _Alloc value_alloc;

    value_alloc.destroy(p->_M_value);
    value_alloc.deallocate(p->_M_value, 1);
    _M_node_alloc.destroy(p);
    _M_node_alloc.deallocate(p, 1);
  }
  void _M_construct_node(_Link_type node, const value_type& x) {
    try {
      node->_M_color = _S_red;
      node->_M_left = NULL;
      node->_M_right = NULL;
      node->_M_parent = NULL;
      _Alloc value_alloc;
      typename _Alloc::pointer p = value_alloc.allocate(1);
      value_alloc.construct(p, x);
      node->_M_value = p;
    } catch (...) {
      _M_put_node(node);
      throw;  // throw_exception_again;
    }
  }

  _Link_type _M_create_node(const value_type& x) {
    _Link_type tmp = _M_get_node();
    _M_construct_node(tmp, x);
    return tmp;
  }

  // lower_bound
  /*
           DUMMY(R): 0x7ffe5ab13d08
             +       |     right: 7
             |       +     left: 1
     [ROOT] 2(B): 0x1892500
              right: 4
              left: 1
             /       \
   1(B): 0x18924d0     4(R): 0x1892560
     right: 0            right: 6
     left: 0             left: 3
                         /      \
             3(B): 0x1892530     6(B): 0x18925c0
              right: 0             right: 7
              left: 0              left: 5
                                  /       \
                        5(R): 0x1892590    7(R): 0x18925f0
                          right: 0            right: 0
                          left: 0             left: 0

  xが探索。yが1手前の状態。
  xの探索がnillに至ったら、return(y)
  */

  iterator _M_lower_bound(_Link_type x, _Link_type y, const _Key& k) {
    while (x != 0)
      if (!_M_key_compare(_S_key(x), k))
        y = x, x = x->_M_left;
      else
        x = x->_M_right;
    return iterator(y);
  }

  const_iterator _M_lower_bound(_Const_Link_type x, _Const_Link_type y,
                                const _Key& k) const {
    while (x != 0)
      if (!_M_key_compare(_S_key(x), k))
        y = x, x = x->_M_left;
      else
        x = x->_M_right;
    return const_iterator(y);
  }

  iterator _M_upper_bound(_Link_type x, _Link_type y, const _Key& k) {
    while (x != 0)
      if (_M_key_compare(k, _S_key(x)))
        y = x, x = x->_M_left;
      else
        x = x->_M_right;
    return iterator(y);
  }

  const_iterator _M_upper_bound(_Const_Link_type x, _Const_Link_type y,
                                const _Key& k) const {
    while (x != 0)
      if (_M_key_compare(k, _S_key(x)))
        y = x, x = x->_M_left;
      else
        x = x->_M_right;
    return const_iterator(y);
  }

  // all_erase(root)
  /*
         3:root
       /   \
      1     5
     / \   / \
    0   2 4   7
             / \
            6   8
                 \
                  9

    9->8->...->0 で壊れていく
  */
  void _M_erase(_Link_type x) {
    // Erase without rebalancing.
    while (x != 0) {
      _M_erase(x->_M_right);
      _Link_type y = x->_M_left;
      _M_put_node(x);
      x = y;
    }
  }

  // valと色だけコピー。親、右、左はさらにクローンしたものをつけるから。
  _Link_type _M_clone_node(_Const_Link_type x) {
    _Link_type tmp = _M_create_node(*x->_M_value);
    tmp->_M_color = x->_M_color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
  }

  /*
        head
         |
         3:root
       /   \
      1     5
     / \   / \
    0   2 4   7
             / \
            6   8
                 \
                  9
  1(x:3 p:header)
  2(x:5 p:3)
  3(x:7 p:5)
  4(x:8 p:7)
  5(x:9 p:8)
  6(x:2 p:1) // 3->leftの再帰
  */
  _Link_type _M_copy(_Const_Link_type x, _Link_type p) {  // {origin, parent}
    _Link_type top = _M_clone_node(x);
    top->_M_parent = p;

    try {
      if (x->_M_right) top->_M_right = _M_copy(x->_M_right, top);
      p = top;
      x = x->_M_left;  // 左に進んで次の探索

      while (x != 0) {
        _Link_type y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right) y->_M_right = _M_copy(x->_M_right, y);
        p = y;
        x = x->_M_left;
      }
    } catch (...) {
      _M_erase(top);
      throw;  // throw_exception_again;
    }
    return top;
  }

 public:
  iterator find(const _Key& __k) {
    iterator __j = lower_bound(__k);
    if (__j == end() || _M_key_compare(__k, _S_key(__j)))
      return end();
    else
      return __j;
  }

  const_iterator find(const _Key& __k) const {
    const_iterator __j = lower_bound(__k);
    if (__j == end() || _M_key_compare(__k, _S_key(__j)))
      return end();
    else
      return __j;
  }

  iterator lower_bound(const key_type& k) {
    return _M_lower_bound(_S_root(), _M_end(), k);  // root , header
  }

  const_iterator lower_bound(const key_type& k) const {
    return _M_lower_bound(_S_root(), _M_end(), k);
  }

  iterator upper_bound(const key_type& k) {
    return _M_upper_bound(_S_root(), _M_end(), k);
  }

  const_iterator upper_bound(const key_type& k) const {
    return _M_upper_bound(_S_root(), _M_end(), k);
  }

  ft::pair<iterator, iterator> equal_range(const _Key& __k) {
    _Link_type __x = _S_root();
    _Link_type __y = _M_end();
    while (__x != 0) {
      if (_M_key_compare(_S_key(__x), __k))
        __x = __x->_M_right;
      else if (_M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = __x->_M_left;
      else {  // __x が keyに合致。 __yは__x->_M_parent
        _Link_type __xu(__x);
        _Link_type __yu(__y);
        __y = __x,
        __x = __x->_M_left;  // __yはkeyに一致。 __xはkeyより小さいかNULL
        __xu = __xu->_M_right;  // __xuはkeyより大きいかNULL
        /*
        __x : key->left
        __y : keyに一致
        __xu : key->right
        __yu : keyに一致

        _M_lower_bound(__x, __y, __k) :
        __x==NULLなら__y
        __xのできるだけ右を返す == keyの前

        _M_upper_bound(__xu, __yu, __k) :
        __xu==NULLなら__yu。
        __xuのできるだけ左を返す == keyの次

        つまり、
        p.first = M.lower_bound(key);
        p.second = M.upper_bound(key);
                を効率化してるだけ
        */
        return ft::pair<iterator, iterator>(_M_lower_bound(__x, __y, __k),
                                            _M_upper_bound(__xu, __yu, __k));
      }
    }
    // 末端まで来た
    return ft::pair<iterator, iterator>(iterator(__y), iterator(__y));
  }

  ft::pair<const_iterator, const_iterator> equal_range(
      const key_type& __k) const {
    _Const_Link_type __x = _S_root();
    _Const_Link_type __y = _M_end();
    while (__x != 0) {
      if (_M_key_compare(_S_key(__x), __k))
        __x = __x->_M_right;
      else if (_M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = __x->_M_left;
      else {  // __x が keyに合致。 __yは__x->_M_parent
        _Const_Link_type __xu(__x);
        _Const_Link_type __yu(__y);
        __y = __x,
        __x = __x->_M_left;  // __yはkeyに一致。 __xはkeyより小さいかNULL
        __xu = __xu->_M_right;  // __xuはkeyより大きいかNULL
        /*
        p.first = M.lower_bound(key);
        p.second = M.upper_bound(key);
                を効率化してるだけ
        */
        return ft::pair<const_iterator, const_iterator>(
            _M_lower_bound(__x, __y, __k), _M_upper_bound(__xu, __yu, __k));
      }
    }
    // 末端まで来た
    return ft::pair<const_iterator, const_iterator>(const_iterator(__y),
                                                    const_iterator(__y));
  }
};
//// Non-member functions

template <typename _Key, typename _Val, typename Compare, typename _Alloc>
inline bool operator==(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                       const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return __x.size() == __y.size() &&
         ft::equal(__x.begin(), __x.end(), __y.begin());
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>

inline bool operator<(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                      const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                     __y.end());
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>

inline bool operator!=(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                       const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return !(__x == __y);
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>

inline bool operator>(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                      const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return __y < __x;
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>

inline bool operator<=(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                       const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return !(__y < __x);
}

template <typename _Key, typename _Val, typename Compare, typename _Alloc>

inline bool operator>=(const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __x,
                       const ft::rb_tree<_Key, _Val, Compare, _Alloc>& __y) {
  return !(__x < __y);
}
}  // namespace ft
#endif