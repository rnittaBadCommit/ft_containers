/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnitta <rnitta@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 11:01:22 by rnitta            #+#    #+#             */
/*   Updated: 2022/09/16 11:17:12 by rnitta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//reference https://en.cppreference.com/w/cpp/container/vector

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <iostream>
# include <limits>

# include "../utils/algorithm.hpp"
# include "../utils/iterator.hpp"
# include "../utils/random_access_iterator.hpp"
# include "../utils/util.hpp"

namespace ft {


template< typename T, typename Allocator = std::allocator<T> >
class	vector
{
	public:
		// Member types
			typedef	T				value_type;
			typedef Allocator		allocator_type;
			typedef std::size_t		size_type;
			typedef std::ptrdiff_t	difference_type;

			typedef value_type&							reference;
			typedef const value_type&					const_reference;
			typedef typename Allocator::pointer			pointer;
			typedef typename Allocator::const_pointer	const_pointer;

			typedef typename ft::random_access_iterator<value_type>			iterator;
			typedef typename ft::random_access_iterator<const value_type>	const_iterator;
			typedef ft::reverse_iterator<iterator>							reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

		// Member functions
			// constructors and canonical
				vector()
				: first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(allocator_type())
				{

				}

				explicit	vector( const Allocator& alloc )
				: first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc)
				{

				}

				explicit	vector( size_type count,
									const value_type& value = value_type(),
									const Allocator& alloc = Allocator() )
				: first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc)
				{
					resize(count, value);
				}

				template< class InputIt >
				vector( InputIt first, InputIt last,
						const Allocator& alloc = Allocator(),
						typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type* = NULL )
				: first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(alloc)	// *check* first_, last_, reserved_last_ を代入するかどうか
				{
					first_ = allocate(ft::distance(first, last));
					last_ = first_;
					for (InputIt i = first; i != last; ++i)
					{
						construct(last_, *i);
						++last_;
					}
					reserved_last_ = last_;
				}

				vector( const vector& other )
				: first_(NULL), last_(NULL), reserved_last_(NULL), alloc_(other.alloc_)
				{
					*this = other;
				}

				virtual		~vector()
				{
					clear();
					deallocate();
				}

				vector&		operator=( const vector& other )
				{
					if (this == &other)
						return (*this);
					assign(other.first_, other.last_);
					return (*this);
				}

				void	assign( size_type count, const value_type& value )
				{
					if (count > capacity())
					{
						clear();
						deallocate();
						first_ = allocate(count);
						reserved_last_ = first_ + count;
						last_ = first_;
						for (size_type i = 0; i < count; ++i, ++last_)
							construct(last_, value);
					}
					else
					{
						pointer		ptr = first_;
						size_type	i = 0;
						size_type	_size = size();

						for (; i < _size; ++i, ++ptr)
							*ptr = value;
						if (count > _size)
							for (; i < count; ++i, ++last_)
								construct(last_, value);
						else
							destroy_until(first_ + count);
					}
				}

				template< class InputIt >
				void	assign( InputIt first, InputIt last,
								typename ft::enable_if<!ft::is_integral<InputIt>::value,
														InputIt>::type* = NULL	)
				{
					size_type	count = ft::distance(first, last);

					if (count > capacity())
					{
						clear();
						deallocate();
						first_ = allocate(count);
						reserved_last_ = first_ + count;
						last_ = first_;
						for (InputIt itr = first; itr != last; ++itr, ++last_)
							construct(last_, *itr);
					}
					else
					{
						pointer		dest = first_;
						size_type	_size = size();
						size_type	i = 0;

						for (; i < _size; ++i, ++dest, ++first)
							*dest = *first;
						if (count > _size)
							for (; i < count; ++i, ++last_, ++first)
								construct(last_, *first);
						else
							destroy_until(first_ + count);
					}
				}

				allocator_type	get_allocator() const
				{ return allocator_type(alloc_); }

			// Element access
				reference	at( size_type pos )
				{
					if (pos >= size())
						throw std::out_of_range("vector::at: out_of_range: pos >= this->size()");
					return (first_[pos]);
				}

				const_reference		at( size_type pos ) const
				{
					if (pos >= size())
						throw std::out_of_range("vector::at: out_of_range: pos >= this->size()");
					return (first_[pos]);
				}

				reference	operator[]( size_type pos )
				{ return (first_[pos]); }

				const_reference		operator[]( size_type pos ) const
				{ return (first_[pos]); }

				reference	front()
				{ return (*first_); }

				const_reference		front() const
				{ return (*first_); }

				reference	back()
				{ return (*(last_ - 1)); }

				const_reference		back() const
				{ return (*(last_ - 1)); }

				value_type*		data()
				{ return (first_); }

				const value_type*	data() const
				{ return (first_); }

			// Iterators
				iterator	begin()
				{ return (first_); }

				const_iterator	begin() const
				{ return (first_); }

				iterator	end()
				{ return (last_); }

				const_iterator	end() const
				{ return (last_); }

				reverse_iterator	rbegin()
				{ return (reverse_iterator(last_)); }

				const_reverse_iterator	rbegin() const
				{ return (reverse_iterator(last_)); }

				reverse_iterator	rend()
				{ return (reverse_iterator(first_)); }

				const_reverse_iterator	rend() const
				{ return (reverse_iterator(first_)); }

			// Capacity
				bool	empty() const
				{ return (first_ == last_); }

				size_type	size() const
				{ return size_type(last_ - first_); }

				size_type	max_size() const
				{
					return std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max() / sizeof(value_type)),
									alloc_.max_size());
				}

				void	reserve( size_type new_cap )
				{
					if (new_cap > capacity())
					{
						size_type old_size = size();
						pointer new_first = allocate(new_cap);

						construct_and_copy(first_, last_, new_first);
						clear();
						deallocate();
						set_pointers_(new_first, new_first + old_size, new_first + new_cap);
					}
				}

				size_type	capacity() const
				{ return size_type(reserved_last_ - first_); }


			// Modifiers
				void	clear()
				{ destroy_until(first_); }

				iterator	insert( iterator pos, const T& value )
				{
					difference_type	offset = pos - begin();
					insert(pos, 1, value);
					return (begin() + offset);
				}

				void	insert( iterator pos, size_type count, const T& value )
				{
					if (count < 0)
						throw std::length_error("vector::insert: length_error: count < 0");
					size_type	new_size = size() + count;

					if (new_size <= capacity())	// enough capacity
					{
						iterator	src = last_, dest = last_ + count;

						while (src != pos && dest != end())
							construct(&*(--dest), *--src);
						if (src != pos)
						{	// count < data(to move) num
							while (src != pos)
								*--dest = *--src;
							while (dest != pos)
								*--dest = value;
						}
						else
						{
							while (dest != end())
								construct(&*(--dest), value);
							while (dest != pos)
								*--dest = value;
						}
						last_ += count;
					}
					else	// less capacity
					{
						new_size = calc_new_capacity(new_size);
						pointer new_first = allocate(new_size);
						pointer new_last = new_first;

						iterator	src = first_;
						for (; src != pos; ++src, ++new_last)
							construct(new_last, *src);
						for (size_type i = 0; i < count; ++i, ++new_last)
							construct(new_last, value);
						for (; src != end(); ++src, ++new_last)
							construct(new_last, *src);
						clear();
						deallocate();
						set_pointers_(new_first, new_last, new_first + new_size);
					}
				}

				template< class InputIt >
				void	insert( iterator pos, InputIt it_first, InputIt it_last,
								typename ft::enable_if<!ft::is_integral<InputIt>::value,
														InputIt>::type* = NULL)
				{
					size_type		count = ft::distance(it_first, it_last);
					size_type		new_size = size() + count;

					if (new_size <= capacity())	// enough capacity
					{
						iterator	src = last_, dest = last_ + count;

						while (src != pos && dest != end())
							construct(&*(--dest), *--src);
						if (src != pos)
						{	// count < data(to move) num
							while (src != pos)
								*--dest = *--src;
							while (dest != pos)
								*--dest = *--it_last;
						}
						else
						{
							while (dest != end())
								construct(&*(--dest), *--it_last);
							while (dest != pos)
								*--dest = *--it_last;
						}
						last_ += count;
					}
					else	// less capacity
					{
						new_size = calc_new_capacity(new_size);
						pointer	new_first = allocate(new_size);
						pointer	new_last = new_first;

						iterator	src = first_;
						for (; src != pos; ++src, ++new_last)
							construct(new_last, *src);
						for (size_type i = 0; i < count; ++i, ++new_last, ++it_first)
							construct(new_last, *it_first);
						for (; src != end(); ++src, ++new_last)
							construct(new_last, *src);
						clear();
						deallocate();
						set_pointers_(new_first, new_last, new_first + new_size);
					}
				}

				iterator	erase( iterator pos )
				{ return (erase(pos, pos + 1)); }

				iterator	erase( iterator first, iterator last )
				{
					if (first_ == last_)
						return (NULL);
					difference_type	head_offset = ft::distance(begin(), first);

					for (iterator head = first, tail = last; tail < end(); ++head, ++tail)
						*head = *tail;
					destroy_until(last_ - ft::distance(first, last));
					return (first_ + head_offset);
				}


				void	push_back( const_reference value )
				{
					if (size() >= capacity())	// less capacity
						reserve(calc_new_capacity(1));
					construct(last_, value);
					++last_;
				}

				void	pop_back()
				{ destroy(--last_); }

				void	resize( size_type count, T value = T() )
				{
					size_type	_size = size();

					if (count < _size)
					{
						last_ = first_ + count;
						destroy_until(last_);
					}
					else if (count > _size)
					{
						difference_type diff = count - _size;
						reserve(count);
						for (difference_type i = 0; i < diff; i++, ++last_)
							construct(last_, value);
					}
				}

				void	swap( vector& other )
				{
					pointer			tmp_first = first_;
					pointer			tmp_last = last_;
					pointer			tmp_reserved_last_ = reserved_last_;
					allocator_type	tmp_alloc = alloc_;

					first_ = other.first_;
					last_ = other.last_;
					reserved_last_ = other.reserved_last_;
					alloc_ = other.alloc_;

					other.first_ = tmp_first;
					other.last_ = tmp_last;
					other.reserved_last_ = tmp_reserved_last_;
					other.alloc_ = tmp_alloc;
				}


	protected:
		pointer			first_;
		pointer			last_;
		pointer			reserved_last_;
		allocator_type	alloc_;


	private:
 			void	allocate_and_set_pointers(size_type size)
			{
				pointer		new_first = allocate(size);

				set_pointers_(new_first, new_first + size, new_first + size);
			}

			void	set_pointers_(pointer first, pointer last, pointer reserved_last)
			{ first_ = first; last_ = last; reserved_last_ = reserved_last; }

			// only when (_require_cap > current_cap)
			size_type	calc_new_capacity(size_type _require_cap)
			{
				size_type	_current_cap = capacity();
				size_type	_max_size = max_size();

				if (_require_cap > _max_size || _current_cap == _max_size)
					throw std::length_error("vector::allocate: length_error: new_cap > max_size()");

				if (_require_cap == 0)
					_require_cap = 1;
				if (_current_cap > _require_cap / 2)
				{
					if (_current_cap * 2 <= _max_size)
						return (_current_cap * 2);
					else
						return (_max_size);
				}
				else
					return (_require_cap);
			}

			pointer		allocate(size_type n) {
				if (n > max_size())
					throw std::length_error("vector::allocate: length_error: new_cap > max_size()");
				return alloc_.allocate(n);
			}

			void	deallocate()
			{ alloc_.deallocate(first_, capacity()); }

			void	construct(pointer ptr)
			{ alloc_.construct(ptr, 0); }

			void	construct(pointer ptr, const_reference value)
			{ alloc_.construct(ptr, value); }

			void	destroy(pointer ptr)
			{ alloc_.destroy(ptr); }

			void	destroy_until( pointer new_last )
			{
				while (last_ != new_last)
				 	alloc_.destroy(--last_);
			}

			template<typename InputIt, typename ForwardIt>
			void	construct_and_copy(InputIt src_first, InputIt src_last, ForwardIt dest)
			{
				while (src_first != src_last)
				{
					construct(dest, *src_first);
					++dest; ++src_first;
				}
			}

			template<typename InputIt, typename Size_type, typename ForwardIt>
			void	construct_and_copy(InputIt src_first, typename ft::enable_if<ft::is_integral<Size_type>::valule, Size_type>::type src_size, ForwardIt dest)
			{
				while (src_size-- != 0)
				{
					construct(dest, *src_first);
					++dest; ++src_first;
				}
			}

};	// class vector


// Non-member functions

// Non-member functions
template< class T, class Alloc >
bool	operator==( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return (lhs.size() == rhs.size()
			&& ft::equal(lhs.begin(), lhs.end(), rhs.begin())); }

template< class T, class Alloc >
bool	operator!=( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return !(lhs == rhs); }

template< class T, class Alloc >
bool	operator<( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
										rhs.begin(), rhs.end())); }

template< class T, class Alloc >
bool	operator<=( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return !(rhs < lhs); }

template< class T, class Alloc >
bool	operator>( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return (rhs < lhs); }

template< class T, class Alloc >
bool	operator>=( const ft::vector<T,Alloc>& lhs,
					const ft::vector<T,Alloc>& rhs )
{ return !(lhs < rhs); }


template< class T, class Alloc >
void	swap( ft::vector<T,Alloc>& lhs,
        		ft::vector<T,Alloc>& rhs )
{ lhs.swap(rhs); }


};  // namespace ft


#endif