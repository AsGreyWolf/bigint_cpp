#include "bigint.hpp"
#include <chrono>
#include <functional>
#include <iostream>

int main() {
	using namespace std;
	using namespace chrono;
	cout.tie(0);
	ios_base::sync_with_stdio(false);
	auto rand = bind(uniform_int_distribution<bigint::value_type>{0, bigint::base},
	                 mt19937(random_device{}()));
	bigint::container_type data1(100000);
	generate(data1.begin(), data1.end(), [&]() { return rand(); });
	bigint::container_type data2(100000);
	generate(data2.begin(), data2.end(), [&]() { return rand(); });
	using clock = high_resolution_clock;
	auto start = clock::now();
	auto b = (bigint{data1} * bigint{data2});
	cout << duration_cast<milliseconds>(clock::now() - start).count() << "ms"
	     << endl;
	auto a = 123456789098765432345678765476543234567892455676543_BI;
	cout << a << endl;
	cout << a * a << endl;
	cout
	    << (a * a ==
	        15241578774577047292943087448865014449380375909239085045885443197750618525894759534458355823840430849_BI)
	    << endl;
}
