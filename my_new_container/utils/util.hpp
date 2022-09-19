/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 13:19:50 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/19 13:19:55 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_HPP
# define UTIL_HPP

namespace ft {


template< typename Pair >
struct _Select1st
{
	typename Pair::first_type&	operator()( Pair& pair ) const
	{ return (pair.first); }

	const typename Pair::first_type&	operator()( const Pair& pair ) const
	{ return (pair.first); }
};


}	// namespace ft

#endif
