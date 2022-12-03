/*
rationale:
Each section represents different utility function's tests. Firstly, I tested for functionality by
using all the utility functions and checking its functionality is correct. After that, I checked
that utility functions throw exceptions with appropriate messages when neccessary and throw none
when the parameters given do not break the conditions given for an exception to occur.
*/

#include <comp6771/euclidean_vector.hpp>

#include <catch2/catch.hpp>
#include <vector>

// test for exceptions if there are any. if not, then test that member function will not throw any
// exceptions

// not sure how to remove clang for this, complexity (split up the tests further is the only thing I
// can think of, but each function having its own test file is slightly too tedious in my opinion,
// as I think it is better to just section it up and test based on function types, e.g.
// constructors, friends, member functions etc)
TEST_CASE("tests for all utility functions (functionality and exceptions)") {
	SECTION("euclidean_norm utility function") {
		auto initialiser_list = {10.0, 3.0, 2.0};
		auto euclidean_vector = comp6771::euclidean_vector(initialiser_list);
		// check if euclidean_norm function returns correct output normally
		CHECK(comp6771::euclidean_norm(euclidean_vector) == sqrt(100 + 9 + 4));
		// checks recalculation only when euclidean_vector is changed (diff value,
		// recalculated to correct updated norm value)
		euclidean_vector[1] = 2.0;
		CHECK(comp6771::euclidean_norm(euclidean_vector) == sqrt(100 + 4 + 4));
		CHECK_NOTHROW(comp6771::euclidean_norm(euclidean_vector));
	}

	SECTION("unit utility function") {
		auto initialiser_list_1 = {10.0, 3.0, 2.0};
		auto euclidean_vector_1 = comp6771::euclidean_vector(initialiser_list_1);
		// use single-argument constructor to make euclidean vector with dimension 0
		auto euclidean_vector_2 = comp6771::euclidean_vector(0);
		// use default constructor to make euclidean vector with dimension 1, magnitude 0.0
		auto euclidean_vector_3 = comp6771::euclidean_vector();
		auto correct_unit_vector = euclidean_vector_1 /= sqrt(100 + 9 + 4);
		// check if unit function returns correct output normally
		CHECK(comp6771::unit(euclidean_vector_1) == correct_unit_vector);

		// check if exception is thrown when dimensions of given euclidean vector is 0
		CHECK_THROWS_WITH(unit(euclidean_vector_2),
		                  "euclidean_vector with no dimensions does not have a unit vector");

		// check if exception is thrown when given euclidean vector's euclidean norm is 0.0
		// (magnitude 0.0 for dimension 1 euclidean vector, euclidean norm is clearly zero)
		CHECK_THROWS_WITH(unit(euclidean_vector_3),
		                  "euclidean_vector with zero euclidean normal does not have a unit vector");
	}

	SECTION("dot utility function") {
		auto initialiser_list_1 = {10.0, 2.0, 0.0, 80.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {5.0, 1.1, 0.0, 2.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		// check if dot function returns correct output normally
		CHECK(dot(euclidean_vector_in_1, euclidean_vector_in_2) == (50.0 + 2.2 + 0.0 + 160.0));

		// diff num of dimensions test (throws exception)
		CHECK_THROWS_WITH(euclidean_vector_in_1 + euclidean_vector_in_3,
		                  "Dimensions of LHS(4) and RHS(3) do not match");
	}
}