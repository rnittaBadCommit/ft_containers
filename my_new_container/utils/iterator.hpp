/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 09:47:40 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/16 00:02:31 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// iterator library:	https://en.cppreference.com/w/cpp/iterator
// // iterator_traits:	https://en.cppreference.com/w/cpp/iterator/iterator_traits
// // iterator:			https://en.cppreference.com/w/cpp/iterator/iterator
// // reverse_iterator:	https://en.cppreference.com/w/cpp/iterator/reverse_iterator
// // distance:			https://en.cppreference.com/w/cpp/iterator/distance

#ifndef ITERATOR_HPP
# define ITERATOR_HPP

#include <cstddef>
# include "type_traits.hpp"

namespace ft
{


// Tags
struct	input_iterator_tag {};
struct	output_iterator_tag {};
struct	forward_iterator_tag : public input_iterator_tag {};
struct	bidirectional_iterator_tag : public forward_iterator_tag {};
struct	random_access_iterator_tag : public bidirectional_iterator_tag {};

template< class Iter >
struct	iterator_traits
{
	// Member types
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;
};

template< class T >
struct iterator_traits<T*>
{
	// Member types
		typedef ptrdiff_t						difference_type;
		typedef T								value_type;
		typedef T								*pointer;
		typedef T								&reference;
		typedef ft::random_access_iterator_tag	iterator_category;
};	// struct iterator_traits<T*>

template< class T >
struct iterator_traits<const T*>
{
	// Member types
		typedef ptrdiff_t							difference_type;
		typedef T									value_type;
		typedef T									*pointer;
		typedef T									&reference;
		typedef ft::random_access_iterator_tag		iterator_category;
};	// struct iterator_traits<const T*>

template< class Category, class T, class Distance = std::ptrdiff_t,
			class Pointer = T*, class Reference = T& >
struct iterator
{
	// Member types
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
};	// struct iterator

template< class Iter >
class	reverse_iterator
: public ft::iterator< typename ft::iterator_traits<Iter>::iterator_category, 
						typename ft::iterator_traits<Iter>::value_type, 
						typename ft::iterator_traits<Iter>::difference_type, 
						typename ft::iterator_traits<Iter>::pointer, 
						typename ft::iterator_traits<Iter>::reference >
{
	protected:
		typedef ft::iterator_traits<Iter>	traits_type;

	public:
		// Member type
			typedef	Iter 										iterator_type;
			typedef typename traits_type::iterator_category		iterator_category;
			typedef typename traits_type::value_type			value_type;
			typedef typename traits_type::difference_type		difference_type;
			typedef typename traits_type::pointer				pointer;
			typedef typename traits_type::reference				reference;
		
		// Member functions
			// Constructors and canonical
				reverse_iterator()
				: current()
				{

				}

				explicit reverse_iterator( iterator_type x )
				: current(x)
				{

				}

				template< class U >
				reverse_iterator( const reverse_iterator<U>& other )
				: current(other.base())
				{

				}

				template< class U >
				reverse_iterator	&operator=( const ft::reverse_iterator<U>& other )
				{
					if (this == &other)
						return (*this);
					
					current = other.base();
					return (*this);
				}

			// Element access
				iterator_type	base() const
				{ return (current); }

				reference	operator*() const
				{
					Iter	tmp = current;
					return (*--tmp);
				}

				pointer	operator->() const
				{ return &(operator*()); }

				reference	operator[]( difference_type n ) const
				{ return *(*this + n); }

				reverse_iterator& operator++()
				{
					--current;
					return (*this);
				}
				reverse_iterator& operator--()
				{
					++current;
					return (*this);
				}
				
				reverse_iterator operator++( int )
				{
					reverse_iterator tmp = *this;
					--current;
					return (tmp);
				}
				
				reverse_iterator operator--( int )
				{
					reverse_iterator tmp = *this;
					++current;
					return (tmp);
				}
				
				reverse_iterator operator+( difference_type n ) const
				{
					return reverse_iterator(current - n);
				}
				
				reverse_iterator operator-( difference_type n ) const
				{
					return reverse_iterator(current + n);
				}
				
				reverse_iterator& operator+=( difference_type n )
				{
					current -= n;
					return (*this);
				}
				
				reverse_iterator& operator-=( difference_type n )
				{
					current += n;
					return (*this);
				}

	protected:
		// Member objects
			iterator_type	current;

};	// class	reverse_iterator
	

// Non-member functions (reverse_iterator)

template< class Iterator1, class Iterator2 >
bool	operator==( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return lhs.base() == rhs.base(); }

template< class Iterator1, class Iterator2 >
bool	operator!=( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return !(lhs == rhs); }

template< class Iterator1, class Iterator2 >
bool	operator<( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return rhs.base() < lhs.base(); }

template< class Iterator1, class Iterator2 >
bool	operator>( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return lhs < rhs; }

template< class Iterator1, class Iterator2 >
bool	operator<=( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return !(rhs < lhs); }

template< class Iterator1, class Iterator2 >
bool	operator>=( const ft::reverse_iterator<Iterator1>& lhs,
            		const ft::reverse_iterator<Iterator2>& rhs )
{ return !(lhs < rhs); }

template< class Iter >
ft::reverse_iterator<Iter>	operator+( typename ft::reverse_iterator<Iter>::difference_type n,
            						const ft::reverse_iterator<Iter>& it )
{ return ft::reverse_iterator<Iter>(it.base() - n); }

template< class Iterator >
typename ft::reverse_iterator<Iterator>::difference_type
	operator-( const ft::reverse_iterator<Iterator>& lhs,
    			const ft::reverse_iterator<Iterator>& rhs )
{ return (rhs.base() - lhs.base()); }
// end of Non-member functions (reverse_iterator)


// other functions
template< class InputIt >
typename ft::iterator_traits<InputIt>::difference_type	distance( InputIt first, InputIt last)
{
	typename ft::iterator_traits<InputIt>::difference_type	ret = 0;

	while (first != last)
	{
		++first;
		++ret;
	}
	return (ret);
}


}	// namespace ft

#endif
