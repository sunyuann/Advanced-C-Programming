/*
rationale for my test files:
Each test case file represents constructors, accessors, extractor, iterators and modifiers tests.
In each test case file, each section represents different of their respective section's tests
(Usually separated as one function per section). I checked to make sure their functionality is
correct and checked that sections throw errors with appropriate messages when neccessary and throw
none when the specification states that it does not throw, or the parameters given do not break the
conditions given for an exception to occur.
*/

#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// TESTS FOR CONSTRUCTORS
TEST_CASE("constructor tests") {
    // check if default constructor is default initialised (check if empty)
	SECTION("default constructor test") {
	    auto default_constructor = gdwg::graph<int, std::string>();
	    CHECK(default_constructor.empty());
	}

	SECTION("initialiser list constructor test") {
	    auto initialiser_list = std::initializer_list<int>{1, 3, 50, 40};
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
	    auto sorted_initialiser_list = std::initializer_list<int>{1, 3, 40, 50};
	    auto list = static_cast<std::vector<int>>(sorted_initialiser_list);
	    // make sure list of nodes are same as initializer list passed in and sorted
	    CHECK(initialiser_list_constructor.nodes() == list);
	}

	SECTION("input iterator list constructor test") {
		auto initialiser_list = std::initializer_list<int>{500, 1, 25, 37, 50};
		// use initialiser list constructor on initializer list
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
	    // use input iterator list constructor on beginning of initializer list to end of list
	    auto input_iterator_list_constructor = gdwg::graph<int, std::string>(initialiser_list.begin(), initialiser_list.end());
        // check that both graphs constructed are the same
        CHECK(input_iterator_list_constructor == initialiser_list_constructor);
	}

	SECTION("move constructor tests") {
        auto initialiser_list = std::initializer_list<int>{2, 49, 1, 25, 37, 50};
		// use initialiser list constructor on initializer list
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
        // move initialiser list constructor
        auto move_constructor = std::move(initialiser_list_constructor);
        auto initialiser_list_constructor2 = gdwg::graph<int, std::string>(initialiser_list);
        CHECK(move_constructor == initialiser_list_constructor2);
	}

	SECTION("move-assign constructor test") {
        auto initialiser_list = std::initializer_list<int>{2, 49, 1, 25, 37, 50};
		// use initialiser list constructor on initializer list
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
        // move assign initialiser list constructor
        auto move_assign_constructor = gdwg::graph<int, std::string>(std::move(initialiser_list_constructor));
        auto initialiser_list_constructor2 = gdwg::graph<int, std::string>(initialiser_list);
        CHECK(move_assign_constructor == initialiser_list_constructor2);
	}

	SECTION("copy constructor test") {
	    auto initialiser_list = std::initializer_list<int>{2, 49, 1, 25, 37, 50};
		// use initialiser list constructor on initializer list
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
        // copy initialiser list constructor
        auto copy_constructor = gdwg::graph<int, std::string>(initialiser_list_constructor);
        auto initialiser_list_constructor2 = gdwg::graph<int, std::string>(initialiser_list);
        CHECK(copy_constructor == initialiser_list_constructor2);
	}

	SECTION("copy assign constructor test") {
	    auto initialiser_list = std::initializer_list<int>{2, 49, 1, 25, 37, 50};
		// use initialiser list constructor on initializer list
	    auto initialiser_list_constructor = gdwg::graph<int, std::string>(initialiser_list);
        // copy assign initialiser list constructor
        auto copy_assign_constructor = initialiser_list_constructor;
        CHECK(copy_assign_constructor == initialiser_list_constructor);
	}
}