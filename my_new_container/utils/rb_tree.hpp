#ifndef RB_TREE_HPP
# define RB_TREE_HPP

#include <iostream>
#include <limits>

#include "./algorithm.hpp"
#include "./iterator.hpp"
#include "./pair.hpp"
#include "./util.hpp"

namespace	ft
{

/*
				[begin](smallest)
				|
				[DUMMY]
				/		\
			[RB_TREE_ROOT_](7)	[DUMMY]
			/		\
		(4)			(14)
		/			/	\
	[begin](1)	  (10)	(18)
							\
							[end - 1](30)
*/

/*	size=0
		NULL
		|
		[DUMMY]
		/		\
	NULL		[DUMMY]
*/


template< class T >
class	rb_node_
{
	public:
		typedef bool			color_type;
		typedef T				value_type;
		typedef rb_node_<T>*	link_type;

		link_type	parent;
		link_type	left;
		link_type	right;
		color_type	color;
		value_type	*value;

		static const bool	BLACK = true;
		static const bool	RED = false;

		link_type	uncle()
		{
			if (parent == NULL || parent->parent == NULL)
				return (NULL);
			else if (parent->right == parent || parent->parent->right == parent->parent)
				return (NULL);
			else if (parent->isOnLeft())
				return (parent->parent->right);
			else
				return (parent->parent->left);
		}

		bool	isOnLeft()
		{ return (this == parent->left); }

		bool	isDummy()
		{ return (this == this->right); }

		link_type	sibling()
		{
			if (parent == NULL)
				return (NULL);
			else if (parent->right == parent)
				return (NULL);
			else if (isOnLeft())
				return (parent->right);
			else
				return (parent->left);
		}

		void	moveDown( link_type new_parent )
		{
			if (parent != NULL)
			{
				if (isOnLeft())
					parent->left = new_parent;
				else
					parent->right = new_parent;
			}
			new_parent->parent = parent;
			parent = new_parent;
		}

		bool	hasRedChild()
		{ return (left != NULL && left->color == RED)
					|| (right != NULL && right->color == RED); }

		void	swapNode( link_type other )
		{
			link_type	other_parent = other->parent;
			link_type	other_left = other->left;
			link_type	other_right = other->right;
			link_type	this_parent = this->parent;
			link_type	this_left = this->left;
			link_type	this_right = this->right;

			if (other->isOnLeft())
				other_parent->left = this;
			else
				other_parent->right = this;
			if (other_left)
				other_left->parent = this;
			if (other_right)
				other_right->parent = this;

			if (this->isOnLeft())
				this_parent->left = other;
			else
				this_parent->right = other;
			if (this_left)
				this_left->parent = other;
			if (this_right)
				this_right->parent = other;

			std::swap(this->color, other->color);
			std::swap(this->parent, other->parent);
			std::swap(this->right, other->right);
			std::swap(this->left, other->left);
		}

		link_type	leftest()
		{
			link_type	tmp = this;

			while (tmp->left != NULL)
				tmp = tmp->left;
			return (tmp);
		}

		link_type	rightest()
		{
			link_type	tmp = this;
			while (tmp->right != NULL)
				tmp = tmp->right;
			return (tmp);
		}
	// ~public:
		
};	// class rb_node_

template< typename Val_ >
static rb_node_<Val_>*	local_rb_tree_increment_( rb_node_<Val_>* node) throw()
{
	if (node->parent == NULL)
		return (node);
	
	if (node->right)
		return (node->right->leftest());
	else
	{
		rb_node_<Val_>*		ret = node->parent;
		rb_node_<Val_>*		tmp = node;
		while (!tmp->isOnLeft())
		{
			tmp = ret;
			ret = ret->parent;
		}
		return (ret);
	}
}

template< typename Val_ >
rb_node_<Val_>*		rb_tree_increment(rb_node_<Val_>* node) throw()
{ return (local_rb_tree_increment_(node)); }

template< typename Val_ >
const rb_node_<Val_>*	rb_tree_increment(const rb_node_<Val_>* node) throw()
{ return (local_rb_tree_increment_(const_cast<rb_node_<Val_>*>(node))); }


template< typename Val_ >
static rb_node_<Val_>*	local_rb_tree_decrement_( rb_node_<Val_>* node) throw()
{
	if (node->parent == NULL)
		return (node);
	
	if (node->left)
		return (node->left->rightest());
	else
	{
		while (node->isOnLeft())
			node = node->parent;
		if (node->right == node)	// input node is begin (current node is DUMMY)
			return (node->parent);	// return begin (no change)
		else
			return (node->parent);
	}
}

template< typename Val_ >
rb_node_<Val_>*		rb_tree_decrement(rb_node_<Val_>* node) throw()
{ return (local_rb_tree_decrement_(node)); }

template< typename Val_ >
const rb_node_<Val_>*	rb_tree_decrement(const rb_node_<Val_>* node) throw()
{ return (local_rb_tree_decrement_(const_cast<rb_node_<Val_>*>(node))); }



template< typename T >
class	rb_iterator_
{
	public:
		typedef T	value_type;
		typedef T&	reference;
		typedef T*	pointer;

		typedef ft::bidirectional_iterator_tag	iterator_category;
 		typedef ptrdiff_t	difference_type;

	private:
		typedef rb_iterator_<T>						Self_;
		typedef typename rb_node_<T>::link_type		link_type;
	
	public:
		rb_iterator_()
		: iterator_node_()
		{

		}

		explicit	rb_iterator_( link_type node )
		: iterator_node_(node)
		{

		}

		reference	operator*() const
		{ return (*iterator_node_->value); }

		pointer		operator->() const
		{ return (iterator_node_->value); }

		Self_&	operator++()
		{
			iterator_node_ = rb_tree_increment(iterator_node_);
			return (*this);
		}
		
		Self_	operator++( int )
		{
			Self_	tmp = *this;

			iterator_node_ = rb_tree_increment(iterator_node_);
			return (tmp);
		}

		Self_	operator--()
		{
			iterator_node_ = rb_tree_decrement(iterator_node_);
			return (*this);
		}

		Self_	operator--( int )
		{
			Self_	tmp = *this;

			iterator_node_ = rb_tree_decrement(iterator_node_);
			return (tmp);
		}

		bool	operator==( const Self_& other ) const
		{ return (iterator_node_ == other.iterator_node_); }
		
		bool	operator!=( const Self_& other ) const
		{ return (iterator_node_ != other.iterator_node_); }

		link_type	get_link() const
		{ return (iterator_node_); }
	// ~public:

	protected:
		link_type	iterator_node_;

};

template< typename T >
class	rb_const_iterator_
{
	public:
		typedef T	value_type;
		typedef const T&	reference;
		typedef const T*	pointer;

		typedef ft::bidirectional_iterator_tag	iterator_category;
 		typedef ptrdiff_t	difference_type;

	private:
		typedef rb_const_iterator_<T>	Self_;
		typedef const rb_node_<T>*		link_type;
	
	public:
		rb_const_iterator_()
		: iterator_node_()
		{

		}

		explicit	rb_const_iterator_( link_type node )
		: iterator_node_(node)
		{

		}

		reference	operator*() const
		{ return (*iterator_node_->value); }

		pointer		operator->() const
		{ return (iterator_node_->value); }

		Self_&	operator++()
		{
			iterator_node_ = rb_tree_increment(iterator_node_);
			return (*this);
		}
		
		Self_	operator++( int )
		{
			Self_	tmp = *this;

			iterator_node_ = rb_tree_increment(iterator_node_);
			return (tmp);
		}

		Self_	operator--()
		{
			iterator_node_ = rb_tree_decrement(iterator_node_);
			return (*this);
		}

		Self_	operator--( int )
		{
			Self_	tmp = *this;

			iterator_node_ = rb_tree_decrement(iterator_node_);
			return (tmp);
		}

		bool	operator==( const Self_& other ) const
		{ return (iterator_node_ == other.iterator_node_); }

		bool	operator!=( const Self_& other ) const
		{ return (iterator_node_ != other.iterator_node_); }

		link_type	get_link() const
		{ return (iterator_node_); }
	// ~public:
	

	protected:
		link_type	iterator_node_;

};



template< typename Key_, typename Val_, typename KeyOfValue_, typename Compare_, typename Allocator_ = std::allocator<Val_> >
class	rb_tree
{
	public:
		// Member types
			typedef rb_node_<Val_>			node_type;
			typedef const rb_node_<Val_>	const_node_type;

			typedef typename rb_node_<Val_>::color_type		color_type;
			typedef size_t			size_type;
			typedef std::ptrdiff_t	difference_type;
			typedef Key_			key_type;
			typedef Val_			value_type;
			typedef Allocator_		allocator_type;
			typedef Compare_		key_compare_type;

			typedef Val_*			pointer;
			typedef const Val_*		const_pointer;
			typedef Val_&			reference;
			typedef const Val_&		const_reference;

			typedef rb_iterator_<Val_>						iterator;
			typedef rb_const_iterator_<Val_>				const_iterator;
			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;


			typedef typename Allocator_::template rebind<rb_node_<Val_> >::other
    				node_allocator_type;

			static const color_type	BLACK = rb_node_<Val_>::BLACK;
			static const color_type	RED = rb_node_<Val_>::RED;

	protected:
		typedef rb_node_<Val_>*			link_type;
		typedef const rb_node_<Val_>*	const_link_type;

		// Member objects
			link_type				dummy_;
			link_type				prev_end;
			node_allocator_type		node_alloc_;
			size_type				size_;
			Compare_				key_compare_;
		
// Macro(define)
#define RB_TREE_ROOT_		dummy_->left
#define RB_TREE_BEGIN_		dummy_->parent
#define RB_TREE_PREV_RB_TREE_END_	prev_end
#define RB_TREE_END_			dummy_

	public:
		// Constructors
			rb_tree()
			: node_alloc_(node_allocator_type()), key_compare_(Compare_())
			{
				dummy_ = createNode(value_type());
				initialize();
			}

			rb_tree( const key_compare_type& key_comp, const node_allocator_type& node_alloc)
			: node_alloc_(node_alloc), key_compare_(key_comp)
			{
				dummy_ = createNode(value_type());
				initialize();
			}

			rb_tree( const rb_tree& other)
			: node_alloc_(other.node_alloc_), key_compare_(other.key_compare_)
			{
				dummy_ = createNode(value_type());
				initialize();
				if (other.size_)
				{
					RB_TREE_ROOT_ = copyTree(other.RB_TREE_ROOT_, dummy_);
					RB_TREE_BEGIN_ = RB_TREE_ROOT_->leftest();
					RB_TREE_PREV_RB_TREE_END_ = RB_TREE_ROOT_->rightest();
				}
			}

			~rb_tree()
			{
				clear();
				destroyNode(dummy_);
			}

			rb_tree&	operator=( const rb_tree& other)
			{
				if (this == &other)
					return (*this);
				
				clear();
				key_compare_ = other.key_compare_;
				node_alloc_ = other.node_alloc_;

				if (other.size_)
				{
					RB_TREE_ROOT_ = copyTree(other.RB_TREE_ROOT_, dummy_);
					RB_TREE_BEGIN_ = RB_TREE_ROOT_->leftest();
					RB_TREE_PREV_RB_TREE_END_ = RB_TREE_ROOT_->rightest();
				}

				return (*this);
			}

			key_compare_type	key_comp() const
			{ return (key_compare_); }

			allocator_type	get_allocator() const
			{ return (allocator_type(node_alloc_)); }
		// ~Constructors



		// Iterator
			inline iterator		begin()
			{ return (iterator(dummy_->parent)); }

			inline const_iterator	begin() const
			{ return (const_iterator(dummy_->parent)); }

			inline iterator		end()
			{ return (iterator(dummy_)); }

			inline const_iterator	end() const
			{ return (const_iterator(dummy_)); }

			inline reverse_iterator		rbegin()
			{ return (reverse_iterator(end())); }

			inline const_reverse_iterator	rbegin() const
			{ return (const_reverse_iterator(end())); }

			inline reverse_iterator		rend()
			{ return (reverse_iterator(begin())); }

			inline const_reverse_iterator	rend() const
			{ return (const_reverse_iterator(begin())); }


		// Capacity
			bool	empty() const
			{ return (size_ == 0); }

			size_type	size() const
			{ return (size_); }

			size_type	max_size() const
			{ return std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max() / sizeof(value_type)),
				node_alloc_.max_size()); }

		// Modifiers
			void	clear()
			{
				clear_process_(RB_TREE_ROOT_);
				initialize();
			}

			ft::pair<iterator, bool>	insert( const value_type &value )
			{
				link_type	insert_place = findInsertPlace(value);
				return (insertNode(value, insert_place));
			}

			size_type	erase( const key_type& key)
			{
				//std::cout << std::endl;
				//std::cout << "before" << std::endl;
				// printTree(RB_TREE_ROOT_);
				//std::cout << "erase key: " << key << std::endl;
				
				if (RB_TREE_ROOT_ == NULL)
					return (0);

				iterator	it_to_delete = lower_bound(key);
				if (it_to_delete.get_link() == RB_TREE_END_ || key_compare_(key, key_(it_to_delete)))
					return (0);

				link_type	save_it_to_delete = it_to_delete.get_link();
				link_type	save_begin = RB_TREE_BEGIN_;
				link_type	save_prev_end = RB_TREE_PREV_RB_TREE_END_;

				deleteNode(it_to_delete.get_link());
				//std::cout << "after" << std::endl;
				// printTree(RB_TREE_ROOT_);
				if (size_)
				{
					if (save_it_to_delete == save_begin)
						RB_TREE_BEGIN_ = RB_TREE_ROOT_->leftest();
					if (save_it_to_delete == save_prev_end)
						RB_TREE_PREV_RB_TREE_END_ = RB_TREE_ROOT_->rightest();
				}
				else
					initialize();
				
				return (1);				
			}

			void	swap( rb_tree& other)
			{
				if (this == &other)
					return ;

				if (size_ == 0)
				{
					if (other.size_ != 0)
						move_data(other);
				}
				else if (other.size_ == 0)
					other.move_data(*this);
				else
				{
					std::swap(RB_TREE_ROOT_, other.RB_TREE_ROOT_);
					std::swap(RB_TREE_BEGIN_, other.RB_TREE_BEGIN_);
					std::swap(RB_TREE_PREV_RB_TREE_END_, other.RB_TREE_PREV_RB_TREE_END_);
					RB_TREE_ROOT_->parent = RB_TREE_END_;
					other.RB_TREE_ROOT_->parent = other.RB_TREE_END_;
					std::swap(size_, other.size_);
				}
				std::swap(key_compare_, other.key_compare_);
				std::swap(node_alloc_, other.node_alloc_);
			}

		// ~Modifiers

		// Lookup
			iterator	find( const key_type& key)
			{
				iterator	it = lower_bound(key);
				if (it.get_link() == RB_TREE_END_ || key_compare_(key, key_(it)))
					return (iterator(RB_TREE_END_));
				else
					return (it);
			}

			const_iterator	find( const key_type& key) const
			{
				const_iterator	it = lower_bound(key);
				if (it.get_link() == RB_TREE_END_ || key_compare_(key, key_(it)))
					return (const_iterator(RB_TREE_END_));
				else
					return (it);
			}

			ft::pair<iterator, iterator>	equal_range( const key_type& key)
			{
				link_type	lower_x = RB_TREE_ROOT_;
				link_type	lower_y = RB_TREE_END_;

				while (lower_x)
				{
					if (key_compare_(key_(lower_x), key))
						lower_x = lower_x->right;
					else if (key_compare_(key, key_(lower_x)))
					{
						lower_y = lower_x;
						lower_x = lower_x->left;
					}
					else
					{
						link_type	upper_x(lower_x);
						link_type	upper_y(lower_y);

						lower_y = lower_x;
						lower_x = lower_x->left;
						upper_x = upper_x->right;
						return (ft::pair<iterator, iterator>(lower_bound_(lower_x, lower_y, key),
																upper_bound_(upper_x, upper_y, key)));
					}
				}
				return (ft::pair<iterator, iterator>(iterator(lower_y), iterator(lower_y)));
			}

			ft::pair<const_iterator, const_iterator>	equal_range( const key_type& key) const
			{
				const_link_type		lower_x = RB_TREE_ROOT_;
				const_link_type		lower_y = RB_TREE_END_;

				while (lower_x)
				{
					if (key_compare_(key_(lower_x), key))
						lower_x = lower_x->right;
					else if (key_compare_(key, key_(lower_x)))
					{
						lower_y = lower_x;
						lower_x = lower_x->left;
					}
					else
					{
						const_link_type		upper_x(lower_x);
						const_link_type		upper_y(lower_y);

						lower_y = lower_x;
						lower_x = lower_x->left;
						upper_x = upper_x->right;
						return (ft::pair<const_iterator, const_iterator>(lower_bound_(lower_x, lower_y, key),
																upper_bound_(upper_x, upper_y, key)));
					}
				}
				return (ft::pair<const_iterator, const_iterator>(const_iterator(lower_y), const_iterator(lower_y)));
			}


			iterator	lower_bound( const key_type& key )
			{ return (lower_bound_(RB_TREE_ROOT_, RB_TREE_END_, key)); }

			const_iterator	lower_bound( const key_type& key ) const
			{ return (lower_bound_((const_link_type)RB_TREE_ROOT_, (const_link_type)RB_TREE_END_, key)); }

			iterator	upper_bound( const key_type& key)
			{ return (upper_bound_(RB_TREE_ROOT_, RB_TREE_END_, key)); }

			const_iterator	upper_bound( const key_type& key) const
			{ return (upper_bound_((const_link_type)RB_TREE_ROOT_, (const_link_type)RB_TREE_END_, key)); }



	protected:
		//
			void	initialize()
			{
				dummy_->color = RED;
				RB_TREE_ROOT_ = NULL;
				RB_TREE_BEGIN_ = dummy_;
				RB_TREE_PREV_RB_TREE_END_ = NULL;
				dummy_->right = dummy_;

				size_ = 0;
			}

		// Create ,Construct, Destroy nodes
			link_type	allocate_node_()
			{
				++size_;
				return (node_alloc_.allocate(1));
			}

			link_type	createNode( const value_type& value )
			{
				link_type	new_node = node_alloc_.allocate(1);

				new_node->color = RED;
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->parent = NULL;
				Allocator_	value_alloc;
				typename allocator_type::pointer	p_value = value_alloc.allocate(1);
				value_alloc.construct(p_value, value);
				new_node->value = p_value;
				++size_;
				return (new_node);
			}

			link_type	copyNode( const_link_type src )
			{
				link_type	new_node = createNode(*src->value);

				new_node->color = src->color;
				return (new_node);
			}

			link_type	copyTree( const_link_type src, link_type parent )
			{
				link_type	new_node = copyNode(src);

				new_node->parent = parent;
				if (src->left)
					new_node->left = copyTree(src->left, new_node);
				if (src->right)
					new_node->right = copyTree(src->right, new_node);
				return (new_node);
			}

			void	destroyNode(link_type node)
			{
				Allocator_	value_alloc;
				value_alloc.destroy(node->value);
				value_alloc.deallocate(node->value, 1);
				node_alloc_.destroy(node);
				node_alloc_.deallocate(node, 1);
				--size_;
			}

		// Rotates
		void	leftRotate(link_type rotate_root)
		{
			link_type	new_parent = rotate_root->right;
			if (rotate_root == RB_TREE_ROOT_)
				RB_TREE_ROOT_ = rotate_root;
			
			rotate_root->moveDown(new_parent);

			rotate_root->right = new_parent->left;
			if (rotate_root->right != NULL)
				rotate_root->right->parent = rotate_root;
			new_parent->left = rotate_root;
		}

		void	rightRotate(link_type rotate_root)
		{
			link_type	new_parent = rotate_root->left;
			if (rotate_root == RB_TREE_ROOT_)
				RB_TREE_ROOT_ = rotate_root;
			
			rotate_root->moveDown(new_parent);

			rotate_root->left = new_parent->right;
			if (rotate_root->left != NULL)
				rotate_root->left->parent = rotate_root;
			new_parent->right = rotate_root;
		}

		void	swapColors(link_type n1, link_type n2)
		{
			color_type	tmp = n1->color;
			n1->color = n2->color;
			n2->color = tmp;
		}

		void	swapValues(link_type n1, link_type n2)
		{
			value_type	tmp = n1->value_;
			n1->value_ = n2->value_;
			n2->value_ = tmp;
		}

		void	fixRedRed(link_type child_to_fix)
		{
			if (child_to_fix == RB_TREE_ROOT_)
			{
				RB_TREE_ROOT_->color = BLACK;
				return ;
			}

			link_type	parent = child_to_fix->parent;
			link_type	grandparent = parent->parent;
			if (grandparent->isDummy())
				grandparent = NULL;
			link_type	uncle = child_to_fix->uncle();

			if (parent->color == RED)
			{
				if (uncle != NULL && uncle->color == RED)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					grandparent->color = RED;
					fixRedRed(grandparent);
				}
				else
				{	// uncle: BLACK
					if (parent->isOnLeft())
					{
						if (child_to_fix->isOnLeft())
							swapColors(parent, grandparent);
						else
						{
							leftRotate(parent);
							swapColors(child_to_fix, grandparent);
						}
						rightRotate(grandparent);
					}
					else
					{	// parent: onRight
						if (child_to_fix->isOnLeft())
						{
							rightRotate(parent);
							swapColors(child_to_fix, grandparent);
						}
						else
							swapColors(parent, grandparent);
						leftRotate(grandparent);
					}
				}
			}
		}



		link_type	newSubroot_toReplace(link_type subroot_to_delete)
		{
			if (subroot_to_delete->left != NULL && subroot_to_delete->right != NULL)
				return (subroot_to_delete->right->leftest());

			if (subroot_to_delete->left == NULL && subroot_to_delete->right == NULL)
				return (NULL);
			if (subroot_to_delete->left != NULL)
				return (subroot_to_delete->left);
			else
				return (subroot_to_delete->right);
		}

		void	deleteNode(link_type node_to_delete)
		{
			link_type	new_subroot = newSubroot_toReplace(node_to_delete);
			link_type	parent_of_subroot = node_to_delete->parent;

			if (new_subroot == NULL)
			{
				if (node_to_delete == RB_TREE_ROOT_)
					RB_TREE_ROOT_ = NULL;
				else
				{
					if ((new_subroot == NULL || new_subroot->color == BLACK)
						&& (node_to_delete->color == BLACK))
					{
						fixDoubleBlack(node_to_delete);
					}
					else
					{
						if (node_to_delete->sibling() != NULL)
							node_to_delete->sibling()->color = RED;
					}

					if (node_to_delete->isOnLeft())
						parent_of_subroot->left = NULL;
					else
						parent_of_subroot->right = NULL;
				}
				destroyNode(node_to_delete);
			}
			else if (node_to_delete->left == NULL
					|| node_to_delete->right == NULL)
			{
				if (node_to_delete == RB_TREE_ROOT_)
				{
					// std::cout << "here" << std::endl;
					node_to_delete->swapNode(new_subroot);
					new_subroot->left = NULL;
					new_subroot->right = NULL;
					destroyNode(node_to_delete);
					// std::cout << "node_to_delete: [" << node_to_delete << "]" << std::endl;
					// std::cout << "deleteNode done" << std::endl;
 				}
				else
				{
					if (node_to_delete->isOnLeft())
						parent_of_subroot->left = new_subroot;
					else
						parent_of_subroot->right = new_subroot;
					destroyNode(node_to_delete);
					new_subroot->parent = parent_of_subroot;
					if ((new_subroot == NULL || new_subroot->color == BLACK)
						&& (node_to_delete->color == BLACK))
						fixDoubleBlack(new_subroot);
					else
						new_subroot->color = BLACK;
				}
			}
			else
			{
				new_subroot->swapNode(node_to_delete);
				deleteNode(node_to_delete);
			}
		}

		void	fixDoubleBlack(link_type child_to_fix)
		{
			if (child_to_fix == RB_TREE_ROOT_)
				return ;
			
			link_type	parent = child_to_fix->parent;
			link_type	sibling = child_to_fix->sibling();

			if (sibling == NULL)
				fixDoubleBlack(parent);
			else
			{
				if (sibling->color == RED)
				{
					parent->color = RED;
					sibling->color = BLACK;
					if (sibling->isOnLeft())
						rightRotate(parent);
					else
						leftRotate(parent);
					fixDoubleBlack(child_to_fix);
				}
				else
				{
					if (sibling->hasRedChild())
					{
						if (sibling->left != NULL && sibling->left->color == RED)
						{
							if (sibling->isOnLeft())
							{
								sibling->left->color = sibling->color;
								sibling->color = parent->color;
								rightRotate(parent);
							}
							else
							{
								sibling->left->color = parent->color;
								rightRotate(sibling);
								leftRotate(parent);
							}
						}
						else
						{
							if (sibling->isOnLeft())
							{
								sibling->right->color = parent->color;
								leftRotate(sibling);
								rightRotate(parent);
							}
							else
							{
								sibling->right->color = sibling->color;
								sibling->color = parent->color;
								leftRotate(parent);
							}
						}
						parent->color = BLACK;
					}
					else
					{
						sibling->color = RED;
						if (parent->color == BLACK)
							fixDoubleBlack(parent);
						else
							parent->color = BLACK;
					}
				}
			}
		}

		link_type	findInsertPlace(const value_type &value)
		{
			link_type	current_node = RB_TREE_ROOT_;

			if (current_node)
				while (1)
				{
					if (key_compare_(key_(value), key_(current_node)))
					{
						if (current_node->left == NULL)
							return (current_node);
						current_node = current_node->left;
					}
					else if (key_compare_(key_(current_node), key_(value)))
					{
						if (current_node->right == NULL)
							return (current_node);
						current_node = current_node->right;
					}
					else
						return (current_node);
				}
			return (dummy_);
		}

		pair<iterator, bool> 	insertNode(const value_type &value, link_type pos)
		{
			link_type	new_node;

			if (size_ == 0)
			{
				new_node = createNode(value);
				new_node->parent = pos;
				RB_TREE_ROOT_ = new_node;
				RB_TREE_BEGIN_ = new_node;
				RB_TREE_PREV_RB_TREE_END_ = new_node;
			}
			else if (key_compare_(key_(value), key_(pos)))
			{
				new_node = createNode(value);
				new_node->parent = pos;
				pos->left = new_node;
				if (pos == RB_TREE_BEGIN_)
					RB_TREE_BEGIN_ = new_node;
			}
			else if (key_compare_(key_(pos), key_(value)))
			{
				new_node = createNode(value);
				new_node->parent = pos;
				pos->right = new_node;
				if (pos == RB_TREE_PREV_RB_TREE_END_)
					RB_TREE_PREV_RB_TREE_END_ = new_node;
			}
			else
				return (ft::make_pair(iterator(pos), false));

			link_type	save_new_node = new_node;
			fixRedRed(new_node);
			RB_TREE_ROOT_->color = BLACK;
			RB_TREE_ROOT_->parent = dummy_;
			return (ft::make_pair(iterator(save_new_node), true));
		}

key_type key_(link_type x) const { return (KeyOfValue_()(*(x->value))); }
key_type key_(const_link_type x) const { return (KeyOfValue_()(*(x->value))); }
key_type key_(value_type x) const { return (KeyOfValue_()(x)); }
key_type key_(iterator x) const { return (KeyOfValue_()(*x)); }
key_type key_(const_iterator x) const { return (KeyOfValue_()(*x)); }
link_type root_() { return (RB_TREE_ROOT_); }
const_link_type root_() const { return (RB_TREE_ROOT_); }
	// ~protected:

	private:
		void	clear_process_( link_type node)
		{
			while (node)
			{
				clear_process_(node->right);
				link_type	tmp = node->left;
				destroyNode(node);
				node = tmp;
			}
		}

		iterator	lower_bound_( link_type x, link_type y, const key_type& key)
		{
			while (x)
			{
				if (!key_compare_(key_(x), key))
				{
					y = x;
					x = x->left;
				}
				else
					x = x->right;
			}
			return (iterator(y));
		}

		const_iterator	lower_bound_( const_link_type x, const_link_type y, const key_type& key) const
		{
			while (x)
			{
				if (!key_compare_(key_(x), key))
				{
					y = x;
					x = x->left;
				}
				else
					x = x->right;
			}
			return (const_iterator(y));
		}

		iterator	upper_bound_( link_type x, link_type y, const key_type& key)
		{
			while (x)
			{
				if (key_compare_(key, key_(x)))
				{
					y = x;
					x = x->left;
				}
				else
					x = x->right;
			}
			return (iterator(y));
		}

		const_iterator	upper_bound_( const_link_type x, const_link_type y, const key_type& key) const
		{
			while (x)
			{
				if (key_compare_(key, key_(x)))
				{
					y = x;
					x = x->left;
				}
				else
					x = x->right;
			}
			return (const_iterator(y));
		}

		void	move_data( rb_tree& from )
		{
			dummy_->color = from.dummy_->color;
			dummy_->parent = from.dummy_->parent;
			dummy_->left= from.dummy_->left;
			dummy_->right = from.dummy_->right;
			RB_TREE_ROOT_->parent = dummy_;
			size_ = from.size_;
			from.initialize();
		}

		void	printTree_( const_link_type root, int space_width )
		{
			if (root)
			{
				space_width += 10;
				printTree_(root->right, space_width);
				std::cout << std::endl;
				for (int i = 10; i < space_width; ++i)
					std::cout << " ";
				if (root->color == BLACK)
					std::cout << " " << key_(root) << " ";
				else
					std::cout << "(" << key_(root) << ")";
				std::cout << "[" << root << "]" << std::endl;
				printTree_(root->left, space_width);
			}
		}

		void	printTree( const_link_type root )
		{
			std::cout << "============================================" << std::endl;
			std::cout << "dummy_: [" << dummy_ <<  "]  dummy_->left: " << dummy_->left << std::endl;;
			printTree_(root, 0);
			std::cout << "============================================" << std::endl;
			std::cout << std::endl;
		}

// Macro(undef)
#undef RB_TREE_ROOT_
#undef RB_TREE_BEGIN_
#undef RB_TREE_PREV_RB_TREE_END_
#undef RB_TREE_END_

};	// class rb_tree


// Non-member functions
	template< typename Key, typename Val, typename Compare, typename Alloc >
	inline bool operator==( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return lhs.size() == rhs.size() &&
			ft::equal(lhs.begin(), lhs.end(), rhs.begin()); }

	template< typename Key, typename Val, typename Compare, typename Alloc >

	inline bool operator<( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
										rhs.end()); }

	template< typename Key, typename Val, typename Compare, typename Alloc >
	inline bool operator!=( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return !(lhs == rhs); }

	template< typename Key, typename Val, typename Compare, typename Alloc >
	inline bool operator>( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return rhs < lhs; }

	template< typename Key, typename Val, typename Compare, typename Alloc >
	inline bool operator<=( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return !(rhs < lhs); }

	template< typename Key, typename Val, typename Compare, typename Alloc >
	inline bool operator>=( const ft::rb_tree<Key, Val, Compare, Alloc>& lhs,
							const ft::rb_tree<Key, Val, Compare, Alloc>& rhs )
	{ return !(lhs < rhs); }

};	// namespace ft


#endif
