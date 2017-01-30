#pragma once

template <typename T, size_t N>
inline bigint_base<T, N> operator+(const bigint_base<T, N> &a,
                                   const bigint_base<T, N> &b) {
	auto result = a;
	return result += b;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator-(const bigint_base<T, N> &a,
                                   const bigint_base<T, N> &b) {
	auto result = a;
	return result -= b;
}
template <typename T, size_t N>
inline bigint_base<T, N>
operator<<(const bigint_base<T, N> &a,
           typename bigint_base<T, N>::iterator::difference_type diff) {
	auto result = a;
	return result <<= diff;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator*(const bigint_base<T, N> &a,
                                   typename bigint_base<T, N>::value_type b) {
	auto result = a;
	return result *= b;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator*(const bigint_base<T, N> &a,
                                   const bigint_base<T, N> &b) {
	const auto n = std::max(a.size(), b.size());
	if (n <= 256) {
		bigint_base<T, N> result(a.size() + b.size(), 0);
		for (size_t i = 0; i < a.size(); ++i) {
			for (size_t j = 0; j < b.size(); ++j) {
				result[i + j] += a[i] * b[j];
			}
		}
		result.normalize();
		return result;
	} else {
		auto mid = min(a.begin() + n / 2, a.end());
		bigint_base<T, N> a_l(a.begin(), mid);
		bigint_base<T, N> a_r(mid, a.end());
		mid = min(b.begin() + n / 2, b.end());
		bigint_base<T, N> b_l(b.begin(), mid);
		bigint_base<T, N> b_r(mid, b.end());
		auto prod1 = a_r * b_r;
		auto prod2 = a_l * b_l;
		auto prod3 = (a_l + a_r) * (b_l + b_r);
		return (prod1 << n / 2 * 2) + ((prod3 - prod1 - prod2) << (n / 2)) + prod2;
	}
}

template <typename T, size_t N>
inline bigint_base<T, N> &operator+=(bigint_base<T, N> &a,
                                     const bigint_base<T, N> &b) {
	a.resize(std::max(a.size(), b.size()), 0);
	for (size_t i = 0; i < b.size(); i++)
		a[i] += b[i];
	a.normalize();
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> &operator-=(bigint_base<T, N> &a,
                                     const bigint_base<T, N> &b) {
	a.resize(std::max(a.size(), b.size()), 0);
	for (size_t i = 0; i < b.size(); i++)
		a[i] -= b[i];
	a.normalize();
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> &
operator<<=(bigint_base<T, N> &a,
            typename bigint_base<T, N>::iterator::difference_type diff) {
	reverse(a.begin(), a.end());
	a.resize(diff + a.size(), 0);
	reverse(a.begin(), a.end());
	a.normalize();
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> &operator*=(bigint_base<T, N> &a,
                                     typename bigint_base<T, N>::value_type b) {
	for (size_t i = 0; i < a.size(); i++)
		a[i] *= b;
	a.normalize();
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> &operator*=(bigint_base<T, N> &a,
                                     const bigint_base<T, N> &b) {
	a = a * b;
	return a;
}

template <typename T, size_t N>
inline bigint_base<T, N> &operator++(bigint_base<T, N> &a) {
	a += bigint_base<T, N>{1};
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> &operator--(bigint_base<T, N> &a) {
	a -= bigint_base<T, N>{1};
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator++(bigint_base<T, N> &a, int) {
	auto result = a;
	++a;
	return result;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator--(bigint_base<T, N> &a, int) {
	auto result = a;
	--a;
	return result;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator+(const bigint_base<T, N> &a) {
	return a;
}
template <typename T, size_t N>
inline bigint_base<T, N> operator-(const bigint_base<T, N> &a) {
	return bigint_base<T, N>{0} - a;
}

template <typename T, size_t N>
inline bool operator<(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	if (a.size() > b.size())
		return a.back() < 0;
	if (a.size() < b.size())
		return b.back() > 0;
	for (auto ai = a.crbegin(), bi = b.crbegin();
	     ai != a.crend() && bi != b.crend(); ai++, bi++) {
		if (*ai != *bi)
			return *ai < *bi;
	}
	return false;
}
template <typename T, size_t N>
inline bool operator>(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	if (a.size() > b.size())
		return a.back() > 0;
	if (a.size() < b.size())
		return b.back() < 0;
	for (auto ai = a.crbegin(), bi = b.crbegin();
	     ai != a.crend() && bi != b.crend(); ai++, bi++) {
		if (*ai != *bi)
			return *ai > *bi;
	}
	return false;
}
template <typename T, size_t N>
inline bool operator==(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	if (a.size() != b.size())
		return false;
	for (auto ai = a.crbegin(), bi = b.crbegin();
	     ai != a.crend() && bi != b.crend(); ai++, bi++) {
		if (*ai != *bi)
			return false;
	}
	return true;
}
template <typename T, size_t N>
inline bool operator!=(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	return !(a == b);
}
template <typename T, size_t N>
inline bool operator<=(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	return a < b || a == b;
}
template <typename T, size_t N>
inline bool operator>=(const bigint_base<T, N> &a, const bigint_base<T, N> &b) {
	return a > b || a == b;
}

template <typename T, size_t N>
inline std::ostream &operator<<(std::ostream &out,
                                const bigint_base<T, N> &data) {
	if (data.size() == 0)
		out << 0;
	using type = bigint_base<T, N>;
	std::copy(data.crbegin(), data.crend(),
	          std::ostream_iterator<typename type::value_type>(out, " "));
	out << " (%" << type::base << ')';
	return out;
}
