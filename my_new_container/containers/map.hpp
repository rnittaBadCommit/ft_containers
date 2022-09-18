/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 23:47:50 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/18 14:37:48 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// map: https://en.cppreference.com/w/cpp/container/map

#ifndef MAP_HPP
# define MAP_HPP

# include "../utils/rb_tree.hpp"

namespace ft {


template< class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<ft::pair<const Key, T> > >
class	map
{
	public:
		// Member types
			typedef Key						key_type;
			typedef T						mapped_type;
			typedef ft::pair<const Key, T>	value_type;
			typedef std::size_t				size_type;
			typedef std::ptrdiff_t			difference_type;
			typedef Compare					key_compare;
			typedef Allocator				allocator_type;

			typedef value_type&					reference;
			typedef const value_type&			const_reference;
			typedef typename Allocator::pointer			pointer;
			typedef typename Allocator::const_pointer	const_pointer;

	private:
		// Data
			typedef rb_tree<key_type, value_type, ft::_Select1st<value_type>,
							key_compare, allocator_type>	storage_type_;

			storage_type_	storage_;

	public:
		// Member types
			typedef typename storage_type_::iterator				iterator;
			typedef typename storage_type_::const_iterator			const_iterator;
			typedef typename storage_type_::reverse_iterator		reverse_iterator;
			typedef typename storage_type_::const_reverse_iterator	const_reverse_iterator;

		// Member classes
		class	value_compare : public std::binary_function<value_type, value_type, bool>
		{
			public:
				// Friend classes
					friend class	map<Key, T, Compare, Allocator>;

				// Member types
					bool		result_type;
					value_type	first_argument_type;
					value_type	second_argument_type;

				// Constructors and canonical
					value_compare(Compare c)
					: comp(c)
					{

					}

					bool	operator()(const value_type& lhs, const value_type& rhs ) const
					{ return (comp(lhs.first, rhs.first)); }

			protected:
				// Protected member objects
					Compare		comp;


		};	// class value_compare

		// Constructors and canonical
			map()
			: storage_()
			{

			}

			explicit map( const Compare& comp,
							const Allocator& alloc = Allocator() )
			: storage_(comp, alloc)
			{

			}

			template< class InputIt >
			map( InputIt first, InputIt last,
					const Compare& comp = Compare(),
					const Allocator& alloc = Allocator(),
					typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = NULL )
			: storage_(comp, alloc)
			{
				insert(first, last);
			}

			map( const map& other )
			: storage_(other.storage_)
			{

			}

			virtual		~map()
			{

			}

			map&	operator=( const map& other )
			{
				if (this == &other)
					return (*this);
				storage_ = other.storage_;
				return (*this);
			}

			allocator_type	get_allocator() const
			{ return (storage_.get_allocator()); }

		// Element access
			T&	at( const Key& key )
			{
				iterator	i = lower_bound(key);
				if (i == end() || key_comp()(key, (*i).first))
					throw std::out_of_range("map::at: out_of_range: no such key");
				return (*i).second;
			}

			const T&	at( const Key& key ) const
			{
				const_iterator	i = lower_bound(key);
				if (i == end() || key_comp()(key, (*i).first))
					throw std::out_of_range("map::at: out_of_range: no such key");
				return (*i).second;
			}

			T&	operator[]( const Key& key )
			{
				iterator	i = insert(value_type(key, mapped_type())).first;
				return (i->second);
			}

		// Iterators
			iterator	begin()
			{ return (storage_.begin()); }

			const_iterator	begin() const
			{ return (storage_.begin()); }

			iterator	end()
			{ return (storage_.end()); }

			const_iterator	end() const
			{ return (storage_.end()); }

			reverse_iterator	rbegin()
			{ return (storage_.rbegin()); }

			const_reverse_iterator	rbegin() const
			{ return (storage_.rbegin()); }

			reverse_iterator	rend()
			{ return (storage_.rend()); }

			const_reverse_iterator	rend() const
			{ return (storage_.rend()); }

		// Capacity
			bool	empty() const
			{ return (storage_.size() == 0); }

			size_type	size() const
			{ return (storage_.size()); }

			size_type	max_size() const
			{
				return storage_.max_size();
				// return std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max() / sizeof(value_type)),
				// 				alloc_.max_size());
			}

		// Modifiers
			void	clear()
			{ storage_.clear(); }

			ft::pair<iterator, bool>	insert( const value_type& value )
			{ return (storage_.insert(value)); }

			iterator	insert( iterator hint, const value_type& value )
			{
				(void)hint;
				return (insert(value).first);
			}

			template< class InputIt >
			void	insert( InputIt first, InputIt last,
							typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = NULL)
			{
				for (InputIt head = first; head != last; ++head)
					insert(*head);
			}

			void	erase( iterator pos )
			{ erase(pos->first); }

			void	erase( iterator first, iterator last )
			{
				if (first == begin() && last == end())
					clear();
				else
					while (first != last)
						erase(first++);
			}

			size_type	erase( const Key& key )
			{ return (storage_.erase(key)); }

			void	swap( map& other )
			{ storage_.swap(other.storage_); }

		// Lookup
			size_type	count( const Key& key ) const
			{ return (storage_.find(key) != storage_.end()); }

			iterator	find( const Key& key )
			{ return (storage_.find(key)); }

			const_iterator	find( const Key& key ) const
			{ return (storage_.find(key)); }

			ft::pair<iterator,iterator>		equal_range( const Key& key )
			{ return (storage_.equal_range(key)); }

			ft::pair<const_iterator,const_iterator>		equal_range( const Key& key ) const
			{ return (storage_.equal_range(key)); }

			iterator	lower_bound( const Key& key )
			{ return (storage_.lower_bound(key)); }

			const_iterator	lower_bound( const Key& key ) const
			{ return (storage_.lower_bound(key)); }
			
			iterator	upper_bound( const Key& key )
			{ return (storage_.upper_bound(key)); }

			const_iterator	upper_bound( const Key& key ) const
			{ return (storage_.upper_bound(key)); }

		// Observers
			key_compare	key_comp() const
			{ return (storage_.key_comp()); }

			value_compare	value_comp() const
			{ return value_compare(storage_.key_comp()); }

		// set friend
					template< class K1, class T1, class C1, class A1 >
					friend bool		operator==( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

					template< class K1, class T1, class C1, class A1 >
					friend bool		operator!=( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

					template< class K1, class T1, class C1, class A1 >
					friend bool		operator<( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

					template< class K1, class T1, class C1, class A1 >
					friend bool		operator<=( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

					template< class K1, class T1, class C1, class A1 >
					friend bool		operator>( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

					template< class K1, class T1, class C1, class A1 >
					friend bool		operator>=( const map<K1,T1,C1,A1>& lhs,
												const map<K1,T1,C1,A1>& rhs );

};	// class map


// Non-member functions
	// Operators
		template< class Key, class T, class Compare, class Alloc >
		bool	operator==( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ == rhs.storage_); }

		template< class Key, class T, class Compare, class Alloc >
		bool	operator!=( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ != rhs.storage_); }

		template< class Key, class T, class Compare, class Alloc >
		bool	operator<( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ < rhs.storage_); }

		template< class Key, class T, class Compare, class Alloc >
		bool	operator<=( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ <= rhs.storage_); }

		template< class Key, class T, class Compare, class Alloc >
		bool	operator>( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ > rhs.storage_); }

		template< class Key, class T, class Compare, class Alloc >
		bool	operator>=( const ft::map<Key,T,Compare,Alloc>& lhs,
							const ft::map<Key,T,Compare,Alloc>& rhs )
		{ return (lhs.storage_ >= rhs.storage_); }



};	// namespace ft

#endif
