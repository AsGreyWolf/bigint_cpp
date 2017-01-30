#pragma once

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iterator>

template <typename ElementType> constexpr size_t prefered_base() {
	return (std::numeric_limits<ElementType>::max() >> (sizeof(ElementType) * 4)) +
	       1;
}

template <typename ElementType = int_fast64_t,
          size_t Base = prefered_base<ElementType>(),
          typename ContainerType = std::vector<ElementType>,
          typename = typename std::enable_if<
              std::numeric_limits<ElementType>::is_integer &&
              std::numeric_limits<ElementType>::is_signed>::type>
class bigint_base : ContainerType {
public:
	using container_type = ContainerType;
	static constexpr auto const &base = Base;

	using typename container_type::value_type;
	using container_type::size;
	using container_type::empty;
	using container_type::cbegin;
	using container_type::cend;
	using container_type::crbegin;
	using container_type::crend;

private:
	using container_type::back;
	using container_type::pop_back;
	using container_type::push_back;
	void trim() {
		while (!empty() && back() == 0)
			pop_back();
	}
	void normalize() {
		size_t n = size();
		push_back(0);
		for (size_t i = 0; i < n; i++) {
			if ((*this)[i] < 0) {
				value_type c = -(*this)[i] / base + 1;
				(*this)[i + 1] -= c;
				(*this)[i] += c * base;
			}
			if ((size_t)(*this)[i] >= base) {
				(*this)[i + 1] += (*this)[i] / base;
				(*this)[i] %= base;
			}
		}
		trim();
	}
	template <typename T,
	          typename = typename std::enable_if<std::is_same<
	              value_type,
	              typename std::iterator_traits<T>::value_type>::value>::type>
	bigint_base(T begin, T end) : container_type(begin, end) {}
	bigint_base(size_t n, value_type value) : container_type(n, value) {}

public:
	explicit bigint_base(const container_type &data)
	    : bigint_base(data.rbegin(), data.rend()) {
		normalize();
	}
	explicit bigint_base(value_type data) {
		while (data) {
			push_back(data % base);
			data /= base;
		}
	}

	template <typename T, size_t N>
	friend bigint_base<T, N> operator+(const bigint_base<T, N> &a,
	                                   const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator-(const bigint_base<T, N> &a,
	                                   const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bigint_base<T, N>
	operator<<(const bigint_base<T, N> &a,
	           typename bigint_base<T, N>::iterator::difference_type diff);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator*(const bigint_base<T, N> &a,
	                                   typename bigint_base<T, N>::value_type b);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator*(const bigint_base<T, N> &a,
	                                   const bigint_base<T, N> &b);

	template <typename T, size_t N>
	friend bigint_base<T, N> &operator+=(bigint_base<T, N> &a,
	                                     const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bigint_base<T, N> &operator-=(bigint_base<T, N> &a,
	                                     const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bigint_base<T, N> &
	operator<<=(bigint_base<T, N> &a,
	            typename bigint_base<T, N>::iterator::difference_type diff);
	template <typename T, size_t N>
	friend bigint_base<T, N> &operator*=(bigint_base<T, N> &a,
	                                     typename bigint_base<T, N>::value_type b);
	template <typename T, size_t N>
	friend bigint_base<T, N> &operator*=(bigint_base<T, N> &a,
	                                     const bigint_base<T, N> &b);

	template <typename T, size_t N>
	friend bigint_base<T, N> &operator++(bigint_base<T, N> &a);
	template <typename T, size_t N>
	friend bigint_base<T, N> &operator--(bigint_base<T, N> &a);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator++(bigint_base<T, N> &a, int);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator--(bigint_base<T, N> &a, int);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator+(const bigint_base<T, N> &a);
	template <typename T, size_t N>
	friend bigint_base<T, N> operator-(const bigint_base<T, N> &a);

	template <typename T, size_t N>
	friend bool operator<(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bool operator>(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bool operator==(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bool operator!=(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bool operator<=(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
	template <typename T, size_t N>
	friend bool operator>=(const bigint_base<T, N> &a, const bigint_base<T, N> &b);
};

#include "bigint_base.inl"
