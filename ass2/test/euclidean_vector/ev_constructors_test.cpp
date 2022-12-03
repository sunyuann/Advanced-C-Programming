/*
rationale:
Each section represents different constructor's tests. Firstly, I tested for functionality by using
all the constructors and checking its functionality is correct. After that, I check that calling
each type of constructor will not cause any exception since all constructors should not throw
exceptions (except the fact that if dimensions passed in are negative). However, since the
specification said that we can assume that dimensions passed in are always positive integers, we can
safely assume that no exceptions will be thrown when an constructor is called.
*/

// assume int input always be non negative for all dimension (constructors) (no need to test)

#include <comp6771/euclidean_vector.hpp>

#include <catch2/catch.hpp>

// for all constructors, test that no exceptions will be thrown

// not sure how to remove clang for this, complexity (split up the tests further is the only thing I
// can think of, but each function having its own test file is slightly too tedious in my opinion,
// as I think it is better to just section it up and test based on function types, e.g.
// constructors, friends, member functions etc)
TEST_CASE("tests for all constructors (functionality and exceptions)") {
	///// DEFAULT CONSTRUCTOR TESTS //////
	/*
	testing default constructor generates euclidean vector with dimension of 1 and magnitude of 0.0
	*/
	SECTION("default constructor tests") {
		auto constructor = comp6771::euclidean_vector();
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(0.0);
		// check dimensions of default vector is 1
		CHECK(constructor.dimensions() == 1);
		// check magnitudes of default vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector());
	}

	///// SINGLE-ARGUMENT CONSTRUCTOR TESTS //////
	/*
	testing single-argument constructor generates euclidean vectors with passed in num of dimensions
	where each dimension's magnitude is 0.0
	*/
	SECTION("single-argument constructor tests") {
		auto constructor = comp6771::euclidean_vector(4);
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(0.0);
		magnitudes.push_back(0.0);
		magnitudes.push_back(0.0);
		magnitudes.push_back(0.0);
		// check dimensions of vector is 4
		CHECK(constructor.dimensions() == 4);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector(4));
	}

	///// DIMENSION AND MAGNITUDE CONSTRUCTOR TESTS //////
	/*
	testing iterator constructor generates euclidean vectors with passed in dimensions where each
	dimension's magnitude corresponds to beginning and end of passed in iterator
	*/
	SECTION("dimension and magnitude constructor tests") {
		auto constructor = comp6771::euclidean_vector(3, 4.5);
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(4.5);
		magnitudes.push_back(4.5);
		magnitudes.push_back(4.5);
		// check dimensions of vector is 3
		CHECK(constructor.dimensions() == 3);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector(3, 4.5));
	}

	///// START AND END OF ITERATOR CONSTRUCTOR TESTS //////
	/*
	testing iterator constructor generates euclidean vectors with passed in dimensions where each
	dimension's magnitude corresponds to beginning and end of passed in iterator
	*/
	SECTION("start and end of iterator constructor tests") {
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(9301.5);
		magnitudes.push_back(53.6);
		magnitudes.push_back(0.0);
		magnitudes.push_back(4.5);
		magnitudes.push_back(6.7);
		auto constructor = comp6771::euclidean_vector(magnitudes.begin(), magnitudes.end());
		// check dimensions of vector is 5
		CHECK(constructor.dimensions() == 5);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector(magnitudes.begin(), magnitudes.end()));
	}

	///// INITIALISER LIST CONSTRUCTOR TESTS //////
	/*
	 testing iterator constructor generates euclidean vectors with passed in dimensions where
	 dimension's magnitude corresponds to entire initialiser list passed in
	*/
	SECTION("initialiser list constructor tests") {
		auto initialiser_list = {101.4, 1.6, 45.3, 1111.5};
		auto constructor = comp6771::euclidean_vector(initialiser_list);
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(101.4);
		magnitudes.push_back(1.6);
		magnitudes.push_back(45.3);
		magnitudes.push_back(1111.5);
		// check dimensions of vector is 4
		CHECK(constructor.dimensions() == 4);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector(initialiser_list));
	}

	///// COPY CONSTRUCTOR TESTS //////
	/*
	testing copy constructor makes exact copy of euclidean vector from euclidean vector passed in
	(same dimensions, same magnitudes)
	*/
	SECTION("copy constructor tests") {
		// normal constructor (using init list constructor)
		auto initialiser_list = {101.4, 1.6, 45.3, 1111.5};
		auto init_list_constructor = comp6771::euclidean_vector(initialiser_list);
		// copy constructor
		auto constructor = comp6771::euclidean_vector(init_list_constructor);
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(101.4);
		magnitudes.push_back(1.6);
		magnitudes.push_back(45.3);
		magnitudes.push_back(1111.5);
		// check dimensions of vector is 4
		CHECK(constructor.dimensions() == 4);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown
		CHECK_NOTHROW(comp6771::euclidean_vector(init_list_constructor));
	}

	///// MOVE CONSTRUCTOR TESTS //////
	/*
	testing move constructor makes exact copy of euclidean vector from euclidean vector passed in
	(same dimensions, same magnitudes)
	*/
	SECTION("move constructor tests") {
		// normal constructor and copy constructor (using init list constructor)
		auto initialiser_list = {101.4, 1.6, 45.3, 1111.5};
		auto init_list_constructor = comp6771::euclidean_vector(initialiser_list);
		// move constructor
		auto constructor = comp6771::euclidean_vector(std::move(init_list_constructor));
		auto magnitudes = std::vector<double>();
		magnitudes.push_back(101.4);
		magnitudes.push_back(1.6);
		magnitudes.push_back(45.3);
		magnitudes.push_back(1111.5);
		// check if old one is deleted
		CHECK(init_list_constructor.dimensions() != 4);
		CHECK(init_list_constructor.magnitudes() != magnitudes);

		// check dimensions of vector is 4
		CHECK(constructor.dimensions() == 4);
		// check magnitudes of vector is variable magnitudes
		CHECK(constructor.magnitudes() == magnitudes);
		// check no exceptions thrown (need new constructor to move because old one is deleted)
		auto init_list_constructor_new = comp6771::euclidean_vector(initialiser_list);
		CHECK_NOTHROW(comp6771::euclidean_vector(std::move(init_list_constructor_new)));
	}
}