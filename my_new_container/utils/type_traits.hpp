/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 09:48:03 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/19 11:32:34 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// enable_if:			https://en.cppreference.com/w/cpp/types/enable_if
// integral_constant:	https://en.cppreference.com/w/cpp/types/integral_constant
// is_integral:			https://en.cppreference.com/w/cpp/types/is_integral

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

namespace ft
{


// enable_if
	template< bool B, class T = void >
	struct	enable_if {};

	template< class T >
	struct	enable_if< true, T >
	{ typedef T type; };

// integral_constant
	template< class T, T v >
	struct	integral_constant
	{
		static const T				value = v;
		typedef T					value_type;
		typedef integral_constant	type;
		const value_type	operator()() const
		{ return (value); }
	};

	typedef integral_constant<bool, true>	true_type;
	typedef integral_constant<bool, false>	false_type;

// remove cv
	template< class T >
	struct	remove_cv
	{ typedef T type; };

	template< class T >
	struct	remove_cv< const T >
	{ typedef T type; };

	template< class T >
	struct	remove_cv< volatile T >
	{ typedef T type; };

	template< class T >
	struct	remove_cv< const volatile T >
	{ typedef T type; };

// is_integral
	template< class T >
	struct	is_integral_helper : public false_type {};

	template<>
	struct	is_integral_helper< bool > : public true_type {};
	template<>
	struct	is_integral_helper< char > : public true_type {};
	template<>
	struct	is_integral_helper< signed char > : public true_type {};
	template<>
	struct	is_integral_helper< unsigned char > : public true_type {};
	template<>
	struct	is_integral_helper< wchar_t > : public true_type {};
	template<>
	struct	is_integral_helper< short > : public true_type {};
	template<>
	struct	is_integral_helper< unsigned short > : public true_type {};
	template<>
	struct	is_integral_helper< int > : public true_type {};
	template<>
	struct	is_integral_helper< unsigned int > : public true_type {};
	template<>
	struct	is_integral_helper< long > : public true_type {};
	template<>
	struct	is_integral_helper< unsigned long > : public true_type {};
	template<>
	struct	is_integral_helper< long long > : public true_type {};
	template<>
	struct	is_integral_helper< unsigned long long > : public true_type {};

	template< class T >
	struct	is_integral : public is_integral_helper<typename remove_cv<T>::type> {};


}	// namespace ft

#endif
