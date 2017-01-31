#pragma once

#include "bigint_base.hpp"

using bigint = bigint_base<int_fast64_t, 1000000000>;
inline bigint operator"" _BI(const char *data) {
	size_t n = strlen(data);
	bigint::container_type result;
	size_t i = 0;
	if (n % 9 != 0)
		result.push_back(std::stoll(std::string(data, data + n % 9)));
	i += n % 9;
	for (; i < n; i += 9) {
		result.push_back(std::stoll(std::string(data + i, data + i + 9)));
	}
	return bigint{result};
}
inline std::ostream &operator<<(std::ostream &out, const bigint &data) {
	if (data.size() == 0)
		out << 0;
	for (auto i = data.crbegin(); i != data.crend(); i++) {
		std::ios init(NULL);
		init.copyfmt(out);
		if (i != data.crbegin())
			out << std::setw(9) << std::setfill('0');
		out << *i;
		out.copyfmt(init);
	}
	return out;
}
