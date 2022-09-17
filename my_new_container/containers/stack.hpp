/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 23:47:01 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/16 00:08:22 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// stack:	https://en.cppreference.com/w/cpp/container/stack

#ifndef STACK_HPP
# define STACK_HPP

# include <iostream>
# include "./vector.hpp"

namespace ft {


template< class T, class Container = ft::vector<T> >
class	stack
{
	public:
		// Member types
			typedef Container								container_type;
			typedef typename Container::value_type			value_type;
			typedef typename Container::size_type			size_type;
			typedef typename Container::reference			reference;
			typedef typename Container::const_reference		const_reference;

		// Member functions
			// Constructors and canonical
				explicit	stack( const Container& cont = Container() )
				: c(cont)
				{

				}

				stack( const stack& other )
				: c(other.c)
				{

				}

				~stack()
				{

				}

				stack&	operator=( const stack& other )
				{
					if (this == &other)
						return (*this);

					c = other.c;
					return (*this);
				}

			// Element access
				reference	top()
				{ return (c.back()); }

				const_reference		top() const
				{ return (c.back()); }

			// Capacity
				bool	empty() const
				{ return (c.empty()); }

				size_type	size() const
				{ return (c.size()); }

				void	push( const value_type& value )
				{ return (c.push_back(value)); }

			// Modifiers
				void	pop()
				{ c.pop_back(); }

	protected:
		container_type	c;

	// Set friend
		template< class T1, class C1 >
		friend bool		operator==( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );

		template< class T1, class C1 >
		friend bool		operator!=( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );

		template< class T1, class C1 >
		friend bool		operator<( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );

		template< class T1, class C1 >
		friend bool		operator<=( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );

		template< class T1, class C1 >
		friend bool		operator>( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );

		template< class T1, class C1 >
		friend bool		operator>=( const ft::stack<T1,C1>& lhs, const ft::stack<T1,C1>& rhs );


};	// class stack

// Non-member functions
template< class T, class Container >
bool	operator==( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return (lhs.c == rhs.c); }

template< class T, class Container >
bool	operator!=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return !(lhs.c == rhs.c); }

template< class T, class Container >
bool	operator<( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return (lhs.c < rhs.c); }

template< class T, class Container >
bool	operator<=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return !(rhs.c < lhs.c); }

template< class T, class Container >
bool	operator>( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return (rhs.c < lhs.c); }

template< class T, class Container >
bool	operator>=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs )
{ return !(lhs.c < rhs.c); }


};	// namespace ft

#endif
