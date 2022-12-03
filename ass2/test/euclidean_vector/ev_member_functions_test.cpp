/*
rationale:
Each section represents different member function's tests. Firstly, I tested for functionality by
using all the member functions and checking its functionality is correct. After that, I checked that
member functions throw exceptions with appropriate messages when neccessary and throw none when the
parameters given do not break the conditions given for an exception to occur.
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
TEST_CASE("tests for all member functions (functionality and exceptions)") {
	/*
	set variable double magnitude to .at, then change the value of magnitude and check the original
	euclidean vector's value at the chosen dimension. If the magnitude of the original euclidean
	vector remains unchanged, then we have ensured that a reference is not returned, and a value is
	returned.
	*/
	SECTION("return magnitude of passed in dimension member function (at) tests (value returned)") {
		auto initialiser_list_1 = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		// double variable specified (value)
		double magnitude = euclidean_vector_in_1.at(3);
		// check magnitude is correct
		CHECK(magnitude == 883.3);
		// change magnitude value
		magnitude = 1.1;
		// check original euclidean vector such that it remains unchanged
		CHECK(euclidean_vector_in_1.at(3) == 883.3);

		// check exceptions (if in range of dimensions then it does not throw) (check edges of range)
		CHECK_NOTHROW(euclidean_vector_in_1.at(0));
		CHECK_NOTHROW(euclidean_vector_in_1.at(3));
		CHECK_THROWS_WITH(euclidean_vector_in_1.at(-1),
		                  "Index -1 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(euclidean_vector_in_1.at(4),
		                  "Index 4 is not valid for this euclidean_vector object");
	}

	/*
	set variable double& magnitude to .at, then change the value of magnitude and check the original
	euclidean vector's value at the chosen dimension. If the magnitude of the original euclidean
	vector is changed, then we have ensured that a reference is returned, and a value is not
	returned.
	*/
	SECTION("return magnitude of passed in dimension member function (at) tests (ref returned)") {
		auto initialiser_list_1 = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		// double& variable specified (reference)
		double& magnitude = euclidean_vector_in_1.at(3);
		// check magnitude is correct
		CHECK(magnitude == 883.3);
		// change magnitude value
		magnitude = 1.1;
		// check original euclidean vector such that it has changed to 1.1 (set only in magnitude)
		CHECK(euclidean_vector_in_1.at(3) == 1.1);

		// check exceptions (if in range of dimensions then it does not throw) (check edges of range)
		CHECK_NOTHROW(euclidean_vector_in_1.at(0));
		CHECK_NOTHROW(euclidean_vector_in_1.at(3));
		CHECK_THROWS_WITH(euclidean_vector_in_1.at(-1),
		                  "Index -1 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(euclidean_vector_in_1.at(4),
		                  "Index 4 is not valid for this euclidean_vector object");
	}

	SECTION("return number of dimensions in given euclidean vector (dimensions) tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list);
		// check if dimensions function returns proper number of dimensions
		CHECK(euclidean_vector_in_1.dimensions() == 4);
		// check that function does not throw exception
		CHECK_NOTHROW(euclidean_vector_in_1.dimensions());
	}
}