#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

// TESTS FOR MODIFIERS
TEST_CASE("modifiers tests") {
    SECTION("insert_node tests") {
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    graph.insert_node(4);
	    graph.insert_node(2);
	    // check inserting new node returns true
	    CHECK(graph.insert_node(5));
	    // check node is actually inserted
		CHECK(graph.nodes() == std::vector<int>{1,2,3,4,5});
		// check inserting already existing node returns false
		CHECK(!(graph.insert_node(1)));
		// check node is not inserted (no duplicates)
		CHECK(graph.nodes() == std::vector<int>{1,2,3,4,5});
	}

	SECTION("insert_edge tests") {
	    // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);

        // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(1, 3, "a");
        // check inserting new edge returns true
        CHECK(graph.insert_edge(1, 3, "c"));
        // check inserting already existing edge returns false
        CHECK(!graph.insert_edge(1, 3, "c"));

        // check that edges are added to graph (a,b,c) added
        CHECK(graph.weights(1, 3) == std::vector<std::string>{"a", "b", "c"});

        // throws with invalid src node
        CHECK_THROWS_WITH(graph.insert_edge(5, 1, "a"),
        "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
        // throws with invalid dst node
        CHECK_THROWS_WITH(graph.insert_edge(1, 5, "a"),
        "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
        // throws with invalid src and dst node
        CHECK_THROWS_WITH(graph.insert_edge(5, 6, "a"),
        "Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
	}

	SECTION("replace node tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    graph.insert_node(2);
	    // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(1, 3, "a");

        // check if returns false since 2 is already a node
        CHECK(!graph.replace_node(1, 2));
        // check that does nothing since 2 is already a node
        CHECK(graph.weights(1, 3) == std::vector<std::string>{"a", "b"});

        // check if returns true since 4 is not already a node
        CHECK(graph.replace_node(1,4));
        // check that 1 is replaced by 4
        // 4 has 1's existing data
        CHECK(graph.weights(4,3) == std::vector<std::string>{"a", "b"});
        // 1 no longer exists as a node (replaced by 4)
        CHECK(graph.nodes() == std::vector<int>{2,3,4});
        // if old_data node does not exist, throw error
        CHECK_THROWS_WITH(graph.replace_node(5,6),
        "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
	}

	SECTION("merge_replace_node tests") {
	    // for correctness, use diagrammatic example given in spec to check
	    // initialise graph and nodes
        auto graph = gdwg::graph<std::string, int>();
	    graph.insert_node("A");
	    graph.insert_node("B");
	    graph.insert_node("C");
	    graph.insert_node("D");
	    // insert edges into input graph
        graph.insert_edge("A", "B", 3);
        graph.insert_edge("C", "B", 2);
        graph.insert_edge("D", "B", 4);
        // check output after merge replace node B by A (same as diagram)
        graph.merge_replace_node("B", "A");
        CHECK(graph.nodes() == std::vector<std::string>{"A", "C", "D"});
        CHECK(graph.weights("A", "A") == std::vector<int>{3});
        CHECK(graph.weights("C", "A") == std::vector<int>{2});
        CHECK(graph.weights("D", "A") == std::vector<int>{4});

        // check error thrown with invalid old_data node
        CHECK_THROWS_WITH(graph.merge_replace_node("E", "D"),
        "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
	    // check error thrown with invalid new_data node
        CHECK_THROWS_WITH(graph.merge_replace_node("D", "E"),
        "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
        // check error thrown with invalid old_data and new_data node
        CHECK_THROWS_WITH(graph.merge_replace_node("E", "F"),
        "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
	}

	SECTION("erase_node tests") {
	    // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(3, 1, "a");
        // erase node 1, then check if returns true
        CHECK(graph.erase_node(1));
        // check if node 1 no longer exists in graph
        CHECK(graph.nodes() == std::vector<int>{3});
        // check edges no longer exist (error thrown because node no longer exists)
        CHECK_THROWS_WITH(graph.is_connected(1, 3),
		"Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");

        // check returns false if node is not removed (node does not exist)
        CHECK(!graph.erase_node(4));
	}

    SECTION("erase_edge src, dst, weight tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(1, 3, "a");
        graph.insert_edge(1, 3, "c");
        // check if returns true (erase valid edge)
        CHECK(graph.erase_edge(1, 3, "b"));
        // check if given edge has been erased (only a and c left)
        CHECK(graph.weights(1, 3) == std::vector<std::string>{"a", "c"});

        // check if returns false (erase invalid edge)
        CHECK(!graph.erase_edge(1, 3, "abc"));

        // check if throws error if src is invalid node
        CHECK_THROWS_WITH(graph.erase_edge(4, 3, "a"),
        "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
        // check if throws error if dst is invalid node
        CHECK_THROWS_WITH(graph.erase_edge(3, 4, "a"),
        "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
        // check if throws error if src and dst is invalid node
        CHECK_THROWS_WITH(graph.erase_edge(4 ,5 , "a"),
        "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
    }

    // TODO: not enough time to finish erase_edge iterator and iterator range :(
    SECTION("erase_edge iterator tests") {
    }
    SECTION("erase_edge iterator range tests") {
    }

    SECTION("clear tests") {
        // initialise graph and nodes
        auto graph = gdwg::graph<int, std::string>();
	    graph.insert_node(3);
	    graph.insert_node(1);
	    // insert edges into input graph
        graph.insert_edge(1, 3, "b");
        graph.insert_edge(1, 3, "a");
        graph.insert_edge(1, 3, "c");

        graph.clear();
        // erase all nodes and check if empty is true after clear
        CHECK(graph.empty());
    }
}