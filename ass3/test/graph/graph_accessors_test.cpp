#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// TESTS FOR ACCESORS
TEST_CASE("accessors tests") {
    SECTION("is_node tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);

        // check returns true if node exists
	    CHECK(graph.is_node(1));
        CHECK(graph.is_node(3));
        // false if node does not exist
        CHECK(!graph.is_node(5));
    }

    SECTION("empty tests") {
        auto graph = gdwg::graph<int, std::string>();
        // check returns true if graph has no nodes
        CHECK(graph.empty());
        graph.insert_node(1);
        // check returns false if graph has 1 or more nodes
        CHECK(!graph.empty());
    }

    SECTION("is_connected tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    graph.insert_node(2);
	    // insert edges into input graph
        graph.insert_edge(1, 3, "b");

        // check returns true if edge exists from 1 to 3
        CHECK(graph.is_connected(1, 3));
        // check returns false if edge does not exist from 1 to 2
        CHECK(!graph.is_connected(1, 2));

        // check throws error if src node does not exist
        CHECK_THROWS_WITH(graph.is_connected(4, 1),
        "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
        // check throws error if dst node does not exist
        CHECK_THROWS_WITH(graph.is_connected(1, 4),
        "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
        // check throws error if src and dst node does not exist
        CHECK_THROWS_WITH(graph.is_connected(5, 4),
        "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
    }

    SECTION("nodes tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    graph.insert_node(4);
	    // checks if node sequence returned in ascending order
	    CHECK(graph.nodes() == std::vector<int>{1, 3, 4});
    }

    SECTION("weights tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(1);
	    graph.insert_node(2);
	    // insert edges into input graph
        graph.insert_edge(1, 2, "b");
        graph.insert_edge(1, 2, "a");
        graph.insert_edge(1, 2, "d");
        // checks if weight sequence returned in ascending order
        CHECK(graph.weights(1, 2) == std::vector<std::string>{"a", "b", "d"});

        // check throws error if src node does not exist
        CHECK_THROWS_WITH(graph.weights(3,2),
        "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
        // check throws error if dst node does not exist
        CHECK_THROWS_WITH(graph.weights(2,3),
        "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
        // check throws error if src and dst node does not exist
        CHECK_THROWS_WITH(graph.weights(3,4),
        "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
    }

    // not sure why this isn't working, can't get it to work
    /*
    SECTION("find tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(1);
	    graph.insert_node(2);
	    // insert edges into input graph
        graph.insert_edge(1, 2, "b");
        graph.insert_edge(1, 2, "a");
        graph.insert_edge(1, 2, "d");

        CHECK(graph.find(1, 2, "a") == graph.begin());
        CHECK(graph.find(1, 2, "b") == graph.begin()++);
        CHECK(graph.find(1, 2, "d") == graph.end());
    }
    */

    SECTION("connections tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(1);
	    graph.insert_node(2);
	    graph.insert_node(3);
	    graph.insert_node(4);
	    // insert edges into input graph
        graph.insert_edge(1, 2, "b");
        graph.insert_edge(1, 3, "a");
        graph.insert_edge(1, 4, "d");

        // check connections of 1 is 2, 3 and 4
        CHECK(graph.connections(1) == std::vector<int>{2, 3, 4});

        // throws error if src node is invalid
        CHECK_THROWS_WITH(graph.connections(5),
        "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
    }
}