/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 09:47:46 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/16 00:29:39 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// pair:	https://en.cppreference.com/w/cpp/utility/pair

#ifndef PAIR_HPP
# define PAIR_HPP

namespace	ft
{


template< class T1, class T2 >
struct pair
{
	// Member types
	typedef	T1	first_type;
	typedef	T2	second_type;

	// Member objects
	T1	first;
	T2	second;

	// Member functions
		//constructors and canonical
		pair()
		: first(), second() 
		{

		}

		pair( const T1& x, const T2& y )
		: first(x), second(y)
		{

		}

		template< class U1, class U2 >
		pair( const pair<U1, U2>& p )
		: first(p.first), second(p.second)
		{

		}
		
		pair&	operator=( const pair& other )
		{
			if (this == &other)
				return (*this);
			first = other.first;
			second = other.second;
			return (*this);
		}
};	// struct pair

// Non-member functions
template< class T1, class T2 >
ft::pair<T1,T2>		make_pair( T1 t, T2 u )
{ return (pair<T1, T2>(t, u)); }

template< class T1, class T2 >
bool	operator==( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return (lhs.first == rhs.first && lhs.second == rhs.second); }

template< class T1, class T2 >
bool	operator!=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return !(lhs == rhs); }

template< class T1, class T2 >
bool	operator<( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return (lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second)); }

template< class T1, class T2 >
bool	operator<=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return !(rhs < lhs); }

template< class T1, class T2 >
bool	operator>( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return (rhs < lhs); }

template< class T1, class T2 >
bool	operator>=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs )
{ return !(lhs < rhs); }


};	// namespace ft

#endif
