/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 10:45:48 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/19 11:33:39 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Algorithms library:	https://en.cppreference.com/w/cpp/algorithm
// // equal:						https://en.cppreference.com/w/cpp/algorithm/equal
// // lexicographical_compare:		https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare

#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{


template< class InputIt1, class InputIt2 >
bool	equal( InputIt1 first1, InputIt1 last1, InputIt2 first2 )
{
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return (false);
	return (true);
}

template< class InputIt1, class InputIt2, class BinaryPredicate >
bool	equal( InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p )
{
	for (; first1 != last1; ++first1, ++first2)
		if (!(p(*first1, *first2)))
			return (false);
	return (true);
}

template< class InputIt1, class InputIt2 >
bool	lexicographical_compare( InputIt1 first1, InputIt1 last1,
									InputIt2 first2, InputIt2 last2 )
{
	for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
	{
		if (*first1 < *first2)
			return (true);
		if (*first2 < *first1)
			return (false);
	}
	return ((first1 == last1) && (first2 != last2));
}

template< class InputIt1, class InputIt2, class Compare >
bool	lexicographical_compare( InputIt1 first1, InputIt1 last1,
									InputIt2 first2, InputIt2 last2,
									Compare comp )
{
	for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2)
	{
		if (comp(*first1, *first2))
			return (true);
		if (comp(*first2, *first1))
			return (false);
	}
	return ((first1 == last1) && (first2 != last2));
}


}	// namespace ft

#endif
