/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_access_iterator.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 09:47:56 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/19 11:33:26 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// random_access_iterator:	https://en.cppreference.com/w/cpp/iterator/random_access_iterator

#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

# include "iterator.hpp"

namespace	ft
{


template< typename T >
class	random_access_iterator
: public ft::iterator<ft::random_access_iterator_tag, T>
{
	public:
		// Member types
			typedef typename ft::iterator<ft::random_access_iterator_tag, T>
								::iterator_category		iterator_category;
			typedef typename ft::iterator<ft::random_access_iterator_tag, T>
								::value_type		value_type;
			typedef typename ft::iterator<ft::random_access_iterator_tag, T>
								::difference_type	difference_type;
			typedef T*	pointer;
			typedef T&	reference;
		
		// Member functions
			// Constructors and canonical
				random_access_iterator()
				: current(NULL)
				{

				}

				random_access_iterator( pointer x )
				: current(x)
				{

				}

				random_access_iterator( const random_access_iterator& other )
				: current(other.current)
				{

				}

				~random_access_iterator()
				{
					
				}

				random_access_iterator&		operator=( const random_access_iterator& other )
				{
					if (this == &other)
						return (*this);
					
					current = other.current;
					return (*this);
				}

			// Element access
				pointer		base() const
				{ return (current); }

				reference	operator*() const
				{ return (*current); }

				pointer		operator->() const
				{ return &(operator*()); }

				reference	operator[]( difference_type n ) const
				{ return *(*this + n); }

				random_access_iterator&		operator++()
				{
					++current;
					return (*this);
				}

				random_access_iterator&		operator--()
				{
					--current;
					return (*this);
				}

				random_access_iterator		operator++( int )
				{
					random_access_iterator	tmp = *this;
					++current;
					return (tmp);
				}

				random_access_iterator		operator--( int )
				{
					random_access_iterator	tmp = *this;
					--current;
					return (tmp);
				}

				random_access_iterator		operator+( difference_type n ) const
				{ return (random_access_iterator(current + n)); }

				random_access_iterator		operator-( difference_type n ) const
				{ return (random_access_iterator(current - n)); }

				random_access_iterator&		operator+=( difference_type n )
				{
					current += n;
					return (*this);
				}

				random_access_iterator&		operator-=( difference_type n )
				{
					current -= n;
					return (*this);
				}

				operator	random_access_iterator< const T >() const
				{ return (random_access_iterator<const T>(current)); }

	protected:
		T*	current;
};	// class random_access_iterator


// Non-member functions (random_access_iterator)

template< class Iterator1, class Iterator2 >
bool	operator==( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() == rhs.base()); }

template< class Iterator1, class Iterator2 >
bool	operator!=( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() != rhs.base()); }

template< class Iterator1, class Iterator2 >
bool	operator<( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() < rhs.base()); }

template< class Iterator1, class Iterator2 >
bool	operator>( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() > rhs.base()); }

template< class Iterator1, class Iterator2 >
bool	operator<=( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() <= rhs.base()); }

template< class Iterator1, class Iterator2 >
bool	operator>=( const ft::random_access_iterator<Iterator1>& lhs,
            		const ft::random_access_iterator<Iterator2>& rhs )
{ return (lhs.base() >= rhs.base()); }


template <class Iter>
ft::random_access_iterator<Iter>	operator+( typename ft::random_access_iterator<Iter>::difference_type n,
    											const ft::random_access_iterator<Iter>& it)
{ return ft::random_access_iterator<Iter>(it.base() + n); }

template< class Iter1, class Iter2 >
typename random_access_iterator<Iter1>::difference_type		operator-( const random_access_iterator<Iter1>& lhs
																		, const random_access_iterator<Iter2>& rhs )
{ return (lhs.base() - rhs.base()); }


}	// namespace ft

#endif
