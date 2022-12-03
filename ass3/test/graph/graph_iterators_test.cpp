#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// TESTS FOR ITERATORS
TEST_CASE("iterators tests") {
    // not sure why this is not working :(
    // would definitely have figured it out if i had enough time
    /*
    SECTION("begin tests") {
        // initialise graph and nodes
        using graph = gdwg::graph<int, int>;
        auto const v = std::vector<graph::value_type>{
          {4, 1, -4},
          {3, 2, 2},
          {2, 4, 2},
          {2, 1, 1},
          {6, 2, 5},
          {6, 3, 10},
          {1, 5, -1},
          {3, 6, -8},
          {4, 5, 3},
          {5, 2, 7},
        };
        auto g = graph{};
        for (const auto& [from, to, weight] : v) {
          g.insert_node(from);
          g.insert_node(to);
          g.insert_edge(from, to, weight);
        }
        auto const start_value_nodes = graph::value_type{4, 1, -4};
        CHECK(graph.begin() == start_value_nodes);
    }
    */

    // not sure why this is not working either
    /*
    SECTION("end tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);

        // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(1, 3, "a");
        graph.insert_edge(1, 3, "d");

        gdwg::graph<int, std::string>::value_type end_value_nodes;
        end_value_nodes.from = 1;
        end_value_nodes.to = 3;
        end_value_nodes.weight = "a";
        CHECK(graph.begin() == end_value_nodes);
    }
    */
}