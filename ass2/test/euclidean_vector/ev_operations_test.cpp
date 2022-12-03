/*
rationale:
Each section represents different operation's tests. Firstly, I tested for functionality by using
all the operations and checking its functionality is correct. After that, I checked that operations
throw exceptions with appropriate messages when neccessary and throw none when the parameters given
do not break the conditions given for an exception to occur.
*/

#include <comp6771/euclidean_vector.hpp>

#include <catch2/catch.hpp>
#include <vector>

// test for exceptions if there are any, if not, then test that operation will not throw any
// exceptions

// not sure how to remove clang for this, complexity (split up the tests further is the only thing I
// can think of, but each function having its own test file is slightly too tedious in my opinion,
// as I think it is better to just section it up and test based on function types, e.g.
// constructors, friends, member functions etc)
TEST_CASE("tests for all operations (functionality and exceptions)") {
	///// COPY ASSIGNMENT TESTS //////
	/*
	testing copy assignment operator overload functionality
	*/
	SECTION("copy assignment tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in = comp6771::euclidean_vector(initialiser_list);
		auto euclidean_vector = euclidean_vector_in;
		// check dimensions are the same as copied assignment (equal to each other)
		CHECK(euclidean_vector.dimensions() == euclidean_vector_in.dimensions());
		// check magnitudes are the same as copied assignment (equal to each other)
		CHECK(euclidean_vector.magnitudes() == euclidean_vector_in.magnitudes());
		// check copy assignment does not throw exception
		CHECK_NOTHROW(euclidean_vector = euclidean_vector_in);
	}

	///// MOVE ASSIGNMENT TESTS //////
	/*
	testing move assignment operator functionality by constructing 2 identical euclidean vectors,
	then move one to variable and check if the untouched constructed euclidean vector is the same
	(test that moved euclidean vector is also removed)
	*/
	SECTION("move assignment tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list);
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list);
		auto euclidean_vector = std::move(euclidean_vector_in_1);
		// check dimensions of moved identical vector are not the same as move assignment (deleted)
		CHECK(euclidean_vector.dimensions() != euclidean_vector_in_1.dimensions());
		// check magnitudes of moved identical vector are not the same as move assignment (deleted)
		CHECK(euclidean_vector.magnitudes() != euclidean_vector_in_1.magnitudes());

		// check dimensions of untouched identical vector are the same as move assignment (moved)
		CHECK(euclidean_vector.dimensions() == euclidean_vector_in_2.dimensions());
		// check magnitudes of untouched identical vector are the same as move assignment (moved)
		CHECK(euclidean_vector.magnitudes() == euclidean_vector_in_2.magnitudes());
		// check copy assignment does not throw exception
		CHECK_NOTHROW(euclidean_vector = std::move(euclidean_vector_in_2));
	}

	///// SUBSCRIPT TESTS //////
	SECTION("subscript tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector = comp6771::euclidean_vector(initialiser_list);
		// first, check if euclidean_vector[1] = 1.1
		CHECK(euclidean_vector[1] == 1.1);
		// then apply subscript and change value of euclidean_vector, then test if it is equal to new
		// value
		euclidean_vector[1] = 192.4;
		CHECK(euclidean_vector[1] == 192.4);
		// no exceptions when subscript operator is called
		CHECK_NOTHROW(euclidean_vector[1] = 902.2);
	}

	///// UNARY PLUS TESTS //////
	/*
	apply unary to a euclidean vector and check if dimensions are magnitude are equal. If they are
	equal, this means that a copy of the old vector has been created, and that the old vector is not
	deleted since it is still equal after the unary plus operation
	*/
	SECTION("unary plus tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in = comp6771::euclidean_vector(initialiser_list);
		// apply unary, then check if it is equal to euclidean_vector_in
		auto euclidean_vector = +euclidean_vector_in;
		// check dimensions are the same
		CHECK(euclidean_vector.dimensions() == euclidean_vector_in.dimensions());
		// check magnitudes are the same
		CHECK(euclidean_vector.magnitudes() == euclidean_vector_in.magnitudes());
		// check exceptions not thrown when unary plus called
		CHECK_NOTHROW(euclidean_vector = +euclidean_vector_in);
	}

	///// NEGATION TESTS //////
	/*
	apply negation to a euclidean vector and check if dimensions are equal and magnitude is negated.
	If these conditions match, this means that a copy of the old vector has been created, and that
	the old vector is not deleted since it still meets the conditions after the unary plus operation
	*/
	SECTION("negation tests") {
		auto initialiser_list = {10.5, 1.1, 0.0, 883.3};
		auto euclidean_vector_in = comp6771::euclidean_vector(initialiser_list);
		// for negation, negate euclidean_vector_in's magnitudes and check if magnitudes are equal
		auto negated_magnitudes = euclidean_vector_in.magnitudes();
		for (auto& magnitude : negated_magnitudes) {
			magnitude *= -1;
		}
		// apply negation, then check if it is equal to euclidean_vector_in
		auto euclidean_vector = -euclidean_vector_in;
		// check dimensions are the same
		CHECK(euclidean_vector.dimensions() == euclidean_vector_in.dimensions());
		// check magnitudes are negated and original vector's magnitudes are not negated
		CHECK(euclidean_vector.magnitudes() != euclidean_vector_in.magnitudes());
		CHECK(euclidean_vector.magnitudes() == negated_magnitudes);
		// check exceptions not thrown when unary plus called
		CHECK_NOTHROW(euclidean_vector = -euclidean_vector_in);
	}

	///// COMPOUND ADDITION TESTS //////
	SECTION("compound addition tests") {
		auto initialiser_list_1 = {1.0, 9.1, 0.0, 4.2};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {10.0, 2.0, 3.0, 1.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		euclidean_vector_in_1 += euclidean_vector_in_2;
		// add 1 and 2, then compare with euclidean_vector_in_3 (sum) and make sure dimensions is the
		// same (4), and every magnitude is equal to sum
		CHECK(euclidean_vector_in_1.dimensions() == 4);
		CHECK(euclidean_vector_in_1.magnitudes()[0] == 11.0);
		CHECK(euclidean_vector_in_1.magnitudes()[1] == 11.1);
		CHECK(euclidean_vector_in_1.magnitudes()[2] == 3.0);
		CHECK(euclidean_vector_in_1.magnitudes()[3] == 5.2);
		// exceptions not thrown when using same dimensions
		CHECK_NOTHROW(euclidean_vector_in_1 += euclidean_vector_in_2);
		// exceptions check using euclidean_vector_in_3 (diff dimensions, 3 to 4)
		CHECK_THROWS_WITH(euclidean_vector_in_1 += euclidean_vector_in_3,
		                  "Dimensions of LHS(4) and RHS(3) do not match");
	}

	///// COMPOUND SUBTRACTION TESTS //////
	SECTION("compound subtraction tests") {
		auto initialiser_list_1 = {10.0, 9.1, 3.0, 4.2};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto initialiser_list_2 = {1.0, 2.0, 1.0, 1.0};
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_2);
		auto initialiser_list_3 = {10.5, 1.1, 0.0};
		auto euclidean_vector_in_3 = comp6771::euclidean_vector(initialiser_list_3);
		euclidean_vector_in_1 -= euclidean_vector_in_2;
		// subtract 1 and 2, then compare with euclidean_vector_in_3 (sum) and make sure dimensions is
		// the same (4), and every magnitude is equal to sum
		CHECK(euclidean_vector_in_1.dimensions() == 4);
		CHECK(euclidean_vector_in_1.magnitudes()[0] == 9.0);
		CHECK(euclidean_vector_in_1.magnitudes()[1] == 7.1);
		CHECK(euclidean_vector_in_1.magnitudes()[2] == 2.0);
		CHECK(euclidean_vector_in_1.magnitudes()[3] == 3.2);
		// exceptions not thrown when using same dimensions
		CHECK_NOTHROW(euclidean_vector_in_1 -= euclidean_vector_in_2);
		// exceptions check using euclidean_vector_in_3 (diff dimensions, 3 to 4)
		CHECK_THROWS_WITH(euclidean_vector_in_1 -= euclidean_vector_in_3,
		                  "Dimensions of LHS(4) and RHS(3) do not match");
	}

	///// COMPOUND MULTIPLICATION TESTS //////
	SECTION("compound multiplcation tests") {
		auto initialiser_list_1 = {10.0, 9.1, 3.0, 4.2};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		euclidean_vector_in_1 *= 4;
		// multiply 1 by 4, then compare with multiplied values and make sure dimensions is
		// the same (4), and every magnitude is equal to multiplied values
		CHECK(euclidean_vector_in_1.dimensions() == 4);
		CHECK(euclidean_vector_in_1.magnitudes()[0] == 40.0);
		CHECK(euclidean_vector_in_1.magnitudes()[1] == 36.4);
		CHECK(euclidean_vector_in_1.magnitudes()[2] == 12.0);
		CHECK(euclidean_vector_in_1.magnitudes()[3] == 16.8);
		// exceptions not thrown when using same dimensions
		CHECK_NOTHROW(euclidean_vector_in_1 *= 4);
	}

	///// COMPOUND DIVISION TESTS //////
	SECTION("compound division tests") {
		auto initialiser_list_1 = {40.0, 36.4, 12.0, 16.8};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		euclidean_vector_in_1 /= 4;
		// divide 1 by 4, then compare with divided values and make sure dimensions is
		// the same (4), and every magnitude is equal to divided values
		CHECK(euclidean_vector_in_1.dimensions() == 4);
		CHECK(euclidean_vector_in_1.magnitudes()[0] == 10.0);
		CHECK(euclidean_vector_in_1.magnitudes()[1] == 9.1);
		CHECK(euclidean_vector_in_1.magnitudes()[2] == 3.0);
		CHECK(euclidean_vector_in_1.magnitudes()[3] == 4.2);
		// exceptions not thrown when using same dimensions
		CHECK_NOTHROW(euclidean_vector_in_1 /= 4);
		CHECK_THROWS_WITH(euclidean_vector_in_1 /= 0, "Invalid vector division by 0");
	}

	///// VECTOR TYPE CONVERSION TESTS //////
	SECTION("vector type conversion tests") {
		// convert to vector using type conversion then compare with vector with same magnitudes
		auto initialiser_list_1 = {40.0, 36.4, 12.0, 16.8};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto converted_vector = static_cast<std::vector<double>>(euclidean_vector_in_1);
		auto vector = std::vector<double>();
		vector.push_back(40.0);
		vector.push_back(36.4);
		vector.push_back(12.0);
		vector.push_back(16.8);
		// should be equal since both are of type std::vector<double>() with exact same magnitudes
		CHECK(converted_vector == vector);
		// check that no assumptions thrown when type converting to vector
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_1);
		CHECK_NOTHROW(static_cast<std::vector<double>>(euclidean_vector_in_2));
	}

	///// LIST TYPE CONVERSION TESTS //////
	SECTION("list type conversion tests") {
		// convert to list using type conversion then compare with list with same magnitudes
		auto initialiser_list_1 = {40.0, 36.4, 12.0, 16.8};
		auto euclidean_vector_in_1 = comp6771::euclidean_vector(initialiser_list_1);
		auto converted_list = static_cast<std::list<double>>(euclidean_vector_in_1);
		auto list = std::list<double>();
		list.push_back(40.0);
		list.push_back(36.4);
		list.push_back(12.0);
		list.push_back(16.8);
		// should be equal since both are of type std::list<double>() with exact same magnitudes
		CHECK(converted_list == list);
		// check that no assumptions thrown when type converting to list
		auto euclidean_vector_in_2 = comp6771::euclidean_vector(initialiser_list_1);
		CHECK_NOTHROW(static_cast<std::list<double>>(euclidean_vector_in_2));
	}
}