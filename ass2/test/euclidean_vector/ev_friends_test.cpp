/*
rationale:
Each section represents different friend function's tests. Firstly, I tested for functionality by
using all the member functions and checking its functionality is correct. After that, I checked that
friends functions throw exceptions with appropriate messages when neccessary and throw none when the
parameters given do not break the conditions given for an exception to occur. However, for the
output stream cout << friend function, I couldn't find a way to automate the tests store the cout
output into a variable without printing it due to the deadline of the assignment. I solved this by
printing out a easy-to-read format comparison when the tests are ran. If the supposed and actual
output matches, then the test counts as it is passed.
*/

#include <comp6771/euclidean_vector.hpp>

#include <catch2/catch.hpp>
#include <iostream>
#include <vector>

// test for exceptions if there are any. if not, then test that friends function will not throw any
// exceptions

// not sure how to remove clang for this, complexity (split up the tests further is the only thing I
// can think of, but each function having its own test file is slightly too tedious in my opinion,
// as I think it is better to just section it up and test based on function types, e.g.
// constructors, friends, member functions etc)
TEST_CASE("tests for all friends (functionality and exceptions)") {
	SECTION("equal friend function tests") {
		auto initialiser_list_1 = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto euclidean_vector_in_4 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.5, 1.1, 0.0, 1.3};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		// diff num of dimensions test (returns false)
		CHECK(!(euclidean_vector_in_1 == euclidean_vector_in_3));
		// diff magnitudes (returns false)
		CHECK(!(euclidean_vector_in_1 == euclidean_vector_in_2));
		// same num of dimensions and same num of magnitudes (returns true)
		CHECK(euclidean_vector_in_1 == euclidean_vector_in_4);
	}

	SECTION("not equal friend function tests") {
		auto initialiser_list_1 = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto euclidean_vector_in_4 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.5, 1.1, 0.0, 1.3};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		// diff num of dimensions test (returns true)
		CHECK(euclidean_vector_in_1 != euclidean_vector_in_3);
		// diff magnitudes (returns true)
		CHECK(euclidean_vector_in_1 != euclidean_vector_in_2);
		// same num of dimensions and same num of magnitudes (returns false)
		CHECK(!(euclidean_vector_in_1 != euclidean_vector_in_4));
	}

	SECTION("addition friend function tests") {
		auto initialiser_list_1 = {10.5, 1.1, 0.0, 883.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.5, 1.1, 0.0, 1.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		auto initialiser_list_4 = {21.0, 2.2, 0.0, 884.0};
		auto euclidean_vector_in_4 = comp6771::euclidean_vector(initialiser_list_4);
		// same num of dimensions (check if euclidean vector 1+2=4, as expected)
		CHECK(euclidean_vector_in_1 + euclidean_vector_in_2 == euclidean_vector_in_4);

		// diff num of dimensions test (throws exception)
		CHECK_THROWS_WITH(euclidean_vector_in_1 + euclidean_vector_in_3,
		                  "Dimensions of LHS(4) and RHS(3) do not match");
	}

	SECTION("subtraction friend function tests") {
		auto initialiser_list_1 = {10.5, 3.3, 0.0, 883.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.5, 1.0, 0.0, 1.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		auto initialiser_list_4 = {0.0, 2.3, 0.0, 882.0};
		auto euclidean_vector_in_4 = comp6771::euclidean_vector(initialiser_list_4);
		// same num of dimensions (check if euclidean vector 1-2=4, as expected)
		CHECK(euclidean_vector_in_1 - euclidean_vector_in_2 == euclidean_vector_in_4);

		// diff num of dimensions test (throws exception)
		CHECK_THROWS_WITH(euclidean_vector_in_1 - euclidean_vector_in_3,
		                  "Dimensions of LHS(4) and RHS(3) do not match");
	}

	// need to check functionality and exceptions for 2 diff types of multiply, since scalar can be
	// either side of vector
	SECTION("multiply friend function tests") {
		auto initialiser_list_1 = {10.0, 4.0, 0.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {30.0, 12.0, 0.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		// check whether multiplying on both sides get correct output
		CHECK(euclidean_vector_in_1 * 3 == euclidean_vector_in_2);
		CHECK(3 * euclidean_vector_in_1 == euclidean_vector_in_2);
		// check no exceptions thrown for multiplying on both sides
		CHECK_NOTHROW(euclidean_vector_in_1 * 3);
		CHECK_NOTHROW(3 * euclidean_vector_in_1);
	}

	SECTION("divide friend function tests") {
		auto initialiser_list_1 = {30.0, 12.0, 0.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.0, 4.0, 0.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		// check whether dividing gets correct output
		CHECK(euclidean_vector_in_1 / 3 == euclidean_vector_in_2);
		// check no exceptions thrown for dividing
		CHECK_NOTHROW(euclidean_vector_in_1 / 3);
		// check exception thrown or dividing by 0
		CHECK_THROWS_WITH(euclidean_vector_in_1 / 0, "Invalid vector division by 0");
	}

	// unsure how to automate tests for output, so just manually check that it is in the correct
	// format
	SECTION("output stream friend function tests") {
		auto initialiser_list_1 = {10.5, 3.3, 0.0, 883.0};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		// check that printed out format is correct
		// if the supposed output and actual output is exactly the same, this "test" has passed
		std::cout << "Check if format for output stream friend function is correct by comparing "
		             "supposed and actual cout format below:\n";
		std::cout << "Output stream friend function supposed output: [10.5 3.3 0 883]\n";
		std::cout << "Output stream friend function actual output:   ";
		std::cout << euclidean_vector_in_1;
		std::cout << "\n";
	}
}